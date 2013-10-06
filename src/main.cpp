#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

namespace Zebra {

// Local-scope objects -------------------------------------------------------
static QP::QEvt const *modbusQueue[50000];
static QP::QSubscrList l_subscrSto[MAX_PUB_SIG];

// storage for event pools...
static QF_MPOOL_EL(TableEvt) l_smlPoolSto[100];            // small pool

//............................................................................
extern "C" int main(int argc, char *argv[]) {

    QP::QF::init();

    BSP_init();

    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(modbusQueue);

    QP::QF::psInit(l_subscrSto, Q_DIM(l_subscrSto));

    QP::QF::poolInit(l_smlPoolSto,
                     sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));


    AO_Modbus->start((uint8_t)(1),
                    modbusQueue, Q_DIM(modbusQueue),
                    (void *)0, 1024, (QP::QEvt *)0);

    return QP::QF::run();
}

}  // namespace Zebra
