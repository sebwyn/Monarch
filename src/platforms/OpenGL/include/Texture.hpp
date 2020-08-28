#pragma once

//opengl includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
namespace Monarch {

class Texture {
public:
    Texture(unsigned char* data, int width, int height, int nrChannels);
    Texture(const char* path);

    ~Texture();

    void bind(int index);

    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }

    inline unsigned int get() const { return m_texture; }

    //temporarily important for text
    void setSwizzleMask(const int* mask){
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, mask);
    }

    static void setPixelAlignment(int i){ glPixelStorei(GL_UNPACK_ALIGNMENT, i); }

private:
    void createTexture(unsigned char* data);

    unsigned int m_texture;
    int m_width, m_height, m_nrChannels;
};
}