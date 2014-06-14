/* 
 * File:   main.cpp
 * Author: fede.tft
 *
 * Created on September 10, 2009, 10:50 AM
 */

#include <iostream>
#include "TimeoutSerial.h"
#include "sss.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
    try {
 
        TimeoutSerial serial("/dev/ttyUSB0",115200);
        serial.setTimeout(posix_time::seconds(20));

        //Text test
#if 0
        serial.writeString("AT+CREG?\r\n");
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;

        serial.writeString("AT+CMGF=1\r\n");

        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        serial.writeString("AT+CSCS=\"GB2312\"\r\n");
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        serial.writeString("AT+CNMI=2,1\r\n");
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
#endif
        serial.writeString("AT+CMGS=\"13488845771\"\r\n");
        sleep(1);
        char out[100];
        //CodeConverter cc = CodeConverter("utf-8","gb2312");
        CodeConverter cc = CodeConverter("unicode","gb2312");
        string in_utf8("何伟强是个傻蛋-----的哦哦那公开恶臭吭人民 test");
        cc.convert((char*)in_utf8.data(),in_utf8.size(),out,100);
        serial.write((const char*)&out, in_utf8.size());
        char del = 0x1A;
        serial.write(&del, 1);
#if 0
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
        cout<<serial.readStringUntil("\r\n")<<endl;
#endif
#if 0
#endif

        serial.close();
  
    } catch(boost::system::system_error& e)
    {
        cout<<"Error: "<<e.what()<<endl;
        return 1;
    }
}

