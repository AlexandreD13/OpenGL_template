
#include "RotationMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>


RotationMaterial::RotationMaterial(string name) :
	MaterialGL(name)
{
	rotation = { 0, 1, 0 };
	willRotate = true;
}

RotationMaterial::~RotationMaterial()
{

}

void RotationMaterial::animate(Node* o, const float elapsedTime)
{
	if (willRotate) {
		o->frame()->rotate(rotation, elapsedTime / 1000);
	}
}

void RotationMaterial::displayInterface(Node* o) {
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Node Rotation: ");
	ImGui::SameLine();
	ImGui::Checkbox("", &willRotate);

	ImGui::Spacing();
	ImGui::Separator();
}