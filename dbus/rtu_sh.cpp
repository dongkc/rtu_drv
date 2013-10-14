#include <iostream> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h> 
#include <vector> 
#include <string> 
#include <unistd.h>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "net_config.h"

using namespace std; 

void cmd_dispatch(const vector<string>& argv)
{
    if (argv.empty()) {
        return;
    }

    if (argv[0] == string("net")) {
        system("/sbin/ifconfig");
    } else if (argv[0] == string("mem")) {
        system("/usr/sbin/free");
    } else if (argv[0] == string("ip")) {
        ip_config(argv);
    }
}

void handle_signal(int signo)
{
    cout << endl << "RTU -> ";
    cout.flush();
}
int main() 
{ 
    signal(SIGINT, SIG_IGN);
    signal(SIGINT, handle_signal);

    if (fork() == 0) {
        execv("/usr/bin/clear", NULL);
        exit(1);
    } else {
        wait(NULL);
    }

    cout << "RTU -> ";
    cout.flush();

    string line;
    
    while (getline(cin, line))
    { 
        vector<string> vec_argv;

        boost::char_separator<char> sep(" ");
        boost::tokenizer<boost::char_separator<char>> tokens(line, sep);
        BOOST_FOREACH( const string& t, tokens) {
//        for (const auto& t : tokens) {
            vec_argv.push_back(t);
        }

        cmd_dispatch(vec_argv);

        cout << "RTU -> ";
        cout.flush();
    } 

    return 0;
} 
