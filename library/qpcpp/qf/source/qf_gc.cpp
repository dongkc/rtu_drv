//////////////////////////////////////////////////////////////////////////////
// Product: QF/C++
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Jul 02, 2012
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "qf_pkg.h"
#include "qassert.h"

/// \file
/// \ingroup qf
/// \brief QF::gc() implementation.

QP_BEGIN_

Q_DEFINE_THIS_MODULE("qf_gc")

//............................................................................
void QF::gc(QEvt const * const e) {
    if (QF_EVT_POOL_ID_(e) != u8_0) {                // is it a dynamic event?
        QF_CRIT_STAT_
        QF_CRIT_ENTRY_();

        if (QF_EVT_REF_CTR_(e) > u8_1) {     // isn't this the last reference?
            QF_EVT_REF_CTR_DEC_(e);               // decrement the ref counter

            QS_BEGIN_NOCRIT_(QS_QF_GC_ATTEMPT, null_void, null_void)
                QS_TIME_();                                       // timestamp
                QS_SIG_(e->sig);                    // the signal of the event
                QS_U8_(QF_EVT_POOL_ID_(e));        // the pool Id of the event
                QS_U8_(QF_EVT_REF_CTR_(e));      // the ref count of the event
            QS_END_NOCRIT_()

            QF_CRIT_EXIT_();
        }
        else {         // this is the last reference to this event, recycle it
            uint8_t idx = static_cast<uint8_t>(QF_EVT_POOL_ID_(e) - u8_1);

            QS_BEGIN_NOCRIT_(QS_QF_GC, null_void, null_void)
                QS_TIME_();                                       // timestamp
                QS_SIG_(e->sig);                    // the signal of the event
                QS_U8_(QF_EVT_POOL_ID_(e));        // the pool Id of the event
                QS_U8_(QF_EVT_REF_CTR_(e));      // the ref count of the event
            QS_END_NOCRIT_()

            QF_CRIT_EXIT_();

            Q_ASSERT(idx < QF_maxPool_);

#ifdef Q_EVT_VIRTUAL
            QF_EVT_CONST_CAST_(e)->~QEvt();        // xtor, cast 'const' away,
                             // which is legitimate, because it's a pool event
#endif
                  // cast 'const' away, which is OK, because it's a pool event
            QF_EPOOL_PUT_(QF_pool_[idx], QF_EVT_CONST_CAST_(e));
        }
    }
}

QP_END_

