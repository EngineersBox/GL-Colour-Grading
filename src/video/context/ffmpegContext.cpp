#include "ffmpegContext.hpp"

#include <stdexcept>

namespace GLCG::Video::Context {
    FFmpegContext::FFmpegContext(std::string& filename) {
        open(filename);
    }

    FFmpegContext::~FFmpegContext() {
        close();
    }

    void FFmpegContext::open(std::string& filename) {
        if (!(this->avFormatCtx = avformat_alloc_context())) {
            throw std::runtime_error("Unable to create FFmpeg AVFormatContext");
        } else if (avformat_open_input(&this->avFormatCtx, filename.c_str(), nullptr, nullptr) != 0) {
            throw std::runtime_error(Utils::String::format(
                "Unable to open stream from file: %s",
                filename.c_str()
            ));
        }
        this->videoStreamIndex = -1;
        AVCodecParameters* avCodecParameters;
        AVCodec* avCodec;
        for (int i = 0; i < this->avFormatCtx->nb_streams; i++) {
            avCodecParameters = avFormatCtx->streams[i]->codecpar;
            if (!(avCodec = avcodec_find_decoder(avCodecParameters->codec_id))) {
                continue;
            } else if (avCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
                this->videoStreamIndex = i;
                this->width = avCodecParameters->width;
                this->height = avCodecParameters->height;
                this->timeBase = this->avFormatCtx->streams[i]->time_base;
                break;
            }
        }
        if (this->videoStreamIndex == -1) {
            throw std::runtime_error("No valid video streams could be found");
        } else if (!(this->avCodecCtx = avcodec_alloc_context3(avCodec))) {
            throw std::runtime_error("Unable to create AVCodecContext from stream");
        } else if (avcodec_parameters_from_context(avCodecParameters, this->avCodecCtx) < 0) {
            throw std::runtime_error("Unable to initialise AVCodeContext");
        } else if (avcodec_open2(this->avCodecCtx, avCodec, nullptr) < 0) {
            throw std::runtime_error("Unable to open codec");
        } else if (!(this->avFrame = av_frame_alloc())) {
            std::runtime_error("Unable to allocate frame");
        } else if (!(this->avPacket = av_packet_alloc())) {
            std::runtime_error("Unable to allocate packet");
        }
    }

    void FFmpegContext::close() {
        sws_freeContext(this->swsScalerCtx);
        avformat_close_input(&this->avFormatCtx);
        avformat_free_context(this->avFormatCtx);
        av_frame_free(&this->avFrame);
        av_packet_free(&this->avPacket);
        avcodec_free_context(&this->avCodecCtx);
    }

    const char* FFmpegContext::getAVError(int errnum) {
        // Fixes GCC compilation error with return of temporary pointer
        static char str[AV_ERROR_MAX_STRING_SIZE];
        memset(str, 0, sizeof(str));
        return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
    }

    AVPixelFormat FFmpegContext::correctPixelFormat(AVPixelFormat pix_fmt) {
        // Fix swscaler deprecated pixel format warning
        // (YUVJ has been deprecated, change pixel format to regular YUV)
        switch (pix_fmt) {
            case AV_PIX_FMT_YUVJ420P: return AV_PIX_FMT_YUV420P;
            case AV_PIX_FMT_YUVJ422P: return AV_PIX_FMT_YUV422P;
            case AV_PIX_FMT_YUVJ444P: return AV_PIX_FMT_YUV444P;
            case AV_PIX_FMT_YUVJ440P: return AV_PIX_FMT_YUV440P;
            default: return pix_fmt;
        }
    }

    void FFmpegContext::readFrameToCurrentContext() {
        int response;
        while (av_read_frame(this->avFormatCtx, this->avPacket) >= 0) {
            if (this->avPacket->stream_index != this->videoStreamIndex) {
                av_packet_unref(this->avPacket);
                continue;
            }
            response = avcodec_receive_frame(this->avCodecCtx, this->avFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                av_packet_unref(this->avPacket);
                continue;
            } else if (response < 0) {
                std::runtime_error(Utils::String::format(
                    "Failed to decode packet: %s",
                    getAVError(response)
                ));
            }
            av_packet_unref(this->avPacket);
            break;
        }
    }

    void FFmpegContext::readFrame(uint8_t *buffer, int64_t *pts) {
        readFrameToCurrentContext();
        *pts = this->avFrame->pts;

        if (!this->swsScalerCtx) {
            if (!(this->swsScalerCtx = sws_getContext(
                this->width,
                this->height,
                correctPixelFormat(this->avCodecCtx->pix_fmt),
                this->width,
                this->height,
                AV_PIX_FMT_RGB0,
                SWS_BILINEAR,
                nullptr, nullptr, nullptr
            ))) {
                throw std::runtime_error("Unable to initialise SWS");
            }
        }
        uint8_t* dest[4] = {
            buffer,
            nullptr, nullptr, nullptr
        };
        int destLineSize[4] = {
            static_cast<int>(this->width * 4),
            0, 0, 0
        };
        sws_scale(
            this->swsScalerCtx,
            this->avFrame->data,
            this->avFrame->linesize,
            0,
            this->avFrame->height,
            dest,
            destLineSize
        );
    }

    void FFmpegContext::seekFrame(int64_t timestamp) {
        av_seek_frame(
            this->avFormatCtx,
            this->videoStreamIndex,
            timestamp,
            AVSEEK_FLAG_BACKWARD
        );
        readFrameToCurrentContext();
    }

//    void FFmpegContext::buildShader() {
//        this->shader = GPU::Shaders::Shader::builder()
//            .withVertex("../assets/shaders/ffmpeg/frame.vsh")
//            .withFragment("../assets/shaders/ffmpeg/frame.fsh");
//        this->shader.validateProgram();
//
//        this->uniforms[FFmpegContext::UniformIndices::MVP_MATRIX] = glGetUniformLocation(this->shader.id, "mvpMatrix");
//        this->uniforms[FFmpegContext::UniformIndices::FRAME_TEX] = glGetUniformLocation(this->shader.id, "frameTex");
//        this->attribs[FFmpegContext::AttributeIndices::VERTICES] = glGetAttribLocation(this->shader.id, "vertex");
//        this->attribs[FFmpegContext::AttributeIndices::TEX_COORDS] = glGetAttribLocation(this->shader.id, "texCoord0");
//    }
//
//    void FFmpegContext::bind() {
//        this->shader.activate();
//    }
//
//    void FFmpegContext::drawFrame() {
//        glBindTexture(GL_TEXTURE_2D, this->frame_tex);
//        glBindVertexArray(this->vao);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
//        glBindVertexArray(0);
//    }
}