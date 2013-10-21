/*
 * GuiComponent.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef GUICOMPONENT_H_
#define GUICOMPONENT_H_

#include <string>
#include <map>

#include <GL/glew.h>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>

#include "../../glw/shaders/IShaderProgram.h"
#include "../../glw/IOpenGlDevice.h"

#include "../IGuiComponent.h"

#include "GuiObject.h"

namespace glmd = glm::detail;

namespace glr {
namespace gui {
namespace cef {

class RenderHandler : public CefRenderHandler
{
public:
	RenderHandler(glm::detail::uint32 web_texture) : web_texture(web_texture)
	{
		// TODO: error check web_texture value
	}

	// CefRenderHandler interface
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
	{
		rect = CefRect(0, 0, 800, 600);
		return true;
	};
	
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {
		std::cout << "painting!: " << width << " " << height << std::endl;
		//memcpy(texBuf->getCurrentLock().data, buffer, width*height*4);
		
		glBindTexture(GL_TEXTURE_2D, web_texture);
		
		// Finally, we perform the main update, just copying the rect that is
		// marked as dirty but not from scrolled data.
		/*
		glTexSubImage2D(GL_TEXTURE_2D, 0,
						0, 0,
						width, height,
						GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, 
						buffer
						);
						*/
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

		glBindTexture(GL_TEXTURE_2D, 0);
	};
	
	glm::detail::uint32 web_texture;
    
    // CefBase interface
	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(RenderHandler)
};




class GuiComponent : public IGuiComponent, public IAddFunctionListener, public CefClient {
public:
	GuiComponent(glw::IOpenGlDevice* openGlDevice, glmd::uint32 width, glmd::uint32 height);
	virtual ~GuiComponent();

	virtual void load();
	virtual void unload();

	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount = 1);
	virtual void mouseClick(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll);

	virtual void textEvent(const wchar_t*evt, size_t evtLength);
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode);

	int setContents(std::string contents);
	int setContentsUrl(std::string url);

	virtual void update();
	virtual void render(shaders::IShaderProgram* shader);

	virtual void executeScript(std::wstring script);

	virtual bool isVisible();
	virtual void setVisible(bool isVisible);

	virtual IGuiObject* createGuiObject(std::wstring name);
	virtual IGuiObject* getGuiObject(std::wstring name);
	
	virtual void addedFunction(std::wstring func);
	
	// Implement functions for CefClient
	virtual bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

private:
	bool isVisible_;

	std::string url_;
	
	glw::IOpenGlDevice* openGlDevice_;

	// Width and height of our window.
	glmd::uint32 width_, height_;
	// Storage for a texture
	glm::detail::uint32 web_texture;
	// Buffer used to store data for scrolling
	char* scroll_buffer;

	// CEF3 variables
	CefRefPtr<CefBrowser> browser_;
    //CefRefPtr<BrowserClient> browserClient_;
    CefRefPtr<CefRenderHandler> renderHandler_;
    
    bool bindDataSent_;

	std::map< std::wstring, std::unique_ptr<GuiObject> > guiObjects_;

	std::wstring getFunctionName(std::wstring name);
	std::wstring getObjectName(std::wstring name);

	unsigned int mapGLUTCoordToTexCoord(unsigned int glut_coord, unsigned int glut_size, unsigned int tex_size);
	glm::detail::int32 getCefStateModifiers(glm::detail::int32 state);
	
	/**
	 * 
	 * @return The number of functions sent via IPC to the render process.
	 */
	int sendBoundFunctionsToRenderProcess();

	/*
	bool mapOnPaintToTexture(
		Berkelium::Window*wini,
		const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
		size_t num_copy_rects, const Berkelium::Rect*copy_rects,
		int dx, int dy,
		const Berkelium::Rect& scroll_rect,
		unsigned int dest_texture,
		unsigned int dest_texture_width,
		unsigned int dest_texture_height,
		bool ignore_partial,
		char* scroll_buffer);

	virtual void onPaint(Berkelium::Window* wini,
						 const unsigned char*bitmap_in, const Berkelium::Rect&bitmap_rect,
						 size_t num_copy_rects, const Berkelium::Rect* copy_rects,
						 int dx, int dy, const Berkelium::Rect& scroll_rect);

	virtual void onCrashed(Berkelium::Window*win);
	virtual void onUnresponsive(Berkelium::Window*win);
	virtual void onScriptAlert(Berkelium::Window*win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool&success, Berkelium::WideString&value);
	virtual void onJavascriptCallback(Berkelium::Window*win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, Berkelium::Script::Variant*args, size_t numArgs);
	*/
	void testLoadTexture();
	void testDrawTest1();
	
	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(GuiComponent)
};

}
}
}
#endif /* GUICOMPONENT_H_ */
