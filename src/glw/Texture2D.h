#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"
#include "ITexture.h"

#include "../common/utilities/Macros.h"

namespace glr {
namespace glw {

namespace glmd = glm::detail;

class IOpenGlDevice;

class Texture2D {
public:
	Texture2D(IOpenGlDevice* openGlDevice, const std::string name);
	Texture2D(utilities::Image* image, IOpenGlDevice* openGlDevice, const std::string name);
	virtual ~Texture2D();

	void bind(GLuint texturePosition = 0);
	
	GLuint getBufferId();
	GLuint getBindPoint();
	
	/**
	 * If the image is of the same format, width, and height of the previously uploaded image, you
	 * call 'pushToVideoMemory()' after calling this method.
	 * 
	 * If the image is NOT of the same format, width, or height of the previously uploaded image, you
	 * need to call 'freeVideoMemory()', 'allocateVideoMemory()', and then 'pushToVideoMemory()' in order to
	 * get the image data uploaded to the video card properly.
	 * 
	 * @param image
	 */
	void setData(utilities::Image* image);
	
	utilities::Image* getData();
	void pushToVideoMemory();
	void pullFromVideoMemory();
	void freeLocalData();
	void freeVideoMemory();
	void allocateVideoMemory();

	GETSET(std::string, name_, Name)

private:
	std::string name_;
	IOpenGlDevice* openGlDevice_;
	GLuint bufferId_;
	GLuint bindPoint_;
	
	utilities::Image image_;
	GLint internalFormat_;
	
	//GLuint textureId_;

	void loadIntoVideoMemory();
};

}
}
#endif /* TEXTURE2D_H_ */