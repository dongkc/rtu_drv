/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** sss.h - 
 **                                                                         **
 ** 03/04/2014 09:51:08 AM                                                  **
 **                                                                         **
 ** Copyright (c) 2014, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __SSS_HEADER__
#define __SSS_HEADER__
#include <iconv.h> 
#include <iostream> 
#include <string.h>

class CodeConverter { 
public: 
    CodeConverter(const char *from_charset,const char *to_charset) { 
        cd = iconv_open(to_charset,from_charset); 
    } 

    ~CodeConverter() { 
        iconv_close(cd); 
    } 

    int convert(char *inbuf, int inlen,char *outbuf, int outlen) {
        char **pin = &inbuf;
        char **pout = &outbuf;
        memset(outbuf,0,outlen);
        return iconv(cd, pin,(size_t *)&inlen,pout,(size_t *)&outlen);
    }

private: 
    iconv_t cd; 
};

#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/

