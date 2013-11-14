/*
+------------------------------------------------------------------------------+
| *************************** pcos.h ***************************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2006 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| PCOS                                                                         |
+------------------------------------------------------------------------------+
| Author: Jürgen Tölke, started at: 25.04.2006                                 |
+------------------------------------------------------------------------------+
*/

#ifndef __PCOS_H__
#define __PCOS_H__

#include "eclr.h"
#include "pcoslib.h"

class ForceListElement : public eListNode<ForceListElement>, public ClrAllocator
{
public:
    ForceListElement(const char* pItemName);
    ~ForceListElement();
    void SetValueInt(PcosDebugValue& debugValue, ClrInt64& val);
    bool SetValueString(PcosDebugValue& debugValue, const char* pValue);
    void Forcen();

    char*       m_pItemName;
    char*       m_pStringValue;
    ClrInt64    m_val64;
    PcosDebugValue m_DebugValue;
    bool        m_isIECString;

private:
    void init();
};

#endif //#ifndef __PCOS_H__

