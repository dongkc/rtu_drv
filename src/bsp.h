#ifndef bsp_h
#define bsp_h

namespace Zebra {

uint32_t const BSP_TICKS_PER_SEC = static_cast<uint32_t>(500);

void BSP_init(void);
void BSP_terminate(int16_t const result);

}                                                             // namespace Zebra

#endif                                                                // bsp_h
