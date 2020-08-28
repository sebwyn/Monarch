#include "TrueTypeFont.h"

#include <iostream>

using namespace Monarch;

TrueTypeFont::TrueTypeFont(const std::string& fontName, int fontSize){
    m_atlas = texture_atlas_new(512, 512, 1);
    m_font = texture_font_new_from_file(m_atlas, fontSize, fontName.c_str());
    
    for(unsigned char c = 0; c < 128; c++){
        texture_font_get_glyph(m_font, (const char*)&c);
    }

    m_texture = new Texture(m_atlas->data, 512, 512, 1);
    //temporary text color/alpha solution
    int mask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
    m_texture->setSwizzleMask(mask);
}

TrueTypeFont::~TrueTypeFont(){
    texture_font_delete(m_font);
    texture_atlas_delete(m_atlas);
    delete m_texture;
}


TrueTypeFont::Character TrueTypeFont::getCharacter(const char& c){
    texture_glyph_t* glyph = texture_font_get_glyph(m_font, &c);
    if(glyph){
        Character c;
        c.bottomLeftTexCoord = glm::vec2(glyph->s0, glyph->t1);
        c.topRightTexCoord = glm::vec2(glyph->s1, glyph->t0);
        
        c.offsetX = glyph->offset_x;
        c.offsetY = glyph->offset_y;
        c.width = glyph->width;
        c.height = glyph->height;
        
        c.advance = glyph->advance_x;

        return c;
    }
    return {glm::vec2(0, 0), glm::vec2(0, 0), 0, 0, 0, 0, 0};
}

float TrueTypeFont::getKerning(const char& c1, const char& c2){
    texture_glyph_t* glyph = texture_font_get_glyph(m_font, &c1);
    if(glyph){
        return texture_glyph_get_kerning(glyph, &c2);
    }
    return 0;
}