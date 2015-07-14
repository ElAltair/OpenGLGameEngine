#ifndef LIGHT_H
#define LIGHT_H
#include <glm.hpp>
using glm::vec3;
class Light
{
public:
	Light();
	Light(vec3 _amb, vec3 _dif, vec3 _spec);
	Light(float amb1,float amb2,float amb3,float dif1, float dif2, float dif3, float spec1, float spec2, float spec3);
	void setPosition(vec3 pos);
	void setAmbient(vec3 amb);
	void setDiffuse(vec3 dif);
	void setSpecular(vec3 spec);
	vec3 getAmbient(void);
	vec3 getDiffuse(void);
	vec3 getSpecular(void);
	~Light();
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

class DirectionalLight :public Light
{
public:
	DirectionalLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _direction);
	vec3 getDirection();

private:
	vec3 direction;

};

#endif
