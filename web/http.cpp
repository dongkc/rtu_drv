
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/Application.h>

#include "http.h"
#include "netconfig.h"


using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;


class RequestHandlerFactory: public HTTPRequestHandlerFactory {
public:
    RequestHandlerFactory(Http* http): _http(http) {
    }

    HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& request) {
        if (request.getURI() == "/")
            return new NetconfigHandler;
        return 0;
    }

private:
    Http* _http;
};

Http::Http(): _httpServer(0) {

}

Http::~Http() {

}

const char* Http::name() const {
    return "Http";
}

void Http::initialize(Util::Application& app)
{
    RequestHandlerFactory* factory = new RequestHandlerFactory(this);
    ServerSocket socket(8080);
    HTTPServerParams* params = new HTTPServerParams;
    _httpServer = new HTTPServer(factory, socket, params);
    _httpServer->start();
}

void Http::uninitialize()
{
    _httpServer->stop();
    delete _httpServer;
    _httpServer = 0;
}
