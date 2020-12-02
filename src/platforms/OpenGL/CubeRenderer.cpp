#include "CubeRenderer.hpp"

#include "MonarchConfig.hpp"

using namespace Monarch;

std::string CubeRenderer::vertPath = std::string(MONARCH_PROJECT_DIR) + std::string("assets/shaders/cubeVert.glsl");
std::string CubeRenderer::fragPath = std::string(MONARCH_PROJECT_DIR) + std::string("assets/shaders/cubeFrag.glsl");

unsigned int CubeRenderer::MAX_CUBES = 1000;
unsigned int CubeRenderer::vao, CubeRenderer::vbo, CubeRenderer::ebo;
glm::mat4 projectionMatrix, viewMatrix;

ShaderProgram* CubeRenderer::shaderProgram;

CubeRenderer::Vertex* CubeRenderer::vertices;
std::vector<unsigned short> CubeRenderer::indices;

glm::mat4 CubeRenderer::projectionMatrix, CubeRenderer::viewMatrix;
CubeRenderer::Vertex* CubeRenderer::currentVertex;

glm::vec3 CubeRenderer::cubeVertices[8] = {
    glm::vec3(-1.0, -1.0, -1.0),
    glm::vec3(-1.0, -1.0,  1.0),
    glm::vec3(-1.0,  1.0, -1.0),
    glm::vec3(-1.0,  1.0,  1.0),
    glm::vec3( 1.0, -1.0, -1.0),
    glm::vec3( 1.0, -1.0,  1.0),
    glm::vec3( 1.0,  1.0, -1.0),
    glm::vec3( 1.0,  1.0,  1.0),
};

unsigned short CubeRenderer::cubeIndices[36] = {
    //-X side
    1, 0, 2,
    1, 2, 3,
    //-Z side
    4, 0, 2,
    4, 2, 6,
    //+X side
    5, 4, 6,
    5, 6, 7,
    //+Z side
    5, 3, 1,
    5, 7, 3,
    //Top
    2, 6, 7,
    2, 7, 3,
    //Bottom
    0, 5, 4,
    0, 1, 5

};

unsigned short CubeRenderer::numCubes = 0;

void CubeRenderer::init()
{
    glDisable(GL_DEPTH_TEST | GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    vertices = new Vertex[MAX_CUBES * 8];

    shaderProgram = new ShaderProgram(vertPath.c_str(), fragPath.c_str());

    indices = std::vector<unsigned short>(MAX_CUBES*36);
    for(int c = 0; c < MAX_CUBES; c++){
        unsigned short cubeStart = c*8;
        unsigned short* cube = &(indices[c*36]);
        for(int i = 0; i < 36; i++){
            cube[i] = cubeStart + cubeIndices[i];
        } 
    }

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 8*MAX_CUBES*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 36*MAX_CUBES*sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
    glEnableVertexAttribArray(1);
}

void CubeRenderer::destroy()
{
    delete[] vertices;
    delete shaderProgram;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void CubeRenderer::beginBatch(){
    numCubes = 0;
    currentVertex = vertices;
}

void CubeRenderer::endBatch(){
    GLsizeiptr size = (uint8_t*)currentVertex - (uint8_t*)vertices;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void CubeRenderer::drawCube(CubeRenderer::CubeData& cube){
    if(numCubes == MAX_CUBES){
        endBatch();
        flush();
        beginBatch();
    }
    for(int v = 0; v < 8; v++){
        currentVertex->position = cube.center + glm::vec3(cube.halfWidth)*cubeVertices[v]; 
        currentVertex->color = cube.color;
        currentVertex++;
    }
    numCubes++;
}

void CubeRenderer::flush(){
    glBindVertexArray(vao); shaderProgram->start();
    shaderProgram->setUniformMat4(viewMatrix, "viewMatrix");
    shaderProgram->setUniformMat4(projectionMatrix, "projectionMatrix");

    glDrawElements(GL_TRIANGLES, numCubes*36, GL_UNSIGNED_SHORT, 0);
}
