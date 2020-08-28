#include "BatchRenderer2D.hpp"

using namespace Monarch;

const char* BatchRenderer2D::vertPath = "../../../Monarch/assets/shaders/vert.glsl";
const char* BatchRenderer2D::fragPath = "../../../Monarch/assets/shaders/frag.glsl";

unsigned int BatchRenderer2D::vao, BatchRenderer2D::vbo, BatchRenderer2D::ebo;
unsigned int BatchRenderer2D::blankTexture;

ShaderProgram* BatchRenderer2D::shader;

int BatchRenderer2D::numQuads;

BatchRenderer2D::Vertex* BatchRenderer2D::vertexBuffer;
BatchRenderer2D::Vertex* BatchRenderer2D::currentVertex;
    
unsigned int BatchRenderer2D::textures[BatchRenderer2D::MAX_TEXTURES];
int BatchRenderer2D::currentTexture;

glm::mat4 BatchRenderer2D::viewMatrix;
glm::mat4 BatchRenderer2D::projectionMatrix;

void BatchRenderer2D::init() {
    vertexBuffer = new Vertex[MAX_QUADS*4];

    shader = new ShaderProgram(vertPath, fragPath);

    short indices[BatchRenderer2D::MAX_QUADS*6];
    for(int i = 0; i < BatchRenderer2D::MAX_QUADS; i++){
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
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
    uint32_t color = 0xff0000ff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    textures[0] = blankTexture;
    currentTexture = 1;

}

void BatchRenderer2D::destroy(){
    delete shader;
    delete[] vertexBuffer;

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1,&blankTexture);
}

void BatchRenderer2D::beginBatch(){
    numQuads = 0;
    currentTexture = 1;
    currentVertex = vertexBuffer;
}
void BatchRenderer2D::endBatch(){
    GLsizeiptr size = (uint8_t*)currentVertex - (uint8_t*)vertexBuffer;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexBuffer);
}

void BatchRenderer2D::drawQuad(QuadData& data){
    if(numQuads == BatchRenderer2D::MAX_QUADS){
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

int BatchRenderer2D::useTexture(unsigned int tex){
    for (int i = 0; i < MAX_TEXTURES; i++)
        if (textures[i] == tex) return i;
    
    if (currentTexture == MAX_TEXTURES) {
        endBatch();
        flush();
        beginBatch();
    }

    textures[currentTexture] = tex;
    return currentTexture++;

}

void BatchRenderer2D::flush(){
    glBindVertexArray(vao);
    shader->start();
    int samplers[MAX_TEXTURES];
    for (int i = 0; i < MAX_TEXTURES; i++) {
        samplers[i] = i;
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    shader->setUniformIntArray(samplers,MAX_TEXTURES,"textures");
    shader->setUniformMat4(viewMatrix, "viewMatrix");
    shader->setUniformMat4(projectionMatrix, "projectionMatrix");

    glDrawElements(GL_TRIANGLES, numQuads*6, GL_UNSIGNED_SHORT, 0);
}
