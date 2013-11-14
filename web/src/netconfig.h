#ifndef netconfig_INCLUDED
#define netconfig_INCLUDED


#include <Poco/Net/HTTPRequestHandler.h>


class NetconfigHandler: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // netconfig_INCLUDED
