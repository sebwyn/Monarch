#pragma once

#include "ShaderProgram.hpp"

namespace Monarch {

class CubeRenderer {
public:
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
        glm::vec3 color;
    };

    struct CubeData {
        glm::vec3 center = {0,0,0};
        float halfWidth = 0.5;
        glm::vec3 color = {1,1,1};
    };

    static void drawCube(CubeData& cube);
private:
    static std::string vertPath, fragPath;
    
    static unsigned int MAX_CUBES;
    static unsigned int vao, vbo, ebo;
    static glm::mat4 projectionMatrix, viewMatrix;
    
    static Monarch::ShaderProgram* shaderProgram;

    static Vertex* vertices;
    static std::vector<unsigned short> indices;

    static glm::vec3 cubeVertices[8];
    static unsigned short cubeIndices[36];

    static unsigned short numCubes;
    static Vertex* currentVertex;
};

}
