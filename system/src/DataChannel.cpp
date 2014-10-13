#include <stdio.h>
#include "DataChannel.h"

using namespace std;

namespace Zebra {

DataChannel::DataChannel(uint16_t type,
                         uint16_t len,
                         void*  source,
                         uint8_t* sink,
                         uint8_t* status)
    :_type(type),
     _len(len),
     _source(source),
     _sink(sink),
     _status(status)
{
};

}
