#pragma once

#include "Mesh.h"
#include "Common.h"
#include <memory>

class Sphere
{
public:
	Sphere();
	void initialise();

	void draw();
	void drawNormal();

	~Sphere(){}

private:
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Mesh> _normal;

	std::vector<Vertex> _meshVertices;
	std::vector<GLuint> _meshIndices;

	std::vector<Vertex> _normalVertices;
	std::vector<GLuint> _normalIndices;

	GLfloat pi = 3.14159265358979323846f;

	void createTetrahedron();
	void createIcosahedron();
	void subdivideToSphere(GLfloat radius);
	void setNormal();
};

