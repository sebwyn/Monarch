#include "Renderer.hpp"

using namespace Monarch;

const char* Renderer::vertPath = "../../../Monarch/assets/shaders/2D/vert.glsl";
const char* Renderer::fragPath = "../../../Monarch/assets/shaders/2D/frag.glsl";

unsigned int Renderer::vao, Renderer::vbo, Renderer::ebo;
unsigned int Renderer::blankTexture;

ShaderProgram* Renderer::shader;

int Renderer::numQuads;

Renderer::Vertex* Renderer::vertexBuffer;
Renderer::Vertex* Renderer::currentVertex;
    
unsigned int Renderer::textures[Renderer::MAX_TEXTURES];
int Renderer::currentTexture;

glm::mat4 Renderer::viewMatrix;
glm::mat4 Renderer::projectionMatrix;

void Renderer::init() {
    vertexBuffer = new Vertex[MAX_QUADS*4];

    shader = new ShaderProgram(vertPath, fragPath);

    short indices[Renderer::MAX_QUADS*6];
    for(int i = 0; i < Renderer::MAX_QUADS; i++){
        int quadStart = i*4;
        short* quad = &indices[i*6];
        quad[0] = quadStart+0;
        quad[1] = quadStart+1;
        quad[2] = quadStart+2;
        quad[3] = quadStart+2;
        quad[4] = quadStart+3;
        quad[5] = quadStart+0;
    }

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 4*MAX_QUADS*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texID));
    glEnableVertexAttribArray(3);

    glGenTextures(1, &blankTexture);
    glBindTexture(GL_TEXTURE_2D, blankTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    textures[0] = blankTexture;
    currentTexture = 1;


    //blending needed for text can be turned off later
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::destroy(){
    delete shader;
    delete[] vertexBuffer;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1,&blankTexture);
}

void Renderer::beginBatch(){
    numQuads = 0;
    currentTexture = 1;
    currentVertex = vertexBuffer;
}
void Renderer::endBatch(){
    GLsizeiptr size = (uint8_t*)currentVertex - (uint8_t*)vertexBuffer;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexBuffer);
}

void Renderer::drawQuad(QuadData& data){
    if(numQuads == Renderer::MAX_QUADS){
        endBatch();
        flush();
        beginBatch();
    }

    int tex = useTexture(data.texID);

    currentVertex->position  = data.position;
    currentVertex->color     = data.color;
    currentVertex->texCoords = data.bottomLeftTexCoords;
    currentVertex->texID     = (float)tex;
    currentVertex++;

    currentVertex->position  = {data.position.x, 
                                data.position.y+data.size.y,
                                data.position.z};
    currentVertex->color     = data.color;
    currentVertex->texCoords = {data.bottomLeftTexCoords.x, data.topRightTexCoords.y};
    currentVertex->texID     = (float)tex;
    currentVertex++;

    currentVertex->position  = {data.position.x+data.size.x, 
                                data.position.y+data.size.y,
                                data.position.z};
    currentVertex->color     = data.color;
    currentVertex->texCoords = data.topRightTexCoords;
    currentVertex->texID     = (float)tex;
    currentVertex++;

    currentVertex->position  = {data.position.x+data.size.x, 
                                data.position.y,
                                data.position.z};
    currentVertex->color     = data.color;
    currentVertex->texCoords = {data.topRightTexCoords.x, data.bottomLeftTexCoords.y};
    currentVertex->texID     = (float)tex;
    currentVertex++;

    numQuads++;
}

int Renderer::useTexture(unsigned int tex){
    //TODO: try to make this faster
    for (int i = 0; i < currentTexture; i++)
        if (textures[i] == tex) return i;
    
    if (currentTexture == MAX_TEXTURES) {
        endBatch();
        flush();
        beginBatch();
    }

    //std::cout << "Using Texture: " << currentTexture << std::endl;
    textures[currentTexture] = tex;
    return currentTexture++;

}

void Renderer::flush(){
    //std::cout << "Flushing" << std::endl;
    glBindVertexArray(vao);
    shader->start();
    int samplers[MAX_TEXTURES];
    for (int i = 0; i < MAX_TEXTURES; i++) {
        samplers[i] = i;
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    shader->setUniformIntArray(samplers, MAX_TEXTURES, "textures");
    shader->setUniformMat4(viewMatrix, "viewMatrix");
    shader->setUniformMat4(projectionMatrix, "projectionMatrix");

    glDrawElements(GL_TRIANGLES, numQuads*6, GL_UNSIGNED_SHORT, 0);
}
