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
    DataChannel(uint8_t id,
                uint8_t di_len,
                uint8_t do_len,
                uint8_t ai_len,
                uint8_t ao_len,
                uint8_t*  di_sink,
                uint8_t*  do_sink,
                uint16_t* ai_sink,
                uint16_t* ao_sink
                );
public:
    std::array<uint8_t,  128>  _di;
    std::array<uint8_t,  128>  _do;
    std::array<uint16_t, 128>  _ai;
    std::array<uint16_t, 128>  _ao;

    uint8_t  _di_len;
    uint8_t  _do_len;
    uint8_t  _ai_len;
    uint8_t  _ao_len;

    uint8_t*  _di_sink;
    uint8_t*  _do_sink;
    uint16_t* _ai_sink;
    uint16_t* _ao_sink;

    uint8_t _slaveID;
    uint8_t _status;

};

}
#endif
/****************************************************************************/
/**                                  EOF                                   **/
/****************************************************************************/
