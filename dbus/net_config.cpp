#include <iostream>

#include "ip.h"
#include "net_interface.h"

using namespace std;

class Client : public net::connman::Service_proxy,
               public DBus::IntrospectableProxy,
               public DBus::ObjectProxy
{
    public:
        Client(DBus::Connection &connection, const char* path, const char* name)
            : DBus::ObjectProxy(connection, path, name)
        {
        }

        void PropertyChanged(const std::string& name, const ::DBus::Variant& value)
        {
        }
};

class Manager : public net::connman::Manager_proxy,
               public DBus::IntrospectableProxy,
               public DBus::ObjectProxy
{
    public:
        Manager(DBus::Connection &connection, const char* path, const char* name)
            : DBus::ObjectProxy(connection, path, name)
        {
        }

        void PropertyChanged(const std::string& name, const ::DBus::Variant& value) {
        }
        void TechnologyAdded(const ::DBus::Path& path, const std::map< std::string, ::DBus::Variant >& properties)
        {
        }

        void TechnologyRemoved(const ::DBus::Path& path)
        {
        }
        void ServicesChanged(const std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > >& changed, const std::vector< ::DBus::Path >& removed)
        {
        }
};

DBus::BusDispatcher dispatcher;

template <typename T>
::DBus::Variant make_variant(T str)
{
    DBus::Variant var;
    DBus::MessageIter i = var.writer();

    i << str;

    return var;
}

int setDNS(const vector<string>& dns)
{
    DBus::default_dispatcher = & dispatcher;
    DBus::Connection bus = DBus::Connection::SystemBus();

    Manager manager(bus, "/", "net.connman");
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > > vec = manager.GetServices();
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > >::const_iterator it;

    std::vector<DBus::Path> vec_path;
    for(it = vec.begin(); it != vec.end(); ++it) {
        vec_path.push_back(it->_1);
    } 

    Client client(bus, vec_path[0].c_str(), "net.connman");

    client.SetProperty("Nameservers.Configuration", make_variant<std::vector<string> >(dns));

    return 0;
}

int getDNS(vector<string>& dns)
{
    DBus::default_dispatcher = & dispatcher;
    DBus::Connection bus = DBus::Connection::SystemBus();

    Manager manager(bus, "/", "net.connman");
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > > vec = manager.GetServices();
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > >::const_iterator it;
    std::vector<DBus::Path> vec_path;
    
    for(it = vec.begin(); it != vec.end(); ++it) {
        vec_path.push_back(it->_1);
    } 

    Client client(bus, vec_path[0].c_str(), "net.connman");

    std::map< std::string, ::DBus::Variant > properties = client.GetProperties();
    ::DBus::Variant dns_property = properties["Nameservers"];

    vector<string> tmp = dns_property;
    dns = tmp;

    return 0;
}

int setIP(string method, string ip, string mask, string gateway)
{
    DBus::default_dispatcher = & dispatcher;
    DBus::Connection bus = DBus::Connection::SystemBus();

    Manager manager(bus, "/", "net.connman");
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > > vec = manager.GetServices();
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > >::const_iterator it;

    std::vector<DBus::Path> vec_path;
    for(it = vec.begin(); it != vec.end(); ++it) {
        vec_path.push_back(it->_1);
    } 

    Client client(bus, vec_path[0].c_str(), "net.connman");

    std::map< std::string, ::DBus::Variant > properties = client.GetProperties();
    std::map<std::string, ::DBus::Variant> ipv4_property = properties["IPv4"];

    std::map<std::string, ::DBus::Variant> ipv4_config;
    ipv4_config["Method"] =  make_variant<std::string>(method);
    ipv4_config["Address"] = make_variant<std::string>(ip);
    ipv4_config["Netmask"] = make_variant<std::string>(mask);
    ipv4_config["Gateway"] = make_variant<std::string>(gateway);

    client.SetProperty("IPv4.Configuration", make_variant<std::map<std::string, ::DBus::Variant> >(ipv4_config));

    return 0;
}

int getIP(std::map<std::string, std::string>& ipv4)
{
    DBus::default_dispatcher = & dispatcher;
    DBus::Connection bus = DBus::Connection::SystemBus();

    Manager manager(bus, "/", "net.connman");
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > > vec = manager.GetServices();
    std::vector< ::DBus::Struct< ::DBus::Path, std::map< std::string, ::DBus::Variant > > >::const_iterator it;
    std::vector<DBus::Path> vec_path;
    
    for(it = vec.begin(); it != vec.end(); ++it) {
        vec_path.push_back(it->_1);
    } 

    Client client(bus, vec_path[0].c_str(), "net.connman");

    std::map< std::string, ::DBus::Variant > properties = client.GetProperties();
    std::map<std::string, ::DBus::Variant> ipv4_property = properties["IPv4"];

    string method = ipv4_property["Method"];
    ipv4["Method"] = method;
    string ip = ipv4_property["Address"];
    ipv4["Address"] = ip;
    string mask = ipv4_property["Netmask"];
    ipv4["Netmask"] = mask;
    string gateway = ipv4_property["Gateway"];
    ipv4["Gateway"] = gateway;

    return 0;
}
