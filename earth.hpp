#ifndef EARTH_HPP
#define EARTH_HPP

#include "engine.hpp"
#include "config.hpp"
#include <vector>
using glm::vec2;
using glm::vec3;
using std::vector;

class Earth {
    public:
        void initialize(Engine *engine, int slices, int stacks, float spherical);
        float isSpherical();
        void setSpherical(float spherical);
        vec3 getPosition(float latitude, float longitude);
        vec3 getNormal(float latitude, float longitude);
        void draw(bool textured);
    protected:
        int slices, stacks;
        int nVertices, nTriangles;
        float spherical;
        Engine *engine;

        // TODO: Define the necessary buffers and texture.
        // Feel free to add helper methods to update the buffers.

        VertexBuffer vertexBuffer, normalBuffer, texCoordBuffer;
        ElementBuffer indexBuffer;
        Texture texture;

};

inline void Earth::initialize(Engine *e, int sl, int st, float sp) {
    engine = e;
    slices = sl;
    stacks = st;
    spherical = sp;

    // TODO: Initialize nVertices, nTriangles, buffers, texture
    nVertices = (slices + 1) * (stacks + 1);
    nTriangles = 2 * slices * stacks;

    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec2> text;
    for (int r = 0; r < stacks + 1; r++) {
        for (int c = 0; c < slices + 1; c++) {
            float latitude = 90 - 180.0 * r / stacks;
            float longitude = -180 + 360.0 * c / slices;
            vertices.push_back(getPosition(latitude, longitude));
            normals.push_back(getNormal(latitude, longitude));
            text.push_back(vec2(c / static_cast<float>(slices), r / static_cast<float>(stacks)));
        }
    }

    vector<int> indices;
    for (int r = 0; r < stacks; r++) {
        for (int c = 0; c < slices; c++) {
            // triangle 1
            indices.push_back(r * (slices+1) + c);
            indices.push_back((r+1) * (slices+1) + c);
            indices.push_back((r+1) * (slices+1) + c + 1);
            // triangle 2
            indices.push_back(r * (slices+1) + c);
            indices.push_back((r+1) * (slices+1) + c + 1);
            indices.push_back(r * (slices+1) + c + 1);
        }
    }

    texture = engine->loadTexture(Config::textureFile);

    vertexBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    normalBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    texCoordBuffer = engine->allocateElementBuffer(nVertices*sizeof(vec2));
    engine->copyVertexData(vertexBuffer, &vertices[0], nVertices*sizeof(vec3));
    engine->copyVertexData(normalBuffer, &normals[0], nVertices*sizeof(vec3));
    engine->copyElementData(texCoordBuffer, &text[0], nVertices*sizeof(vec2));

    int nIndices = indices.size();
    indexBuffer = engine->allocateElementBuffer(nIndices*sizeof(int));
    engine->copyElementData(indexBuffer, &indices[0], nIndices*sizeof(int));

}

inline float Earth::isSpherical() {
    return spherical;
}

inline void Earth::setSpherical(float s) {
    spherical = s;

    // TODO: Also update the relevant buffers.
    // You should only need to update two buffers, not all of them.
    vector<vec3> vertices;
    vector<vec3> normals;
    for (int r = 0; r < stacks + 1; r++) {
        for (int c = 0; c < slices + 1; c++) {
            float latitude = 90 - 180.0 * r / stacks;
            float longitude = -180 + 360.0 * c / slices;
            vertices.push_back(getPosition(latitude, longitude));
            normals.push_back(getNormal(latitude, longitude));
        }
    }

    engine->copyVertexData(vertexBuffer, &vertices[0], nVertices*sizeof(vec3));
    engine->copyVertexData(normalBuffer, &normals[0], nVertices*sizeof(vec3));
}

inline vec3 Earth::getPosition(float latitude, float longitude) {
    vec3 rectangularPosition(0,0,0), sphericalPosition(0,0,0);

    // TODO compute vertex positions on rectangle and sphere
    float rlat = M_PI/2 * latitude / 90.0;
    float rlong = M_PI * longitude / 180.0;
    rectangularPosition.x = rlong;
    rectangularPosition.y = rlat;
    sphericalPosition.x = cos(rlat)*sin(rlong);
    sphericalPosition.y = sin(rlat);
    sphericalPosition.z = cos(rlat)*cos(rlong);

    if (spherical == 0)
        return rectangularPosition;
    else if (spherical == 1)
        return sphericalPosition;
    else {
        float s = spherical;
        float c = 1.0 - s;
        vec3 inter = s * sphericalPosition +
                     c * rectangularPosition;
        return inter;

    }
}

inline vec3 Earth::getNormal(float latitude, float longitude) {
    vec3 rectangularNormal(0,0,1), sphericalNormal(0,0,0);

    // TODO compute vertex positions on rectangle and sphere
    sphericalNormal = glm::normalize(getPosition(latitude, longitude));

    if (spherical == 0)
        return rectangularNormal;
    else if (spherical == 1)
        return sphericalNormal;
    else {
        float s = spherical;
        float c = 1.0 - s;
        vec3 inter = s * sphericalNormal +
                     c * rectangularNormal;
        return inter;

    }
}

inline void Earth::draw(bool textured) {
    if (!textured) {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        engine->setVertexArray(vertexBuffer);
        engine->setNormalArray(normalBuffer);
        engine->drawElements(GL_TRIANGLES, indexBuffer, nTriangles * 3);
    } else {
        // texture
        engine->setVertexArray(vertexBuffer);
        engine->setNormalArray(normalBuffer);
        engine->setTexture(texture);
        engine->setTexCoordArray(texCoordBuffer);
        engine->drawElements(GL_TRIANGLES, indexBuffer, nTriangles * 3);
        engine->unsetTexture();

    }
}

#endif
