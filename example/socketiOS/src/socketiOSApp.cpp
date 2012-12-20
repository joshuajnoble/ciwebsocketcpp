#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "chat_client_handler.hpp"

using namespace ci;
using namespace ci::app;

using namespace websocketchat;

class socketiOSApp : public AppCocoaTouch {
  public:
	virtual void	setup();
	virtual void	resize( int width, int height );
	virtual void	update();
	virtual void	draw();
	virtual void	mouseDown( ci::app::MouseEvent event );
		
	ci::Matrix44f	mCubeRotation;
	ci::gl::Texture mTex;
	ci::CameraPersp	mCam;
};

void socketiOSApp::setup()
{
	mCubeRotation.setToIdentity();

	// Create a blue-green gradient as an OpenGL texture
	Surface8u surface( 256, 256, false );
	Surface8u::Iter iter = surface.getIter();
	while( iter.line() ) {
		while( iter.pixel() ) {
			iter.r() = 0;
			iter.g() = iter.x();
			iter.b() = iter.y();
		}
	}
	
	mTex = gl::Texture( surface );
    
    //////////////////////////////////////////////////////
    chat_client_handler_ptr handler(new chat_client_handler());
    client endpoint(handler);
    client::connection_ptr con;
    
    endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
    endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
    
    endpoint.elog().set_level(websocketpp::log::elevel::RERROR);
    endpoint.elog().set_level(websocketpp::log::elevel::FATAL);
    
    con = endpoint.get_connection("10.118.73.111:3000");
    
    con->add_request_header("User-Agent","WebSocket++/0.2.0 WebSocket++Chat/0.2.0");
    con->add_subprotocol("com.zaphoyd.websocketpp.chat");
    
    con->set_origin("http://zaphoyd.com");
    
    endpoint.connect(con);
    
    boost::thread t(boost::bind(&client::run, &endpoint, false));
}

void socketiOSApp::resize( int width, int height )
{
	mCam.lookAt( Vec3f( 3, 2, -3 ), Vec3f::zero() );
	mCam.setPerspective( 60, width / (float)height, 1, 1000 );
}

void socketiOSApp::mouseDown( MouseEvent event )
{
	std::cout << "Mouse down @ " << event.getPos() << std::endl;
}

void socketiOSApp::update()
{
	mCubeRotation.rotate( Vec3f( 1, 1, 1 ), 0.03f );
}

void socketiOSApp::draw()
{
	gl::clear( Color( 0.2f, 0.2f, 0.3f ) );
	gl::enableDepthRead();
	
	mTex.bind();
	gl::setMatrices( mCam );
	glPushMatrix();
		gl::multModelView( mCubeRotation );
		gl::drawCube( Vec3f::zero(), Vec3f( 2.0f, 2.0f, 2.0f ) );
	glPopMatrix();
}

CINDER_APP_COCOA_TOUCH( socketiOSApp, RendererGl )
