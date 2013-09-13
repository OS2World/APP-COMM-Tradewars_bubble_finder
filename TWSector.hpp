#include <string.h>
#include <fstream.h>

#include "TWTypes.hpp"
#include "TWBubble.hpp"
#include "TWWarp.hpp"
#include "AbEnds.hpp"

#ifndef _TWSECTOR_HPP_
#define _TWSECTOR_HPP_

typedef struct TWSectorData
{
    TWWarp *inWarps;
    TWWarp *outWarps;
#ifdef __TCPLUSPLUS__
    unsigned numInWarps:7;
    unsigned numOutWarps:3;
    unsigned explored:1;
    unsigned ptype:4;
    unsigned phidden:1;
#else
    UCHAR numInWarps;
    UCHAR numOutWarps;
    Bool explored;
    UCHAR ptype;
    Bool phidden;
#endif
};

typedef struct TWBinaryHeader
{
    ULONG sig1;
    ULONG sig2;
    ULONG reserved;
    UCHAR majorVer;
    UCHAR minorVer;
    USHORT sectorCount;
};

typedef struct TWBinarySectorData
{
    UCHAR explored;
    UCHAR portType;
    UCHAR reserved[2];
    USHORT outWarps[MAX_WARPS];
};

class TWSectorArray;

class TWSector
{
    public:

    inline TWSector()
    {
        initData();
    }

    inline ~TWSector()
    {
        nixData();
    }

    TWSector(TWSector& copy);
    TWSector& operator=(const TWSector& assign);

    inline UCHAR numberOfInWarps(void) const
    {
        return sdata->numInWarps;
    }
    inline UCHAR numberOfOutWarps(void) const
    {
        return sdata->numOutWarps;
    }

    inline TWWarp& inWarpAtPosition(UCHAR index)
    {
        return sdata->inWarps[index];
    }
    inline TWWarp& outWarpAtPosition(UCHAR index)
    {
        return sdata->outWarps[index];
    }

    Bool addInWarp(const TWWarp& addition);
    Bool addOutWarp(const TWWarp& addition);

    inline Bool isExplored(void) const
    {
        return sdata->explored;
    }

    inline void setExplored(Bool expState)
    {
        sdata->explored=expState;
    }

    inline Bool hasPort(void) const
    {
        if (sdata->ptype==portUndefined)
            return false;
        else
            return true;
    }

    inline UCHAR portType(void) const
    {
        return sdata->ptype;
    }

    inline void setPortType(UCHAR newtype)
    {
        sdata->ptype=newtype;
    }

    inline Bool portIsHidden(void)
    {
        return sdata->phidden;
    }

    inline void setPortHidden(Bool hid)
    {
        sdata->phidden=hid;
    }

    inline void killPort(void)
    {
        sdata->ptype=portUndefined;
        sdata->phidden=false;
    }

// Add the define for your compiler if it optimizes better with the
// following functions inlined.  Make sure to modify the .CPP file
// accordingly.

    #if (defined(__IBMCPP__))
    inline Bool TWSector::hasInWarp(USHORT testSect) const
    {
        if (sdata->numInWarps==0)
            return false;
        for (USHORT i=0; i<sdata->numInWarps; i++)
            {
            if (sdata->inWarps[i].sector()==testSect)
                return true;
            }
        return false;
    }

    inline Bool TWSector::hasOutWarp(USHORT testSect) const
    {
        if (sdata->numOutWarps==0)
            return false;
        for (USHORT i=0; i<sdata->numOutWarps; i++)
            {
            if (sdata->outWarps[i].sector()==testSect)
                return true;
            }
        return false;
    }
    #else
    Bool hasInWarp(USHORT testSect) const;
    Bool hasOutWarp(USHORT testSect) const;
    #endif

    private:

    friend TWSectorArray;

    inline void initData(void)
    {
        sdata = new TWSectorData;
        if (sdata==NULL)
            OutOfMemory();
        sdata->numInWarps=0;
        sdata->numOutWarps=0;
        sdata->inWarps=0;
        sdata->outWarps=0;
        sdata->explored=false;
        sdata->ptype=0;
        sdata->phidden=false;
    }

    inline void nixData(void)
    {
        if (sdata->numInWarps>0)
            delete[] sdata->inWarps;
        if (sdata->numOutWarps>0)
            delete[] sdata->outWarps;
        delete sdata;
        sdata=0;
    }

    inline TWWarp* outWarpPointer(void)
    {
        return sdata->outWarps;
    }

    TWSectorData *sdata;
};

class TWSectorArray
{
    public:

    inline TWSectorArray(USHORT ns)
    {
        numSects=ns;
        sects = new TWSector[numSects];
        if (sects==NULL)
            OutOfMemory();
        sdsect=0;
        c01=0;
        c02=0;
    }

    inline TWSectorArray()
    {
        numSects=0;
        sects=0;
        sdsect=0;
        c01=0;
        c02=0;
    }

    inline ~TWSectorArray()
    {
        if (numSects>0)
            delete[] sects;
    }

    inline Bool setNumberOfSectors(USHORT ns)
    {
        if (numSects>0)
            delete[] sects;
        numSects=ns;
        sects = new TWSector[numSects];
        if (sects==NULL)
            return false;
        else
            return true;
    }

    inline USHORT numberOfSectors(void) const
    {
        return numSects;
    }

    inline TWSector& operator[](USHORT index)
    {
        return sects[index];
    }

    inline TWSector& sectorAtPosition(USHORT index)
    {
        return sects[index];
    }

    inline USHORT stardockSector(void)
    {
        return sdsect;
    }

    inline USHORT class0_1Sector(void)
    {
        return c01;
    }

    inline USHORT class0_2Sector(void)
    {
        return c02;
    }

    inline void setStardock(USHORT snum)
    {
        sects[snum-1].setPortType(portSD);
        sdsect=snum;
    }

    inline void setClass0_1(USHORT snum)
    {
        sects[snum-1].setPortType(port0);
        c01=snum;
    }

    inline void setClass0_2(USHORT snum)
    {
        sects[snum-1].setPortType(port0);
        c02=snum;
    }

    void readWarpData(ifstream &dataStream);
    void readPlotData(ifstream &dataStream);
    void readPortData(ifstream &dataStream);
    void readBinaryData(fstream &dataStream);
    void writeBinaryData(fstream &dataStream);
    void genCIMData(ofstream &dataStream);
    void setOneWayStatus(void);
    void sortWarps(void);
    ULONG totalWarps(void);
    void findBubbles(TWBubbleSet &bubbleSet);
    Bool traverseBubble(TWBubbleSet &bubbleSet);

    private:

    Bool addWarps(USHORT previous, USHORT sectNum);

    inline void maxBubblesMet(void)
    {
        cout << endl << endl << "Maximum bubbles (" << Globals::maxBubbles
             << ") exceeded, terminating search." << endl << endl
             << "Either adjust parameters to find less bubbles, or use the"
             << endl << "/R switch to reserve more storage." << endl << flush;
        if (uniSects!=NULL)
            delete[] uniSects;
    }

    TWSector *sects;
    USHORT numSects, *uniSects, entrance, previous;
    ULONG bubbleCursor;
    UCHAR numUniSects;
    USHORT sdsect, c01, c02;
    TWBubble *currBubble;
    Bool bubbleGood, bubbleDone;
};

class TWZappedPorts
{
    public:

    TWZappedPorts()
    {
        zaps=0;
        numZaps=0;
    }

    ~TWZappedPorts()
    {
        if (numZaps>0)
            delete[] zaps;
    }

    Bool addZap(USHORT snum);

    USHORT numberOfZaps(void)
    {
        return numZaps;
    }

    USHORT operator [](USHORT index)
    {
        return zaps[index];
    }

    USHORT zapAtPosition(USHORT index)
    {
        return zaps[index];
    }

    void zapPorts(void);

    private:

    USHORT *zaps;
    USHORT numZaps;
};
#endif
