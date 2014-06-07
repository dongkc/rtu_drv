#include "DataChannel.h"

using namespace std;

namespace Zebra {

DataChannel::DataChannel(uint8_t id,
                         uint8_t di_len,
                         uint8_t do_len,
                         uint8_t ai_len,
                         uint8_t ao_len,
                         uint8_t*  di_sink,
                         uint8_t*  do_sink,
                         uint16_t* ai_sink,
                         uint16_t* ao_sink,
                         uint8_t* spi_sink)
    :_di_len(di_len),
     _do_len(do_len),
     _ai_len(ai_len),
     _ao_len(ao_len),
     _di_sink(di_sink),
     _do_sink(do_sink),
     _ai_sink(ai_sink),
     _ao_sink(ao_sink),
     _slaveID(id),
     _status(1),
     _spi_sink(spi_sink)
{
//
};

}
