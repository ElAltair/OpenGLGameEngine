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

Light::Light(const Light& light)
{
	ambient = light.getAmbient();
	diffuse = light.getDiffuse();
	specular = light.getSpecular();
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

vec3 Light::getAmbient(void) const
{
	return ambient;
}

vec3 Light::getDiffuse(void) const
{
	return diffuse;
}

vec3 Light::getSpecular(void) const
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

DirectionalLight::DirectionalLight(const Light& light, vec3 _direction) : Light(light)
{
	direction = _direction;
}

DirectionalLight::~DirectionalLight()
{

}

vec3 DirectionalLight::getDirection() const
{
	return direction;
}


PointLight::PointLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _pos, float _constant, float _linear, float _quadratic) :Light(_amb, _dif, _spec)
{
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

PointLight::PointLight(const Light& light, vec3 _pos, float _constant, float _linear, float _quadratic) :Light(light)
{
	position = _pos;
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

PointLight::PointLight(const Light& light) :Light(light)
{

}

PointLight::PointLight(const PointLight & light)
{
	position = light.getPosition();
	constant = light.getConstant();
	linear = light.getLinear();
	quadratic = light.getQuadratic();
}

vec3 PointLight::getPosition(void) const
{
	return position;
}

float PointLight::getConstant(void) const
{
	return constant;
}

float PointLight::getLinear(void) const
{
	return linear;
}

float PointLight::getQuadratic(void) const
{
	return quadratic;
}

void PointLight::setParamsForDistance(int distance)
{
	switch (distance)
	{
	case 7:
		constant = 1.0f;
		linear = 0.7f;
		quadratic = 1.8f;
		break;
	case 13:
		constant = 1.0f;
		linear = 0.35f;
		quadratic = 0.44f;
		break;
	case 20:
		constant = 1.0f;
		linear = 0.22f;
		quadratic = 0.2f;
		break;
	case 32:
		constant = 1.0f;
		linear = 0.14f;
		quadratic = 0.07f;
		break;
	case 50:
		constant = 1.0f;
		linear = 0.09;
		quadratic = 0.032f;
		break;
	case 65:
		constant = 1.0f;
		linear = 0.07f;
		quadratic = 0.017f;
		break;
	case 100:
		constant = 1.0f;
		linear = 0.045f;
		quadratic = 0.0075f;
		break;
	case 160:
		constant = 1.0f;
		linear = 0.027f;
		quadratic = 0.0028;
		break;
	case 200:
		constant = 1.0f;
		linear = 0.022f;
		quadratic = 0.0019f;
		break;
	case 325:
		constant = 1.0f;
		linear = 0.014f;
		quadratic = 0.0007f;
		break;
	case 600:
		constant = 1.0f;
		linear = 0.007f;
		quadratic = 0.0002f;
		break;
	case 3250:
		constant = 1.0f;
		linear = 0.0014f;
		quadratic = 0.000007f;
	default:
		constant = 1.0f;
		linear = 0.7f;
		quadratic = 1.8f;

		break;
	}
}

PointLight::~PointLight()
{

}