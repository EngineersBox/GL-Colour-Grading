#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include "texture.hpp"

namespace GLCG::Resources {
    inline void Texture::generateTexImage(uint8_t const* bytes) const {
        GLenum format;
        switch (this->channels) {
            case 4:
                format = GL_RGBA;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 1:
                format = GL_RED;
                break;
            default:
                throw std::invalid_argument("Automatic Texture type recognition failed");
        }
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            this->getWidth(),
            this->getHeight(),
            0,
            format,
            GL_UNSIGNED_BYTE,
            bytes
        );
    }

    Texture::Texture(const char* image, const char* texType, GLuint slot) {
        this->type = texType;

        stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(
            image,
            &this->width,
            &this->height,
            &this->channels,
            0
        );
        if (bytes == nullptr) {
            throw std::exception("Unable to load image");
        }

        glGenTextures(1, &this->id);
        glActiveTexture(GL_TEXTURE0 + slot);
        unit = slot;
        glBindTexture(GL_TEXTURE_2D, this->id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

        this->generateTexImage(bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(bytes);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::assignTextureUnit(GPU::Shaders::Shader& shader, const char* uniform, GLuint unit){
        GLuint uniformLocation = glGetUniformLocation(shader.id, uniform);
        shader.activate();
        glUniform1i(uniformLocation, unit);
    }

    void Texture::bind() {
        glActiveTexture(GL_TEXTURE0 + this->unit);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }

    void Texture::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::destroy() {
        glDeleteTextures(1, &this->id);
    }
}