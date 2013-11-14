#ifndef __ECLRVERSION_H__
#define __ECLRVERSION_H__

#define ECLR_VERSION_MAJOR      2
#define ECLR_VERSION_MINOR      2
#define ECLR_VERSION_REVSN      0
// ATTENTION !!! Do not change the buildnumber manually.
// ATTENTION !!! This will be done automatically during the next buildprocess.
#define ECLR_VERSION_BUILD      20221


#define ECLR_VERSION_LITERAL2(a, b, c, d) #a "." #b "." #c "." #d
#define ECLR_VERSION_LITERAL(a, b, c, d) ECLR_VERSION_LITERAL2( a, b, c, d )
#define ECLR_VERSION_STRING  ECLR_VERSION_LITERAL(ECLR_VERSION_MAJOR, ECLR_VERSION_MINOR, ECLR_VERSION_REVSN, ECLR_VERSION_BUILD)


#endif //#define __ECLRVERSION_H__
