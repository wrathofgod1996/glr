#ifndef ICAMERA_H_
#define ICAMERA_H_

#include "ISceneNode.hpp"

namespace glr
{
	
class ICamera : public virtual ISceneNode 
{
public:
	virtual ~ICamera()
	{
	}
	;

	virtual bool isActive() const = 0;

	virtual void move(const glm::vec3& moveVector) = 0;

	virtual void tick(glm::detail::float32 time) = 0;

	virtual const glm::mat4& getViewMatrix() const = 0;
};

}

#endif /* ICAMERA_H_ */
