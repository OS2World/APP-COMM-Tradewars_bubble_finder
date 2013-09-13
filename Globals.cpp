#include "TWBFind.h"
#include "Globals.hpp"
#include "TWSector.hpp"

#if (defined(__OS2__) || defined(__EMX__)) // priority adjustment
ULONG Globals::pclass=2;
LONG Globals::pdelta=0;
#endif
USHORT Globals::minSectors=5;
USHORT Globals::maxSectors=50;
USHORT Globals::totalSectors=1000;
UCHAR Globals::maxEntrances=1;
float Globals::entMultiplier=2.0;
TWSectorArray Globals::sectors(1);
Bool Globals::tunnelEntrance=false;
Bool Globals::avoidUnexplored=false;
Bool Globals::bogusBubbles=false;
Bool Globals::gobbleMemory=false;
Bool Globals::quietMode=false;
Bool Globals::noExpSet=false;
Bool Globals::nixClass0=false;
Bool Globals::nixSD=false;
Bool Globals::showPorts=false;
Bool Globals::checkHidden=false;

ULONG Globals::maxBubbles=1024;
int Globals::sectWidth=5;
char Globals::spaceStr[20];
char Globals::backStr[20];

#ifdef DEBUG_ENABLED
ofstream Globals::debug("debug.log", ios::app);
#endif

UCHAR Globals::minorVersion=15;
UCHAR Globals::majorVersion=0;

ULONG ipow(ULONG base, ULONG exponent)
{
    ULONG answer=base;
    for (ULONG i=1; i<exponent; i++)
        {
        answer*=base;
        }
    return answer;
}

