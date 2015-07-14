#include "Light.h"


Light::Light(vec3 _amb, vec3 _dif, vec3 _spec)
{
	ambient = _amb;
	diffuse = _dif;
	specular = _spec;

}
Light::Light()
{
	ambient = vec3(1.0f);
	diffuse = vec3(1.0f);
	specular = vec3(1.0f);
}
	
Light::Light(float amb1, float amb2, float amb3, float dif1, float dif2, float dif3, float spec1, float spec2, float spec3)
{
	vec3 _amb(amb1, amb2, amb3);
	vec3 _dif(dif1, dif2, dif3);
	vec3 _spec(spec1, spec2, spec3);
	ambient = _amb;
	diffuse = _dif;
	specular = _spec;

}
void Light::setAmbient(vec3 amb)
{
	ambient = amb;
}
void Light::setDiffuse(vec3 dif)
{
	diffuse = dif;
}
void Light::setSpecular(vec3 spec)
{
	specular = spec;
}
vec3 Light::getAmbient(void)
{
	return ambient;
}
vec3 Light::getDiffuse(void)
{
	return diffuse;
}
vec3 Light::getSpecular(void)
{
	return specular;
}

Light::~Light()
{
}

DirectionalLight::DirectionalLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _direction) :Light(_amb, _dif, _spec)
{
	direction = _direction;

}
vec3 DirectionalLight::getDirection()
{
	return direction;
}
