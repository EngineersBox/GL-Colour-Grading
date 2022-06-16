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
#include "../../stream/closable.hpp"

namespace GLCG::Device::Video::Context {
    class FFmpegContext: protected Stream::Closable {
        public:
            FFmpegContext(std::string& filename);
            virtual ~FFmpegContext();

            virtual void readFrame(uint8_t* buffer, int64_t* pts);
            virtual void seekFrame(int64_t timestamp);

            void buildShader();
            void bind();
            void drawFrame();
        protected:
            void open() override;
            void close() override;
        private:
            static const char* getAVError(int errnum);
            static AVPixelFormat correctPixelFormat(AVPixelFormat pix_fmt);

            void readFrameToCurrentContext();

            std::string filename;
            uint32_t width;
            uint32_t height;

            AVRational timeBase;
            AVFormatContext* avFormatCtx;
            AVCodecContext* avCodecCtx;
            int videoStreamIndex;
            AVFrame* avFrame;
            AVPacket* avPacket;
            SwsContext* swsScalerCtx;

//            GPU::Shaders::Shader shader;
//            GLuint vao;
//            GLuint vert_buf;
//            GLuint elem_buf;
//            GLuint frame_tex;
//            GLuint attribs[2];
//            GLuint uniforms[2];
//
//            enum AttributeIndices {
//                VERTICES = 0,
//                TEX_COORDS
//            };
//
//            enum UniformIndices {
//                MVP_MATRIX = 0,
//                FRAME_TEX
//            };
    };
}

#endif //GL_COLOUR_GRADING_FFMPEGCONTEXT_HPP
