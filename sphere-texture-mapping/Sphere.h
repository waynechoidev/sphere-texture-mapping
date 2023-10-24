#pragma once

#include "Mesh.h"
#include "Common.h"
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	void createSphere(GLfloat radius, GLuint numOfSlices, GLuint numOfStacks);
	void subdivideToSphere(GLfloat radius);
	void setNormal();
};

