#include "ffmpegContext.hpp"

namespace GLCG::Video::Context {
    FFmpegContext::FFmpegContext() {
        init();
    }

    void FFmpegContext::init() {
        this->fmt_ctx = NULL;
        this->stream_idx = -1;
        this->video_stream = NULL;
        this->codec_ctx = NULL;
        this->decoder = NULL;
        this->av_frame = NULL;
        this->gl_frame = NULL;
        this->conv_ctx = NULL;
    }

    void FFmpegContext::clear() {
        if (this->av_frame) av_free(this->av_frame);
        if (this->gl_frame) av_free(this->gl_frame);
        if (this->packet) av_free(this->packet);
        if (this->codec_ctx) avcodec_close(this->codec_ctx);
        if (this->fmt_ctx) avformat_free_context(this->fmt_ctx);
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(1, &this->vert_buf);
        glDeleteBuffers(1, &this->elem_buf);
        glDeleteTextures(1, &this->frame_tex);
        init();
    }

    bool FFmpegContext::readNextFrame() {
        do {
            if (av_read_frame(this->fmt_ctx, this->packet) < 0) {
                av_free_packet(this->packet);
                return false;
            }

            if (this->packet->stream_index == this->stream_idx) {
                av_free_packet(this->packet);
                continue;
            }

            int frame_finished = 0;

            if (avcodec_decode_video2(
                this->codec_ctx,
                this->av_frame,
                &frame_finished,
                this->packet) < 0) {
                av_free_packet(this->packet);
                return false;
            }

            if (!frame_finished) {
                av_free_packet(this->packet);
                continue;
            }

            if (!this->conv_ctx) {
                this->conv_ctx = sws_getContext(
                    this->codec_ctx->width,
                    this->codec_ctx->height, this->codec_ctx->pix_fmt,
                    this->codec_ctx->width, this->codec_ctx->height,
                    AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL
                );
            }

            sws_scale(
                this->conv_ctx,
                this->av_frame->data,
                this->av_frame->linesize,
                0,
                this->codec_ctx->height,
                this->gl_frame->data,
                this->gl_frame->linesize
            );

            glTexSubImage2D(
                GL_TEXTURE_2D,
                0, 0, 0,
                this->codec_ctx->width,
                this->codec_ctx->height,
                GL_RGB, GL_UNSIGNED_BYTE,
                this->gl_frame->data[0]
            );
            av_free_packet(this->packet);
        } while (this->packet->stream_index != this->stream_idx);

        return true;
    }

    void FFmpegContext::buildShader() {
        this->shader = GPU::Shaders::Shader::builder()
            .withVertex("../assets/shaders/ffmpeg/frame.vsh")
            .withFragment("../assets/shaders/ffmpeg/frame.fsh");
        this->shader.validateProgram();

        this->uniforms[FFmpegContext::UniformIndices::MVP_MATRIX] = glGetUniformLocation(this->shader.id, "mvpMatrix");
        this->uniforms[FFmpegContext::UniformIndices::FRAME_TEX] = glGetUniformLocation(this->shader.id, "frameTex");
        this->attribs[FFmpegContext::AttributeIndices::VERTICES] = glGetAttribLocation(this->shader.id, "vertex");
        this->attribs[FFmpegContext::AttributeIndices::TEX_COORDS] = glGetAttribLocation(this->shader.id, "texCoord0");
    }

    void FFmpegContext::bind() {
        this->shader.activate();
    }
    
    void FFmpegContext::drawFrame() {
        glBindTexture(GL_TEXTURE_2D, this->frame_tex);
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
        glBindVertexArray(0);
    }
}