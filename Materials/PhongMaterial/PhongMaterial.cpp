
#include "PhongMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>


PhongMaterial::PhongMaterial(string name) :
	MaterialGL(name)
{
	vp = new GLProgram(MaterialPath + "PhongMaterial/Phong-VS.glsl", GL_VERTEX_SHADER);
	fp = new GLProgram(MaterialPath + "PhongMaterial/Phong-FS.glsl", GL_FRAGMENT_SHADER);

	m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
	m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

	l_View = glGetUniformLocation(vp->getId(), "View");
	l_Proj = glGetUniformLocation(vp->getId(), "Proj");
	l_Model = glGetUniformLocation(vp->getId(), "Model");

	l_lightPosition = glGetUniformLocation(vp->getId(), "LightPosition");
	l_cameraPosition = glGetUniformLocation(vp->getId(), "CameraPosition");

	l_AmbientReflectionColor = glGetUniformLocation(fp->getId(), "AmbientReflectionColor");
	l_DiffuseReflectionColor = glGetUniformLocation(fp->getId(), "DiffuseReflectionColor");
	l_LightColor = glGetUniformLocation(fp->getId(), "LightColor");

	l_AmbientReflectionCoefficient = glGetUniformLocation(fp->getId(), "AmbientReflectionCoefficient");
	l_DiffuseReflectionCoefficient = glGetUniformLocation(fp->getId(), "DiffuseReflectionCoefficient");
	l_SpecularReflectionCoefficient = glGetUniformLocation(fp->getId(), "SpecularReflectionCoefficient");

	l_ConeSize = glGetUniformLocation(fp->getId(), "ConeSize");

	ambientReflectionColor = glm::vec3(0.92, 0.77, 0.10);
	diffuseReflectionColor = glm::vec3(0.98, 1.00, 0.00);
	lightColor = glm::vec3(0.66, 0.63, 0.08);

	ambientReflectionCoefficient = 0.4;
	diffuseReflectionCoefficient = 0.25;
	specularReflectionCoefficient = 0.75;

	coneSize = 1;

	breakdance = { 0, 1, glm::tan(glm::sin(0.5))};
	willFlip = false;
	willBreakdance = false;

	update();
}

PhongMaterial::~PhongMaterial()
{

}

void PhongMaterial::render(Node* o)
{
	m_ProgramPipeline->bind();

	o->drawGeometry(GL_TRIANGLES);
	m_ProgramPipeline->release();
}

void PhongMaterial::animate(Node* o, const float elapsedTime)
{
	glProgramUniformMatrix4fv(vp->getId(), l_View, 1, GL_FALSE, glm::value_ptr(Scene::getInstance()->camera()->getViewMatrix()));
	glProgramUniformMatrix4fv(vp->getId(), l_Proj, 1, GL_FALSE, glm::value_ptr(Scene::getInstance()->camera()->getProjectionMatrix()));
	glProgramUniformMatrix4fv(vp->getId(), l_Model, 1, GL_FALSE, glm::value_ptr(o->frame()->getModelMatrix()));

	Frame* lumiere = Scene::getInstance()->getNode("Light")->frame();
	glm::vec3 positionLumiere = lumiere->convertPtTo(glm::vec3(0.0, 0.0, 0.0), o->frame());
	glProgramUniform3fv(vp->getId(), l_lightPosition, 1, glm::value_ptr(positionLumiere));

	Frame* camera = Scene::getInstance()->camera()->frame();
	glm::vec3 positionCamera = camera->convertPtTo(glm::vec3(0.0, 0.0, 0.0), o->frame());
	glProgramUniform3fv(vp->getId(), l_cameraPosition, 1, glm::value_ptr(positionCamera));

	if (willFlip) {
		o->frame()->scale(glm::vec3(1.0, -1.0, 1.0));
		willFlip = false;
	}

	if (willBreakdance) {
		o->frame()->rotate(breakdance, elapsedTime / 100);
	}
}

void PhongMaterial::update() {
	glProgramUniform3fv(fp->getId(), l_AmbientReflectionColor, 1, glm::value_ptr(ambientReflectionColor));
	glProgramUniform3fv(fp->getId(), l_DiffuseReflectionColor, 1, glm::value_ptr(diffuseReflectionColor));
	glProgramUniform3fv(fp->getId(), l_LightColor, 1, glm::value_ptr(lightColor));
	glProgramUniform1f(fp->getId(), l_AmbientReflectionCoefficient, ambientReflectionCoefficient);
	glProgramUniform1f(fp->getId(), l_DiffuseReflectionCoefficient, diffuseReflectionCoefficient);
	glProgramUniform1f(fp->getId(), l_SpecularReflectionCoefficient, specularReflectionCoefficient);
	glProgramUniform1f(fp->getId(), l_ConeSize, coneSize);
}

void PhongMaterial::displayInterface(Node* o) {
	ImGui::Separator();
	ImGui::Spacing();

	const char* AmbientColorLabel = "##Ambient Reflection Color";
	const char* DiffuseColorLabel = "##Diffuse Reflection Color";
	const char* LightColorLabel = "##Light Color";

	ImGui::Text("%s:", AmbientColorLabel + 2);
	ImGui::SameLine(0, 50);
	ImGui::Text("%s:", DiffuseColorLabel + 2);
	ImGui::SameLine(0, 50);
	ImGui::Text("%s:", LightColorLabel + 2);

	ImGui::PushItemWidth(100);

	if (ImGui::ColorEdit3(AmbientColorLabel, (float*)&ambientReflectionColor)) { update(); }
	ImGui::SameLine(0, 125);
	if (ImGui::ColorEdit3(DiffuseColorLabel, (float*)&diffuseReflectionColor)) { update(); }
	ImGui::SameLine(0, 125);
	if (ImGui::ColorEdit3(LightColorLabel, (float*)&lightColor)) { update(); }

	ImGui::PopItemWidth();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushItemWidth(200);

	ImGui::Text("Ambient Reflection Coefficient (ka):   ");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##Ambient Reflection Coefficient", &ambientReflectionCoefficient, 0.0f, 1.0f)) { update(); }

	ImGui::Text("Diffuse Reflection Coefficient (kd):   ");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##Diffuse Reflection Coefficient", &diffuseReflectionCoefficient, 0.0f, 1.0f)) { update(); }

	ImGui::Text("Specular Reflection Coefficient (ks):  ");
	ImGui::SameLine();
	if (ImGui::SliderFloat("##Specular Reflection Coefficient", &specularReflectionCoefficient, 0.0f, 1.0f)) { update(); }

	ImGui::Text("Cone Size (s):                         ");
	ImGui::SameLine();
	if (ImGui::InputInt("##Cone Size", &coneSize)) { update(); }

	ImGui::PopItemWidth();

	if (o->getName() == "Bunny") {
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Breakdance Mode: ");
		ImGui::SameLine();
		if (ImGui::Checkbox("", &willBreakdance)) { willFlip = true; }
	}

	ImGui::Spacing();
	ImGui::Separator();
}