

#ifndef _PhongMaterial_
#define _PhongMaterial_


#include "MaterialGL.h"

class PhongMaterial : public MaterialGL
{
public:
	glm::vec3 breakdance;
	bool willBreakdance;
	bool willFlip;

	PhongMaterial(string name = "");
	~PhongMaterial();
	virtual void render(Node* o);
	virtual void animate(Node* o, const float elapsedTime);
	void displayInterface(Node* o);
	void update();

protected:
	GLProgram* vp;
	GLProgram* fp;

	GLuint l_View, l_Proj, l_Model, l_Time, l_lightPosition, l_cameraPosition;

	glm::vec3 ambientReflectionColor, diffuseReflectionColor, lightColor;
    GLfloat ambientReflectionCoefficient, diffuseReflectionCoefficient, specularReflectionCoefficient;
    GLuint l_AmbientReflectionColor, l_DiffuseReflectionColor, l_LightColor, l_AmbientReflectionCoefficient,
		l_DiffuseReflectionCoefficient, l_SpecularReflectionCoefficient, l_ConeSize;
	GLint coneSize;
};

#endif