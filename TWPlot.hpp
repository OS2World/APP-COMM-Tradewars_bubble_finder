#include "TWTypes.hpp"
#include "AbEnds.hpp"
#include "TWBFind.h"
#include "Globals.hpp"
#include <stdlib.h>
#include <limits.h>
#if (defined(__GNUC__))
    #include <string.h>
#endif

#ifndef _TWPLOT_HPP_
#define _TWPLOT_HPP_

class TWNodeMap
{
    public:

    inline TWNodeMap()
    {
        bf=0;
    }

    inline TWNodeMap(USHORT newSize)
    {
        bf=0;
        setSize(newSize);
    }

    inline ~TWNodeMap()
    {
        if (bf!=NULL)
            delete[] bf;
    }

    inline void setSize(USHORT newSize)
    {
        size=newSize;
        if (bf!=NULL)
            delete[] bf;
        bf = new Bool[size];
        memset(bf, 0, size*sizeof(Bool));
    }

    inline Bool isSet(USHORT offset)
    {
        return bf[offset];
    }

    inline void set(USHORT offset, Bool val)
    {
        bf[offset]=val;
    }

    inline void reset(void)
    {
        if (bf==NULL)
            return;
        memset(bf, 0, size*sizeof(Bool));
    }

    private:

    Bool *bf;
    USHORT size;
};

class TWPlotNode
{
    public:

    inline TWPlotNode()
    {
        trailp=0;
        sect=0;
    }

    inline TWPlotNode(USHORT sectNum, USHORT newTrail)
    {
        trailp=newTrail;
        sect=sectNum;
    }

    inline void setSector(USHORT newSect)
    {
        sect=newSect;
    }

    inline USHORT sector(void)
    {
        return sect;
    }

    inline void setTrail(USHORT newTrail)
    {
        trailp=newTrail;
    }

    inline USHORT trail(void)
    {
        return trailp;
    }

    private:

    USHORT sect;
    USHORT trailp;
};

class TWPlot
{
    public:

    inline TWPlot()
    {
        initData();
        start=0;
        dest=0;
    }

    inline TWPlot(USHORT startSect, USHORT destSect)
    {
        initData();
        dest=destSect;
        start=startSect;
    }

    inline ~TWPlot()
    {
        nixData();
    }

    inline USHORT destSector(void)
    {
        return dest;
    }

    inline USHORT startSector(void)
    {
        return start;
    }

    inline void setStart(USHORT startSect)
    {
        start=startSect;
    }

    inline void setDest(USHORT destSect)
    {
        dest=destSect;
    }

    inline Bool findCourse(USHORT startSect, USHORT destSect)
    {
        dest=destSect;
        start=startSect;
        return findCourse();
    }

    Bool findCourse(void);

    inline USHORT sectorAt(USHORT coursePos)
    {
        return course[coursePos];
    }

    inline USHORT courseLength(void)
    {
        return cl;
    }

    private:

    inline void initData(void)
    {
        map = new TWNodeMap(Globals::totalSectors);
        if (map==NULL)
            OutOfMemory();
        nodes = new TWPlotNode[Globals::totalSectors];
        if (nodes==NULL)
            OutOfMemory();
        cl=0;
        nc=0;
    }

    inline void reset(void)
    {
        map->reset();
        nc=0;
        cl=0;
    }

    inline void nixData(void)
    {
        if (nodes!=NULL)
            delete[] nodes;
        nodes=0;
        if (map!=NULL)
            delete map;
        map=0;
    }

    inline void addNode(TWPlotNode nn)
    {
        if (map->isSet(nn.sector()-1))
            return;
        nodes[nc]=nn;
        nc++;
        map->set(nn.sector()-1, true);
    }

    Bool constructCourse(void);
    void truncLeft(void);

    USHORT start, dest, nc, cl;
    USHORT course[MAX_COURSE];
    TWPlotNode *nodes;
    TWNodeMap *map;
};
#endif
