#include <iostream>

#include "Window.h"
#include "Gui.h"
#include "Sphere.h"
#include "Program.h"
#include "Texture.h"
#include "Common.h"

Window mainWindow;
Gui gui;
Sphere sphere;
Program mainProgram;
Program normalProgram;
Texture texture;

int main()
{
	mainWindow = Window(1920, 1080);
	mainWindow.initialise();
	
	gui = Gui();
	gui.initialise(mainWindow.getWindow());
	
	sphere = Sphere();
	sphere.initialise();

	mainProgram = Program();
	mainProgram.createFromFiles("object.vert", "object.frag");
	mainProgram.genVertexBuffers();
	mainProgram.genFragmentBuffers();

	normalProgram = Program();
	normalProgram.createFromFiles("normal.vert", "normal.frag");
	normalProgram.genVertexBuffers();

	texture = Texture();
	texture.initialise("map.jpg");

	bool useTexture = true;
	bool wireFrame = false;
	bool backFaceCull = false;
	bool drawNormal = false;

	// Model
	glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaling = { 0.4f, 0.4f, 0.4f };
	glm::vec3 rotation = { 0.5f, 0.5f, 0.0f };

	// View
	glm::vec3 viewPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 viewUp = { 0.0f, 1.0f, 0.0f };
	float yaw = -90.0f;
	float pitch = 0.0f;

	// Projection
	float aspectRatio = (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight();
	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 orthographicProjection = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);
	bool usePerspective = true;

	// Material
	Material material;
	material.ambient = 0.1f;
	material.shininess = 1.0f;
	material.diffuse = 1.0f;
	material.specular = 1.0f;

	// Light
	Light light;
	light.position = { 0.0f, 0.0f, -1.0f };
	light.direction = { 0.0f, 0.0f, 1.0f };
	light.strength = 1.0;
	light.fallOffStart = 0.0f;
	light.fallOffEnd = 10.0f;
	light.spotPower = 1.0f;
	light.isDirectional = 1;
	light.isPoint = 0;
	light.isSpot = 0;
	light.useBlinnPhong = true;

	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		gui.update(useTexture, wireFrame, backFaceCull, drawNormal,
			translation.x, scaling.x, rotation.x,
			viewPosition.x, yaw, pitch,
			usePerspective, material, light);

		glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
		backFaceCull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

		mainWindow.clear(0.0f, 0.0f, 0.0f, 1.0f);

		// Model
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(translation.x,translation.y,translation.z-2.5f));
		model = glm::scale(model, scaling);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		
		// View
		glm::vec3 viewFront = glm::vec3(0.0f);
		viewFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		viewFront.y = sin(glm::radians(pitch));
		viewFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		viewFront = glm::normalize(viewFront);
		glm::mat4 view = glm::lookAt(viewPosition, viewPosition + viewFront, viewUp);

		// Projection
		glm::mat4 projection = usePerspective ? perspectiveProjection : orthographicProjection;

		texture.use();

		mainProgram.use();
		mainProgram.bindVertexBuffers(model, projection, view);
		mainProgram.bindFragmentBuffers(useTexture, viewPosition, material, light);
		sphere.draw();

		normalProgram.use();
		normalProgram.bindVertexBuffers(model, projection, view);
		if(drawNormal) sphere.drawNormal();
		
		gui.render();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
}

