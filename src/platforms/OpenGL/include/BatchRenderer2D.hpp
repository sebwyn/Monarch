#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>

#include "ShaderProgram.hpp"

namespace Monarch {

class BatchRenderer2D {
public:
    static constexpr int MAX_QUADS = 1000;
    static constexpr int MAX_TEXTURES = 16;

    static const char *vertPath, *fragPath;

    static void init();
    static void destroy();

    static void beginBatch();
    static void endBatch();

    static void setMatrices(glm::mat4 projection, glm::mat4 view){
        projectionMatrix = projection;
        viewMatrix = view;
    }

    static void flush();

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 size;
        glm::vec2 texCoords;
        float texID;
    };

    struct QuadData {
        glm::vec3 position = {0,0,0};
        glm::vec2 size = {1,1};
        glm::vec4 color = {1,1,1,1};
        glm::vec2 bottomLeftTexCoords = {0,0};
        glm::vec2 topRightTexCoords = {1,1};
        int texID = 0;
    };

    static void drawQuad(QuadData& data);

private:

    static int useTexture(unsigned int tex);

    static unsigned int vao, vbo, ebo;
    static unsigned int blankTexture;

    static ShaderProgram* shader;

    static Vertex* vertexBuffer;
    static Vertex* currentVertex;

    static int numQuads;
    
    static unsigned int textures[MAX_TEXTURES];
    static int currentTexture;

    static glm::mat4 projectionMatrix;
    static glm::mat4 viewMatrix;
};
}
