#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#include <fstream.h>
#include "TWTypes.hpp"

class TWSectorArray;

struct Globals
{
    #if (defined(__OS2__) || defined(__EMX__)) // priority adjustment
    static ULONG pclass;
    static LONG pdelta;
    #endif
    static USHORT minSectors, maxSectors, totalSectors;
    static UCHAR maxEntrances;
    static float entMultiplier;
    static TWSectorArray sectors;
    static Bool tunnelEntrance;
    static Bool avoidUnexplored;
    static Bool bogusBubbles;
    static Bool gobbleMemory;
    static Bool quietMode;
    static Bool noExpSet;
    static Bool nixClass0;
    static Bool nixSD;
    static Bool showPorts;
    static Bool checkHidden;
    static UCHAR minorVersion;
    static UCHAR majorVersion;
    static ULONG maxBubbles;
    static int sectWidth;
    static char spaceStr[20];
    static char backStr[20];
    #ifdef DEBUG_ENABLED
    static ofstream debug;
    #endif
};

ULONG ipow(ULONG base, ULONG exponent);

#endif

