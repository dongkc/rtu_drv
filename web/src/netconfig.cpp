#include "netconfig.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"


#line 1 "/home/dongkc/workspace/rtu/web/html/netconfig.html"

    #include "Poco/Util/ServerApplication.h"


    using Poco::Util::ServerApplication;
    using Poco::Util::Application;
    using Poco::Net::NameValueCollection;


void NetconfigHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	Poco::Net::HTMLForm form(request, request.stream());
	std::ostream& responseStream = response.send();
	responseStream << "";
	responseStream << "\n";
	responseStream << "";
#line 9 "/home/dongkc/workspace/rtu/web/html/netconfig.html"


		Application& app = Application::instance();
		app.logger().information("URI: " + request.getURI());
	responseStream << "\n";
	responseStream << "<html>\n";
	responseStream << "<head>\n";
	responseStream << "<title>系统配置</title>\n";
	responseStream << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />\n";
	responseStream << "<meta content=\"Comsenz Inc.\" name=\"Copyright\" />\n";
	responseStream << "</head>\n";
	responseStream << "<body>\n";
	responseStream << "<div class=\"container\">\n";
	responseStream << "    <div class=\"row-fluid\">\n";
	responseStream << "        <div class=\"well span9\" align=\"center\">\n";
	responseStream << "            <H3>网络参数配置</H3>\n";
	responseStream << "            <FORM id=\"formNet\" method=\"post\"  action=\"/goform/NetConfig\">\n";
	responseStream << "                <TABLE class=\"msgtable\" order=\"0\" cellSpacing=\"0\">\n";
	responseStream << "                    <TBODY>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\">设备网络IP地址:</TD>\n";
	responseStream << "                        <TD><input name=\"ip\" type=\"text\" id=\"ip\" value=\"192.168.0.88\" maxlength=\"15\" /></TD></TR>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\">设备网络掩码:</TD>\n";
	responseStream << "                        <TD><input name=\"mask\" type=\"text\" id=\"mask\" value=\"255.255.255.0\" maxlength=\"15\" /></TD></TR>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\">设备网络网关:</TD>\n";
	responseStream << "                        <TD><input name=\"gateway\" type=\"text\" id=\"gateway\" value=\"192.168.0.1\" maxlength=\"15\" /></TD></TR>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\">设备主DNS服务器地址:</TD>\n";
	responseStream << "                        <TD><input name=\"dnss1\" type=\"text\" id=\"dnss1\" value=\"0.0.0.0\" maxlength=\"15\" /></TD></TR>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\">设备备DNS服务器地址:</TD>\n";
	responseStream << "                        <TD><input name=\"dnss2\" type=\"text\" id=\"dnss2\" value=\"0.0.0.0\" maxlength=\"15\" /></TD></TR>\n";
	responseStream << "                    <TR>\n";
	responseStream << "                        <TD align=\"right\"><INPUT class=\"btn\" name=\"Submit\" value=\"应用网络设置\" type=\"submit\"><INPUT class=\"btn\" name=\"Submit2\" value=\"重置\" type=\"reset\"></TD></TR></TBODY></TABLE></FORM>\n";
	responseStream << "        </div>\n";
	responseStream << "    </div>\n";
	responseStream << "</div>\n";
	responseStream << "\n";
	responseStream << "</body>\n";
	responseStream << "</html>\n";
	responseStream << "\n";
	responseStream << "";
#line 52 "/home/dongkc/workspace/rtu/web/html/netconfig.html"

		NameValueCollection::ConstIterator it = request.begin();
		NameValueCollection::ConstIterator end = request.end();
		if (!form.empty())
		{
			it = form.begin();
			end = form.end();
			for (; it != end; ++it)
			{
                app.logger().information(it->first + " : " + it->second);
			}
		}
		
	responseStream << "\n";
	responseStream << "";
}
