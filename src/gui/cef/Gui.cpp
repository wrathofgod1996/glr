#ifdef USE_CEF

#include <iostream>
#include <string.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "gui/cef/Gui.hpp"

#include "common/logger/Logger.hpp"

#include "glw/shaders/GlslShaderProgram.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

Gui::Gui(glw::IOpenGlDevice* openGlDevice, shaders::IShaderProgramManager* shaderProgramManager, glmd::uint32 width, glmd::uint32 height) : openGlDevice_(openGlDevice), shaderProgramManager_(shaderProgramManager), width_(width), height_(height)
{
}

Gui::~Gui()
{
	destroy();
}

void Gui::destroy()
{	
	views_.clear();
	
	CefShutdown();
}



void Gui::update()
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->executeScript(L"update();");
	}
}

void Gui::render()
{
	CefDoMessageLoopWork();
	
	//shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_gui");

	//shader->bind();
	
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->render(nullptr);
	}
	
	//shaders::GlslShaderProgram::unbindAll();
}

void Gui::mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseMoved(xPos, yPos);
		}
	}
}

void Gui::mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseButton(buttonId, xPos, yPos, down, clickCount);
		}
	}
}

void Gui::mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseWheel(xScroll, yScroll);
		}
	}
}

void Gui::textEvent(const wchar_t* evt, size_t evtLength)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->textEvent(evt, evtLength);
		}
	}
}

void Gui::keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->keyEvent(pressed, mods, vk_code, scancode);
		}
	}
}


/**
 * TESTING
 */
IGuiComponent* Gui::loadFromFile(std::string filename)
{
	auto comp = std::unique_ptr<GuiComponent>( new GuiComponent(openGlDevice_, width_, height_) );

	comp->setContentsUrl(std::move(filename));

	/*
	if ( comp->load() < 0 )
	{
		// TODO: throw error instead of returning a 'return status' from load() method
		LOG_DEBUG( << "Unable to load gui component.";
		comp->unload();
		return nullptr;
	}
	*/
	views_.push_back( std::move(comp) );

	return views_.back().get();
}

IGuiComponent* Gui::loadFromData(std::string data)
{
	auto comp = std::unique_ptr<GuiComponent>( new GuiComponent(openGlDevice_, width_, height_) );

	comp->setContents(std::move(data));
	/*
	if ( comp->load() < 0 )
	{
		// TODO: throw error instead of returning a 'return status' from load() method
		LOG_DEBUG( << "Unable to load gui component.";
		comp->unload();
		return 0;
	}
	*/
	views_.push_back( std::move(comp) );

	return views_.back().get();
}

void Gui::release(IGuiComponent* comp)
{
	for ( glm::detail::uint32 i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get() == comp )
		{
			views_.erase(views_.begin() + i);
		}
	}
}

void Gui::windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height)
{
	for ( auto& it : views_ )
	{
		it->windowSizeUpdate( width, height );
	}
}

}
}
}

#endif /* USE_CEF */
