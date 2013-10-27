#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <memory>
#include <exception>

#include "glm/glm.hpp"

#include "GlrInclude.h"

#include "../extras/FpsCamera.cpp"

BOOST_AUTO_TEST_SUITE(fpsCamera)

BOOST_AUTO_TEST_CASE(createFpsCamera)
{
	
    glr::GlrProgram* p = new glr::GlrProgram();
    
    p->createWindow();
    
    glr::ISceneManager* smgr = p->getSceneManager();
	
	std::shared_ptr< glr::extras::FpsCamera > camera = std::shared_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera("camera", p->getOpenGlDevice(), 0.020f) );
	camera->setPosition(0, 0, 0);
	
	glm::vec3 pos = camera->getPosition();
	BOOST_CHECK_EQUAL( pos.x, 0.0f );
	BOOST_CHECK_EQUAL( pos.y, 0.0f );
	BOOST_CHECK_EQUAL( pos.z, 0.0f );
	
	smgr->addCamera(camera);
       
    delete p;
    
}

BOOST_AUTO_TEST_SUITE_END()
