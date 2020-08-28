#pragma once

#include "Texture.hpp"

#include <string>

#include <freetype-gl.h>
#include <glm/glm.hpp>

namespace Monarch {

//TODO: make seperate versions for different apis
//TODO: support more characters
class TrueTypeFont {
public:
    TrueTypeFont(const std::string& font, int fontSize);
    ~TrueTypeFont();

    struct Character {
        glm::vec2 bottomLeftTexCoord, topRightTexCoord;

        //everything in pixels
        unsigned int offsetX, offsetY, width, height;
        float advance, kerning;
    };

    Character getCharacter(const char& c);
    float getKerning(const char& c1, const char& c2);

    inline unsigned int getTexId() const { return m_texture->get(); }
private:
    texture_atlas_t* m_atlas;
    texture_font_t* m_font;
    Texture* m_texture;
};

}
