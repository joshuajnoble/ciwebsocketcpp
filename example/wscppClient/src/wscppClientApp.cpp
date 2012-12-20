#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "chat_client_handler.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

using boost::asio::ip::tcp;
using websocketpp::client;

using namespace websocketchat;

class wscppClientApp : public AppBasic {
  public:
    
    wscppClientApp();
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

wscppClientApp::wscppClientApp()
{
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

void wscppClientApp::setup()
{
}

void wscppClientApp::mouseDown( MouseEvent event )
{
}

void wscppClientApp::update()
{
}

void wscppClientApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( wscppClientApp, RendererGl )
