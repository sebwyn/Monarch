#include "Texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

using namespace Monarch;

Texture::Texture(unsigned char* data, int width, int height, int depth, int nrChannels)
 : m_width(width), m_height(height), m_depth(depth), m_nrChannels(nrChannels), m_glTexture(GL_TEXTURE_3D)
{
    createTexture(data);
}

Texture::Texture(unsigned char* data, int width, int height, int nrChannels)
 : m_width(width), m_height(height), m_depth(1), m_nrChannels(nrChannels), m_glTexture(GL_TEXTURE_2D)
{
    createTexture(data);
}

Texture::Texture(const char* path)
: m_depth(1), m_glTexture(GL_TEXTURE_2D) 
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);

    
    if(data) createTexture(data);
    else std::cout << "Failed to load texture" << std::endl;
    
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1,&m_texture);
}

void Texture::createTexture(unsigned char* data){
    glGenTextures(1, &m_texture);
    glBindTexture(m_glTexture, m_texture);

    glTexParameteri(m_glTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(m_glTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_glTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_glTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if(m_glTexture == GL_TEXTURE_3D){
        glTexParameteri(m_glTexture, GL_TEXTURE_WRAP_R, GL_REPEAT);
        switch (m_nrChannels) {
                case 1:
                    glTexImage3D(m_glTexture, 0, GL_RED, m_width, m_height, m_depth, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                    break;
                case 2:
                    glTexImage3D(m_glTexture, 0, GL_RG, m_width, m_height, m_depth, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                    break;
                case 3:
                    glTexImage3D(m_glTexture, 0, GL_RGB, m_width, m_height, m_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case 4:
                    glTexImage3D(m_glTexture, 0, GL_RGBA, m_width, m_height, m_depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
        }
    } else {
        switch (m_nrChannels) {
                case 1:
                    glTexImage2D(m_glTexture, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                    break;
                case 2:
                    glTexImage2D(m_glTexture, 0, GL_RG, m_width, m_height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                    break;
                case 3:
                    glTexImage2D(m_glTexture, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    break;
                case 4:
                    glTexImage2D(m_glTexture, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    break;
        }
    }
    //glGenerateMipmap(m_glTexture);
}

void Texture::bind(int index){
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_glTexture, m_texture);
}
