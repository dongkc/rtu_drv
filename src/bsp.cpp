#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>                               // for memcpy() and memset()
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////////
namespace Zebra {

Q_DEFINE_THIS_FILE

// Local objects -------------------------------------------------------------
#ifdef Q_SPY
    enum {
        PHILO_STAT = QP::QS_USER
    };
    static uint8_t const l_clock_tick = 0U;
#endif

//............................................................................
void BSP_init(void) {

    Q_ALLEGE(QS_INIT((void *)0));
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_clock_tick);      // must be called *after* QF_init()
    QS_USR_DICTIONARY(PHILO_STAT);
}
//............................................................................
void BSP_terminate(int16_t result) {
    (void)result;
    QP::QF::stop();
}

} // namespace Zebra

//////////////////////////////////////////////////////////////////////////////

namespace QP {

//............................................................................
void QF::onStartup(void) {                             // startup callback

    QF_setTickRate(Zebra::BSP_TICKS_PER_SEC);       // set the desired tick rate
}
//............................................................................
void QF::onCleanup(void) {                             // cleanup callback
    QS_EXIT();                                       // perfomr the QS cleanup
}
//............................................................................
void QF_onClockTick(void) {

    QF::TICK(&Zebra::l_clock_tick);      // perform the QF clock tick processing
}

//............................................................................
extern "C" void Q_onAssert(char const Q_ROM * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    Zebra::BSP_terminate(-1);
}

//----------------------------------------------------------------------------*/
#ifdef Q_SPY                                            // define QS callbacks

#include "qspy.h"

static uint8_t l_running;

//............................................................................
bool QS::onStartup(void const *arg) {
    static uint8_t qsBuf[4*1024];                 // 4K buffer for Quantum Spy
    initBuf(qsBuf, sizeof(qsBuf));

    QSPY_config((QP_VERSION >> 8),  // version
                QS_OBJ_PTR_SIZE,    // objPtrSize
                QS_FUN_PTR_SIZE,    // funPtrSize
                QS_TIME_SIZE,       // tstampSize
                Q_SIGNAL_SIZE,      // sigSize,
                QF_EVENT_SIZ_SIZE,  // evtSize
                QF_EQUEUE_CTR_SIZE, // queueCtrSize
                QF_MPOOL_CTR_SIZE,  // poolCtrSize
                QF_MPOOL_SIZ_SIZE,  // poolBlkSize
                QF_TIMEEVT_CTR_SIZE,// tevtCtrSize
                (void *)0,          // matFile,
                (void *)0,
                (QSPY_CustParseFun)0); // customized parser function

    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);
//    QS_FILTER_OFF(QS_QEP_DISPATCH);
//    QS_FILTER_OFF(QS_QEP_UNHANDLED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
    QS_FILTER_OFF(QS_QF_MPOOL_GET);
    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
    QS_FILTER_OFF(QS_QF_PUBLISH);
    QS_FILTER_OFF(QS_QF_NEW);
    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
    QS_FILTER_OFF(QS_QF_GC);
    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    pthread_attr_t attr;
    struct sched_param param;
    pthread_t idle;

    return true;
}
//............................................................................
void QS::onCleanup(void) {
    l_running = (uint8_t)0;
    QSPY_stop();
}
//............................................................................
void QS::onFlush(void) {
    uint16_t nBytes = 1024U;
    uint8_t const *block;
    QF_CRIT_ENTRY(dummy);
    while ((block = getBlock(&nBytes)) != (uint8_t *)0) {
        QF_CRIT_EXIT(dummy);
        QSPY_parse(block, nBytes);
        nBytes = 1024U;
    }
}
//............................................................................
QSTimeCtr QS::onGetTime(void) {
    return (QSTimeCtr)clock();                                   // see NOTE01
}
//............................................................................
void QSPY_onPrintLn(void) {
    fputs(QSPY_line, stdout);
    fputc('\n', stdout);
}

//////////////////////////////////////////////////////////////////////////////
// NOTE01:
// clock() is the most portable facility, but might not provide the desired
// granularity. Other, less-portable alternatives are clock_gettime(),
// rdtsc(), or gettimeofday().
//

#endif                                                                // Q_SPY
//----------------------------------------------------------------------------

} // namespace QP

