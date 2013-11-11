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

#include "../common/utilities/Macros.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class Animation : public IAnimation {
public:
	Animation(IOpenGlDevice* openGlDevice, const std::string name);
	Animation(IOpenGlDevice* openGlDevice, const std::string name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes);
	Animation(const Animation& other);
	virtual ~Animation();
	
	void bind();
	void bind(std::vector< glm::mat4 >& transformations);
	GLuint getBufferId();
	GLuint getBindPoint();
	
	/**
	 * Will set the animation time to runningTime.
	 */
	void setAnimationTime(glmd::float32 runningTime);
	/**
	 * Set the animation to run only between the given start and end frame.
	 * 
	 * Note: A value of 0 for the start and end frame will run through all frames of the animation.
	 * 
	 * endFrame must be larger than or equal to startFrame.  
	 * 
	 * A value below zero for the start or end frame is invalid.
	 */
	void setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame);
	
	// TODO: make rootBoneNode const?
	// TODO: Should we really be sending in BoneData&?  Shouldn't we send IMesh* or something?
	/**
	 * Will generate the transformation matrices to be used to animate the model with the given bone data.
	 * 
	 * @param globalInverseTransformation - Not sure what this is...
	 * @param rootBoneNode - Root node of the skeleton used for the current animation.
	 * @param boneData - Data about the bones we are generating the animation on.  The method will use some of the boneData variables as a cache.
	 * (TODO: Should I be using this as a cache?  We currently use finalTransformation...)
	 * @param indexCache - If passed in, represents a cache of previous rotation, position, and scaling node search indices.  The algorithm to find
	 * the correct indices for the animation will use the indexCache index values to determine where to begin the search for the next
	 * rotation, position, and scaling index.
	 */
	void generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData);
	void generateBoneTransforms(glm::mat4& globalInverseTransformation, BoneNode& rootBoneNode, BoneData& boneData, std::vector<glmd::uint32>& indexCache);
	
	void generateIdentityBoneTransforms(glmd::uint32 numBones);
	
	virtual const std::string& getName();
	
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
	
	// A temporary cache to hold previous rotation, position, and scaling node search indices
	std::vector<glmd::uint32> indexCache_;
	
	// The current transformation matrices generated by this animation from the information provided through calling the generateBoneTransforms(..) method
	std::vector< glm::mat4 > currentTransforms_;
	
	IOpenGlDevice* openGlDevice_;

	void setupAnimationUbo();
	void loadIntoVideoMemory();
	void loadIntoVideoMemory(std::vector< glm::mat4 >& transformations);
	
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
