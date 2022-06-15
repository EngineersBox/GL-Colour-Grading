#pragma once

#ifndef GL_COLOUR_GRADING_FFMPEGCONTEXT_HPP
#define GL_COLOUR_GRADING_FFMPEGCONTEXT_HPP

#include <glad/glad.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavfilter/avfilter.h>
    #include <libavdevice/avdevice.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavutil/avutil.h>
}

#include "../../gpu/shaders/shader.hpp"
#include "../../gpu/bindableResource.hpp"

namespace GLCG::Video::Context {
    class FFmpegContext: public GPU::BindableResource {
        public:
            FFmpegContext();
            void init();
            void clear();

            bool readNextFrame();
            void buildShader();

            virtual void bind() override;
            virtual void unbind() override = 0;
            virtual void destroy() override = 0;
            void drawFrame();
        private:
            AVFormatContext *fmt_ctx;
            int stream_idx;
            AVStream *video_stream;
            AVCodecContext *codec_ctx;
            AVCodec *decoder;
            AVPacket *packet;
            AVFrame *av_frame;
            AVFrame *gl_frame;
            struct SwsContext *conv_ctx;

            GPU::Shaders::Shader shader;
            GLuint vao;
            GLuint vert_buf;
            GLuint elem_buf;
            GLuint frame_tex;
            GLuint attribs[2];
            GLuint uniforms[2];

            enum AttributeIndices {
                VERTICES = 0,
                TEX_COORDS
            };

            enum UniformIndices {
                MVP_MATRIX = 0,
                FRAME_TEX
            };
    };
}

#endif //GL_COLOUR_GRADING_FFMPEGCONTEXT_HPP
