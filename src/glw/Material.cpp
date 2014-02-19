/*
 * Material.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "../common/utilities/AssImpUtilities.h"

#include "../exceptions/GlException.h"

#include "Material.h"

namespace glr {
namespace glw {

Material::Material(IOpenGlDevice* openGlDevice, const std::string& name)
	: openGlDevice_(openGlDevice), name_(name), bufferId_(0)
{	
}

Material::Material(
		IOpenGlDevice* openGlDevice,
		const std::string& name,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
		)
	: openGlDevice_(openGlDevice), name_(name), ambient_(ambient), diffuse_(diffuse), specular_(specular), emission_(emission), shininess_(shininess), strength_(strength), bufferId_(0)
{
	LOG_DEBUG( "loading material..." );
	
	allocateVideoMemory();
	pushToVideoMemory();
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream ss;
		ss << "Error while loading material '" << name_ << "' in OpenGl: " << err.name;
		LOG_ERROR( ss.str() );
		throw exception::GlException( ss.str() );
	}
	else
	{
		std::stringstream ss;
		ss << "Successfully loaded material.  Buffer id: " << bufferId_;
		LOG_DEBUG( ss.str() );
	}
}

void Material::pushToVideoMemory()
{
	LOG_DEBUG( "loading material '" + name_ +"' into video memory." );
	
	MaterialData md = MaterialData();
	md.ambient = ambient_;
	md.diffuse = diffuse_;
	md.specular = specular_;
	md.emission = emission_;
	//md.shininess = shininess_;
	//md.strength = strength_;
	
	bufferId_ = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, sizeof(MaterialData), &md);
	
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{		
		std::string msg = std::string( "Error while push data for material '" + name_ + "' to video memory in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully pushed data for material '" + name_ + "' to video memory." );
	}
}

void Material::pullFromVideoMemory()
{
	// TODO: Implement
}

void Material::freeLocalData()
{
}

void Material::freeVideoMemory()
{
	if (bufferId_ == 0)
	{
		LOG_WARN( "Cannot free video memory - buffer does not exist for material." );
		return;
	}
	
	openGlDevice_->releaseBufferObject( bufferId_ );
	
	bufferId_ = 0;
}

void Material::allocateVideoMemory()
{
	// TODO: Implement
}

Material::~Material()
{
	freeVideoMemory();
}

void Material::bind()
{
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	//std::cout << "material: " << name_ << " | " << bufferId_ << " | " << bindPoint_ << std::endl;
	/*
	for (int i=0; i < 4; i++)
		std::cout << ambient_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << diffuse_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << specular_[i] << " ";
	std::cout << std::endl;
	for (int i=0; i < 4; i++)
		std::cout << emission_[i] << " ";
	
	std::cout << std::endl << std::endl;
	*/
}

GLuint Material::getBufferId()
{
	return bufferId_;
}

GLuint Material::getBindPoint()
{
	return bindPoint_;
}

void Material::setAmbient(const glm::vec4& ambient)
{
	ambient_ = ambient;
}

void Material::setDiffuse(const glm::vec4& diffuse)
{
	diffuse_ = diffuse;
}

void Material::setSpecular(const glm::vec4& specular)
{
	specular_ = specular;
}

void Material::setEmission(const glm::vec4& emission)
{
	emission_ = emission;
}

void Material::setShininess(glm::detail::float32 shininess)
{
	shininess_ = shininess;
}

void Material::setStrength(glm::detail::float32 strength)
{
	strength_ = strength;
}

}
}
