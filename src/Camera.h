/*
 * Camera.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef GLR_CAMERA_H_
#define GLR_CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ICamera.h"
#include "BasicSceneNode.h"

//#include "IInputListener.h"

namespace glr {
class Camera : public virtual ICamera, public BasicSceneNode {
public:
	Camera(glw::IOpenGlDevice* openGlDevice);
	Camera(const std::string name, glw::IOpenGlDevice* openGlDevice);
	virtual ~Camera();

	// inherited from ICamera
	virtual bool isActive();

	virtual void render();

	virtual void attach(models::IModel* model);

	virtual void move(const glm::vec3& moveVector);
	
	virtual void lookAt(const glm::vec3& lookAt);

	virtual void tick(glm::detail::float32 time);

	virtual const glm::mat4& getViewMatrix();

private:
	glm::detail::int32 prevX_, prevY_;

	glm::mat4 viewMatrix_;
	glm::vec3 direction_;

	glm::vec3 movementBuffer_;
	glm::detail::float32 moveSpeed_, rotSpeed_;
	void clearMovementBuffer();
	glm::detail::float32 xRot_, yRot_;

	void initialize();
};
}
#endif /* GLR_CAMERA_H_ */
