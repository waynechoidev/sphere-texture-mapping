#include "Gui.h"

void Gui::initialise(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.FontGlobalScale = 1.5f;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Gui::update(bool& useTexture, bool& wireFrame, bool& backFaceCull, bool& drawNormal,
	float& translation, float& scaling, float& rotation,
	float& viewPosition, float& yaw, float& pitch,
	bool& usePerspective, Material& material, Light& light) 
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Scene Control");

	// Print framerate
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	ImGui::Checkbox("Use Texture", &useTexture);
	ImGui::Checkbox("Wire Frame", &wireFrame);
	ImGui::Checkbox("Backface Curl", &backFaceCull);
	ImGui::Checkbox("Draw Normal", &drawNormal);
	
	ImGui::Text("Model");
	ImGui::SliderFloat3("Translation", &translation, -2.0f, 2.0f);
	ImGui::SliderFloat3("Rotation", &rotation, -3.14f, 3.14f);
	ImGui::SliderFloat3("Scaling", &scaling, 0.1f, 2.0f);

	ImGui::Text("View");
	ImGui::SliderFloat3("Camera Position", &viewPosition, -1.0f, 1.0f);
	ImGui::SliderFloat("Yaw", &yaw, -180.0f, 0.0f);
	ImGui::SliderFloat("Pitch", &pitch, -90.0f, 90.0f);

	ImGui::Text("Projection");
	if (ImGui::RadioButton("Perspective", usePerspective == true)) {
		usePerspective = true;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Orthographic", usePerspective == false)) {
		usePerspective = false;
	}

	ImGui::Text("Material");
	ImGui::SliderFloat("Diffuse", &material.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &material.specular, 0.0f, 1.0f);

	ImGui::Text("Light");
	ImGui::Checkbox("Use Blinn Phong", &light.useBlinnPhong);
	if (ImGui::RadioButton("Directional Light", light.isDirectional == 1))
	{
		light.isDirectional = 1;
		light.isPoint = 0;
		light.isSpot = 0;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Point Light", light.isPoint == 1))
	{
		light.isDirectional = 0;
		light.isPoint = 1;
		light.isSpot = 0;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Spot Light", light.isSpot == 1))
	{
		light.isDirectional = 0;
		light.isPoint = 0;
		light.isSpot = 1;
	}
	ImGui::SliderFloat3("Light Position", &light.position[0], -2.0f, 2.0f);
	ImGui::SliderFloat("Fall off start", &light.fallOffStart, 0.0f, 5.0f);
	ImGui::SliderFloat("Fall off end", &light.fallOffEnd, 0.0f, 10.0f);
	ImGui::SliderFloat("Spot power", &light.spotPower, 1.0f, 512.0f);

	ImGui::End();
}

void Gui::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
