#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "texture.hpp"

namespace GLCG::Resources {
    Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
        this->type = texType;

        int widthImg = 0;
        int heightImg = 0;
        int numColCh = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
        if (bytes == nullptr) {
            throw std::exception("Unable to load image");
        }

        glGenTextures(1, &this->id);
        glActiveTexture(slot);
        glBindTexture(texType, this->id);

        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Extra lines in case you choose to use GL_CLAMP_TO_BORDER
        // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

        glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
        glGenerateMipmap(texType);

        stbi_image_free(bytes);

        glBindTexture(texType, 0);
    }

    void Texture::assignTextureUnit(GPU::Shaders::Shader& shader, const char* uniform, GLuint unit){
        GLuint uniformLocation = glGetUniformLocation(shader.id, uniform);
        shader.activate();
        glUniform1i(uniformLocation, unit);
    }

    void Texture::bind() {
        glBindTexture(this->type, this->id);
    }

    void Texture::unbind() {
        glBindTexture(this->type, 0);
    }

    void Texture::destroy() {
        glDeleteTextures(1, &this->id);
    }
}