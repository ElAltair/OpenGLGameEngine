#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm.hpp>
using glm::vec3;

class Material
{
public:
	Material(vec3 amb,vec3 diffuse,vec3 spec,float shin);
	Material(float a1, float a2, float a3, float d1, float d2, float d3, float s1, float s2, float s3, float shin);
	void setDiffuse(vec3 diffuse);
	void setAmbient(vec3 ambient);
	void setSpecular(vec3 specular);
	void setDiffuse(float dif1,float dif2, float dif3);
	void setAmbient(float amb1, float abm2, float amb3);
	void setSpecular(float spec1, float spec2, float spec3);
	void setShininess(float shin);
	vec3 getDiffuse(void);
	vec3 getAmbient(void);
	vec3 getSpecular(void);
	float getShininess(void);
	~Material();
	void truere(void);
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	
};

#endif