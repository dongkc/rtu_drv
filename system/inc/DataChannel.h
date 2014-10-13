/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** DataChannel.h - 
 **                                                                         **
 ** 12/03/2013 03:39:30 PM                                                  **
 **                                                                         **
 ** Copyright (c) 2011, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __DATACHANNEL_HEADER__
#define __DATACHANNEL_HEADER__
#include <array>
#include <stdint.h>

namespace Zebra {


class  DataChannel
    /// The datachannel collect data from client and store it to the specific
    /// destination
{
public:
    DataChannel(uint16_t type,
                uint16_t len,
                void*  source,
                uint8_t* sink,
                uint8_t* status);

public:
    uint16_t _type;
    uint16_t _len;
    void* _source;
    uint8_t* _sink;
    uint8_t* _status;
};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
