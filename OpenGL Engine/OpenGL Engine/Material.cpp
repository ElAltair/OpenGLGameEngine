#include "Material.h"

Material::Material(vec3 amb, vec3 diff, vec3 spec, float shin)
{
	ambient = amb;
	diffuse = diff;
	specular = spec;
	shininess = shin;
}
Material::Material(float a1, float a2, float a3, float d1, float d2, float d3, float s1, float s2, float s3, float shin)
{
	vec3 ambientVec(a1, a2, a3);
	vec3 diffuseVec(d1, d2, d3);
	vec3 specVec(s1, s2, s3);
	ambient = ambientVec;
	diffuse = ambientVec;
	specular = ambientVec;

}
Material::~Material()
{


}
void Material::setDiffuse(vec3 _diffuse)
{
	diffuse = _diffuse;
}
void Material::setAmbient(vec3 _ambient)
{
	ambient = _ambient;
}
void Material::setSpecular(vec3 _specular)
{
	specular = _specular;
}
void Material::setDiffuse(float dif1, float dif2, float dif3)
{
	vec3 _diffuse(dif1, dif2, dif3);
	diffuse = _diffuse;
}
void Material ::setAmbient(float amb1, float amb2, float amb3)
{
	vec3 _ambient(amb1, amb2, amb3);
	diffuse = _ambient;
}
void Material::setSpecular(float spec1, float spec2, float spec3)
{
	vec3 _specular(spec1, spec2, spec3);
	specular = _specular;
}
void Material::setShininess(float shin)
{
	shininess = shin;
}
vec3 Material::getDiffuse(void)
{
	return diffuse;
}
vec3 Material::getAmbient(void)
{
	return ambient;
}
vec3 Material::getSpecular(void)
{
	return specular;
}
float Material::getShininess(void)
{
	return shininess;
}
