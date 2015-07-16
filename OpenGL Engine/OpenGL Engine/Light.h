#ifndef LIGHT_H
#define LIGHT_H
#include <glm.hpp>
using glm::vec3;
class Light
{
public:
	Light();
	Light(const Light& light);
	Light(vec3 _amb, vec3 _dif, vec3 _spec);
	Light(float amb1,float amb2,float amb3,float dif1, float dif2, float dif3, float spec1, float spec2, float spec3);
	~Light();
	void setPosition(vec3 pos);
	void setAmbient(vec3 amb);
	void setDiffuse(vec3 dif);
	void setSpecular(vec3 spec);
	vec3 getAmbient(void) const;
	vec3 getDiffuse(void) const;
	vec3 getSpecular(void) const;
private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

class DirectionalLight :public Light
{
public:
	DirectionalLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _direction);
	DirectionalLight(const Light& light, vec3 _direction);
	~DirectionalLight();
	vec3 getDirection() const;

private:
	vec3 direction;

};


class PointLight : public Light
{
public: 
	PointLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _pos,float _constant, float _linear, float _quadratic);
	PointLight(const Light& light,vec3 _pos,float _constant, float _linear, float _quadratic);
	PointLight(const Light& light);
	PointLight(const PointLight& light);
	~PointLight();
	vec3 getPosition(void) const; 
	float getConstant(void) const;
	float getLinear(void) const;
	float getQuadratic(void) const;
	void setParamsForDistance(int distance);
	

private:
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

class FlashLight : public PointLight
{
public:
	FlashLight(vec3 _amb, vec3 _dif, vec3 _spec, vec3 _pos,float _constant, float _linear, float _quadratic,vec3 _direction, float _InnerCutoff, float _OuterCutoff);
	FlashLight(const PointLight& light,vec3 _direction, float _InnerCutoff, float _OuterCutoff);

private:
	vec3 direction;
	vec3 position;
	float InnerCutoff;
	float OuterCutoff;

};

#endif
