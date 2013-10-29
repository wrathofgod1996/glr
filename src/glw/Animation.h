/*
 * Animation.h
 *
 *  Created on: 2012-05-08
 *      Author: jarrett
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <map>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "IAnimation.h"
#include "IMesh.h"

#include "AnimatedBoneNode.h"

#include "IOpenGlDevice.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class Animation : public IAnimation {
public:
	Animation(IOpenGlDevice* openGlDevice);
	Animation(IOpenGlDevice* openGlDevice, const std::string name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes);
	virtual ~Animation();
	
	void bind();
	GLuint getBufferId();
	GLuint getBindPoint();
	
	void setAnimationTime(glmd::float32 runningTime);
	void setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame);
	
	const std::string getName();
	
	// TODO: make rootBoneNode const?
	// TODO: Should we really be sending in BoneData&?  Shouldn't we send IMesh* or something?
	void generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData, glmd::uint32 startFrame = 0, glmd::uint32 endFrame = 0);
	
	void generateIdentityBoneTransforms(glmd::uint32 numBones);

private:
	GLuint bufferId_;
	GLuint bindPoint_;

	std::string name_;
	glm::detail::float64 duration_;
	glm::detail::float64 ticksPerSecond_;
	
	// The running time can change multiple times per frame (models share animations)
	glmd::float32 runningTime_;
	
	// Only play frames within this range
	glmd::uint32 startFrame_;
	glmd::uint32 endFrame_;

	std::map< std::string, AnimatedBoneNode > animatedBoneNodes_;
	
	// The current transformation matrices generated by this animation from the information provided through calling the generateBoneTransforms(..) method
	std::vector< glm::mat4 > currentTransforms_;
	
	IOpenGlDevice* openGlDevice_;

	void setupAnimationUbo();
	void loadIntoVideoMemory();
	
	glmd::uint32 findPosition(float animationTime, AnimatedBoneNode* animatedBoneNode);
	glmd::uint32 findRotation(float animationTime, AnimatedBoneNode* animatedBoneNode);
	glmd::uint32 findScaling(float animationTime, AnimatedBoneNode* animatedBoneNode);

	void calcInterpolatedPosition(glm::vec3& Out, float animationTime, AnimatedBoneNode* animatedBoneNode);
	void calcInterpolatedRotation(glm::quat& Out, float animationTime, AnimatedBoneNode* animatedBoneNode);
	void calcInterpolatedScaling(glm::vec3& Out, float animationTime, AnimatedBoneNode* animatedBoneNode);
	void readNodeHeirarchy(glmd::float32 animationTime, glm::mat4& globalInverseTransform, BoneNode& rootBoneNode, BoneData& boneData, const glm::mat4& parentTransform);
};

}
}
#endif /* ANIMATION_H_ */
