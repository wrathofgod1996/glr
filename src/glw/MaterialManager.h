/*
 * MaterialManager.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MATERIALMANAGER_H_
#define MATERIALMANAGER_H_

#include <memory>
#include <string>
#include <map>

#include <GL/glew.h>

#include "IOpenGlDevice.h"

#include "IMaterialManager.h"

#include "Material.h"

namespace glr {
namespace glw {
class MaterialManager : public IMaterialManager {
public:
	MaterialManager(IOpenGlDevice* openGlDevice);
	virtual ~MaterialManager();

	virtual Material* getMaterial(const std::string path);
	virtual Material* addMaterial(
		const std::string path,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		glm::vec4 emission,
		glm::detail::float32 shininess,
		glm::detail::float32 strength
	);
private:	
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Material> > materials_;
};
}
}
#endif /* MATERIALMANAGER_H_ */