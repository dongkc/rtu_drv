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

int SetIp(const string& method,
          const string& ip,
          const string& mask,
          const string& gateway)
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

    if (vec_path.empty()) {
        std::cout << "No avaible network connection" << std::endl;
        return -1;
    }

    cout << vec_path.size() << endl;
    cout << vec_path[0] << endl;

    Client client(bus, vec_path[0].c_str(), "net.connman");

    std::map< std::string, ::DBus::Variant > map = client.GetProperties();

    std::map<std::string, ::DBus::Variant> ipv4_config;
    ipv4_config["Method"] =  make_variant<std::string>(method);
    ipv4_config["Address"] = make_variant<std::string>(ip);
    ipv4_config["Netmask"] = make_variant<std::string>(mask);
    ipv4_config["Gateway"] = make_variant<std::string>(gateway);

    client.SetProperty("IPv4.Configuration", make_variant<std::map<std::string, ::DBus::Variant> >(ipv4_config));

    return 0;
}

void ip_config(vector<string> argv)
{
    if (argv.size() < 4) {
        cout << "Invalid argument" << endl;
        return;
    }

    string ip = argv[1];
    string netmask = argv[2];
    string gateway = argv[3];
    
    if (argv.size() > 4) {
        string method = argv[4];
    }

    cout << ip << endl;
    cout << netmask << endl;
    cout << gateway << endl;

    try {
        if (argv.size() > 4) {
            SetIp("dhcp", ip, netmask, gateway);
        } else {
            SetIp("manual", ip, netmask, gateway);
        }

    }
    catch (...)
    {
        cout << "set ip failed! please check your input cmd" << endl;
    }
}
