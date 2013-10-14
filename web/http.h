#ifndef Web_Http_INCLUDED
#define Web_Http_INCLUDED

#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/Subsystem.h>


class Http: public Poco::Util::Subsystem {
    public:
        Http();
        ~Http();

        const char* name() const override;

    protected:
        void initialize(Poco::Util::Application& app) override;
        void uninitialize() override;

    private:
        Poco::Net::HTTPServer* _httpServer;
};
#endif  // Web_Http_INCLUDED
