#include "TWPlot.hpp"
#include "TWSector.hpp"
#include "TWWarp.hpp"
#include "Globals.hpp"

Bool TWPlot::findCourse(void)
{
    USHORT nCursor=0;
    UCHAR numWarps;
    Bool destReached=false;
    TWWarp twarp;
    TWSector tsect;
    USHORT i;

    if (start==dest)
        {
        cl=0;
        return true;
        }

    reset();
    nodes[nc]=TWPlotNode(start, 0);
    nc++;
    map->set(start-1, true);

    tsect=Globals::sectors[nodes[nCursor].sector()-1];
    numWarps=tsect.numberOfOutWarps();
    for (i=0; i<numWarps; i++)
        {
        twarp=tsect.outWarpAtPosition(i);
        if (twarp.sector()==dest)
            {
            destReached=true;
            addNode(TWPlotNode(twarp.sector(), nCursor));
            break;
            }
        addNode(TWPlotNode(twarp.sector(), nCursor));
        }

    while (nCursor<nc && !destReached)
        {
        nCursor++;
        tsect=Globals::sectors[nodes[nCursor].sector()-1];
        numWarps=tsect.numberOfOutWarps();
        for (i=0; i<numWarps; i++)
            {
            twarp=tsect.outWarpAtPosition(i);
            if (twarp.sector()==dest)
                {
                destReached=true;
                addNode(TWPlotNode(twarp.sector(), nCursor));
                break;
                }
            else
                {
                addNode(TWPlotNode(twarp.sector(), nCursor));
                }
            }
        }

    if (!destReached)
        return false;

    if (!constructCourse())
        return false;

    return true;
}

Bool TWPlot::constructCourse(void)
{
    TWSector tsect;
    TWWarp twarp;
    USHORT nCursor=nc-1;

    Bool startFound=false;

    course[(MAX_COURSE-1)-cl]=nodes[nCursor].sector();
    cl++;

    while (!startFound)
        {
        if (cl>=MAX_COURSE)
            return false;
        nCursor=nodes[nCursor].trail();
        if (nodes[nCursor].sector()==start)
            {
            startFound=true;
            course[(MAX_COURSE-1)-cl]=nodes[nCursor].sector();
            cl++;
            }
        else
            {
            course[(MAX_COURSE-1)-cl]=nodes[nCursor].sector();
            cl++;
            }
        }

    truncLeft();
    return true;
}

void TWPlot::truncLeft(void)
{
    USHORT tc[MAX_COURSE];

    memcpy(tc, course+MAX_COURSE-cl, cl*sizeof(USHORT));
    memcpy(course, tc, sizeof(course));
}
