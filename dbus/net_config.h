
#ifndef NET_CONFIG_H
#define NET_CONFIG_H

#include <string>
#include <map>

void ip_config(std::vector<std::string> argv);
int setDNS(const std::vector<std::string>& dns);
int getDNS(std::vector<std::string>& dns);
int setIP(const std::string& method, const std::string& ip, const std::string& mask, const std::string& gateway);
int getIP(std::map<std::string, std::string>& ipv4);

#endif //NET_CONFIG_H
