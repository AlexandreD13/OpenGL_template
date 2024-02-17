

#ifndef _RotationMaterial_
#define _RotationMaterial_


#include "MaterialGL.h"

class RotationMaterial : public MaterialGL
{
public:
	RotationMaterial(string name = "");
	~RotationMaterial();
	virtual void animate(Node* o, const float elapsedTime);
	void displayInterface(Node* o);

protected:
	glm::vec3 rotation;
	bool willRotate;
};

#endif