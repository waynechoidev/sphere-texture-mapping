#include "Sphere.h"

Sphere::Sphere()
{
    _mesh = std::make_shared<Mesh>();
    _normal = std::make_shared<Mesh>();
}

void Sphere::initialise()
{
    createTetrahedron();

    subdivideToSphere(1.5f);
    subdivideToSphere(1.5f);
    subdivideToSphere(1.5f);
    subdivideToSphere(1.5f);

    setNormal();

    _mesh->initialise(_meshVertices, _meshIndices);
    _normal->initialise(_normalVertices, _normalIndices);
}

void Sphere::createTetrahedron()
{
    const float a = 1.0f;
    const float x = sqrt(3.0f) / 3.0f * a;
    const float d = sqrt(3.0f) / 6.0f * a; // = x / 2
    const float h = sqrt(6.0f) / 3.0f * a;

    std::vector<glm::vec3> positions = { {0.0f, x, 0.0f},
                                      {-0.5f * a, -d, 0.0f},
                                      {+0.5f * a, -d, 0.0f},
                                      {0.0f, 0.0f, -h} };

    glm::vec3 center = glm::vec3(0.0f);

    for (int i = 0; i < 4; i++) {
        center += positions[i];
    }
    center /= 4.0f;

    for (int i = 0; i < 4; i++) {
        positions[i] -= center;
    }

    for (GLint i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = glm::normalize(v.position);
        // getting tex coord when subdivide
        _meshVertices.push_back(v);
    }

    _meshIndices = { 0, 1, 2, 3, 2, 1, 0, 3, 1, 0, 2, 3 };
}

void Sphere::subdivideToSphere(GLfloat radius)
{
    auto ProjectVertex = [&](Vertex& v) {
        v.normal = glm::normalize(v.position);
        v.position = v.normal * radius;

        const float theta = atan2f(v.position.z, v.position.x);
        const float phi = acosf(v.position.y / radius);
        v.texcoord.x = theta / (pi * 2);
        v.texcoord.y = phi / pi;
        };

    auto UpdateFaceNormal = [](Vertex& v0, Vertex& v1, Vertex& v2) {
        glm::vec3 v0_position(v0.position.x, v0.position.y, v0.position.z);
        glm::vec3 v1_position(v1.position.x, v1.position.y, v1.position.z);
        glm::vec3 v2_position(v2.position.x, v2.position.y, v2.position.z);

        glm::vec3 faceNormal = glm::normalize(glm::cross(v1_position - v0_position, v2_position - v0_position));

        v0.normal = faceNormal;
        v1.normal = faceNormal;
        v2.normal = faceNormal;
        };

    std::vector<Vertex> originalVertices = _meshVertices;
    std::vector<GLuint> originalIndices = _meshIndices;
    _meshVertices.clear();
    _meshIndices.clear();
    GLint count = 0;
    for (GLuint i = 0; i < originalIndices.size(); i += 3) {
        GLuint i0 = originalIndices[i];
        GLuint i1 = originalIndices[i + 1];
        GLuint i2 = originalIndices[i + 2];

        Vertex v0 = originalVertices[i0];
        Vertex v1 = originalVertices[i1];
        Vertex v2 = originalVertices[i2];

        ProjectVertex(v0);
        ProjectVertex(v1);
        ProjectVertex(v2);

        Vertex v3;
        v3.position = (v0.position + v2.position) * 0.5f;
        v3.texcoord = (v0.texcoord + v2.texcoord) * 0.5f;
        ProjectVertex(v3);

        Vertex v4;
        v4.position = (v0.position + v1.position) * 0.5f;
        v4.texcoord = (v0.texcoord + v1.texcoord) * 0.5f;
        ProjectVertex(v4);

        Vertex v5;
        v5.position = (v1.position + v2.position) * 0.5f;
        v5.texcoord = (v1.texcoord + v2.texcoord) * 0.5f;
        ProjectVertex(v5);
      
        _meshVertices.push_back(v4);
        _meshVertices.push_back(v1);
        _meshVertices.push_back(v5);

        _meshVertices.push_back(v0);
        _meshVertices.push_back(v4);
        _meshVertices.push_back(v3);

        _meshVertices.push_back(v3);
        _meshVertices.push_back(v4);
        _meshVertices.push_back(v5);

        _meshVertices.push_back(v3);
        _meshVertices.push_back(v5);
        _meshVertices.push_back(v2);

        for (GLuint j = 0; j < 12; j++) {
            _meshIndices.push_back(j + count);
        }
        count += 12;
    }
}

void Sphere::setNormal()
{
    for (GLint i = 0; i < _meshVertices.size(); i++) {
        Vertex n;
        n.position = _meshVertices[i].position;
        n.normal = _meshVertices[i].normal;
        n.texcoord.x = n.texcoord.y = 0.0f;
        _normalVertices.push_back(n);

        n.texcoord.x = 1.0f;
        _normalVertices.push_back(n);

        _normalIndices.push_back(GLuint(2 * i));
        _normalIndices.push_back(GLuint(2 * i + 1));
    }
}

void Sphere::draw()
{
    _mesh->render(GL_TRIANGLES);
}

void Sphere::drawNormal()
{
    _normal->render(GL_LINES);
}