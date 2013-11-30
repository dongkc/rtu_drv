#ifndef bsp_h
#define bsp_h

namespace Zebra {

unsigned int const BSP_TICKS_PER_SEC = static_cast<unsigned int>(500);

void BSP_init(void);
void BSP_terminate(int16_t const result);

int qpRun(void);

}  // namespace Zebra

#endif                                                                // bsp_h
