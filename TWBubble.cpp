#include <iomanip.h>
#include <memory.h>
#include <stdio.h>

#include "TWBFind.h"
#include "TWBubble.hpp"
#include "TWSector.hpp"
#include "TWWarp.hpp"
#include "AbEnds.hpp"
#include "TWPlot.hpp"

TWBubble::TWBubble(TWBubble& copy)
{
    initData();
    bd->numSectors=copy.bd->numSectors;
    bd->numEnts=copy.bd->numEnts;
    bd->numBD=copy.bd->numBD;
    bd->bogus=copy.bd->bogus;
    if (!Globals::gobbleMemory)
        {
        if (bd->numSectors>0)
            {
            bd->sectors = new USHORT[bd->numSectors];
            if (bd->sectors==NULL)
                OutOfMemory();
            }
        if (bd->numEnts>0)
            {
            bd->entrances = new USHORT[bd->numEnts];
            if (bd->entrances==NULL)
                OutOfMemory();
            }
        }

    if (bd->numSectors>0)
        memcpy(bd->sectors, copy.bd->sectors, sizeof(USHORT)*bd->numSectors);
    if (bd->numEnts>0)
        memcpy(bd->entrances, copy.bd->entrances, sizeof(USHORT)*bd->numEnts);

    if (bd->numBD>0)
        {
        bd->back = new USHORT[bd->numBD];
        if (bd->back==NULL)
            OutOfMemory();
        memcpy(bd->back, copy.bd->back, sizeof(USHORT)*bd->numBD);
        }
}

TWBubble& TWBubble::operator=(const TWBubble& assign)
{
    if (this==&assign)
        return *this;

    if (assign.bd==NULL)
        {
        nixData();
        return *this;
        }

    if (bd==NULL)
        {
        initData();
        }

    if (bd->numBD>0)
        delete[] bd->back;

    if (!Globals::gobbleMemory)
        {
        if (bd->numSectors>0)
            delete[] bd->sectors;
        if (bd->numEnts>0)
            delete[] bd->entrances;
        }

    bd->numSectors=assign.bd->numSectors;
    bd->numEnts=assign.bd->numEnts;
    bd->numBD=assign.bd->numBD;
    bd->bogus=assign.bd->bogus;

    if (!Globals::gobbleMemory)
        {
        if (bd->numSectors>0)
            {
            bd->sectors = new USHORT[bd->numSectors];
            if (bd->sectors==NULL)
                OutOfMemory();
            }
        if (bd->numEnts>0)
            {
            bd->entrances = new USHORT[bd->numEnts];
            if (bd->entrances==NULL)
                OutOfMemory();
            }
        }

    if (bd->numBD>0)
        {
        bd->back = new USHORT[bd->numBD];
        if (bd->back==NULL)
            OutOfMemory();
        memcpy(bd->back, assign.bd->back, sizeof(USHORT)*bd->numBD);
        }

    if (bd->numSectors>0)
        memcpy(bd->sectors, assign.bd->sectors, sizeof(USHORT)*bd->numSectors);
    if (bd->numEnts>0)
        memcpy(bd->entrances, assign.bd->entrances, sizeof(USHORT)*bd->numEnts);

    return *this;
}

Bool TWBubble::operator==(const TWBubble& comp) const
{
    if (bd->numSectors!=comp.bd->numSectors ||
        bd->numBD!=comp.bd->numBD ||
        bd->numEnts!=comp.bd->numEnts ||
        bd->bogus!=comp.bd->bogus)
        return false;
    for (USHORT i=0; i<bd->numSectors; i++)
        {
        if (bd->sectors[i]!=comp.bd->sectors[i])
            {
            return false;
            }
        }
    return true;
}

Bool TWBubble::containsBubble(const TWBubble& sBub) const
{
    if (bd->numSectors<sBub.bd->numSectors)
        return false;
    for (USHORT i=0; i<sBub.bd->numSectors; i++)
        {
        if (!hasSector(sBub.bd->sectors[i]))
            return false;
        }
    if (bd->numEnts>sBub.bd->numEnts)
        return false;
    else
        return true;
}

Bool TWBubble::addSector(USHORT newSect)
{
    if (bd==NULL)
        initData();

    if (!Globals::gobbleMemory)
        {
        if (bd->numSectors==0)
            {
            bd->sectors = new USHORT[1];
            if (bd->sectors==NULL)
                return false;
            }
        else
            {
            USHORT *tempArray = new USHORT[bd->numSectors+1];
            if (tempArray==NULL)
                return false;
            memcpy(tempArray, bd->sectors, sizeof(USHORT)*bd->numSectors);
            delete[] bd->sectors;
            bd->sectors=tempArray;
            }
        }
    if ((bd->numSectors+1)>(Globals::maxSectors+Globals::maxEntrances))
        return false;
    bd->sectors[bd->numSectors]=newSect;
    bd->numSectors++;
    return true;
}

Bool TWBubble::addEntrance(USHORT newEnt)
{
    if (newEnt==0)
        return true;
    if (bd==NULL)
        initData();

    if (!Globals::gobbleMemory)
        {
        if (bd->numEnts==0)
            {
            bd->entrances = new USHORT[1];
            if (bd->entrances==NULL)
                return false;
            }
        else
            {
            USHORT *tempArray = new USHORT[bd->numEnts+1];
            if (tempArray==NULL)
                return false;
            memcpy(tempArray, bd->entrances, sizeof(USHORT)*bd->numEnts);
            delete[] bd->entrances;
            bd->entrances=tempArray;
            }
        }
    if ((bd->numEnts+1)>Globals::maxEntrances)
        return false;
    bd->entrances[bd->numEnts]=newEnt;
    bd->numEnts++;
    return true;
}

// Add the define for your compiler if it optimizes better with
// the following functions inlined.  Make sure to also change
// the header file accordingly.

#if (!defined(__IBMCPP__))
Bool TWBubble::hasSector(USHORT sectNum) const
{
    if (bd->numSectors==0)
        return false;
    for (USHORT i=0; i<bd->numSectors; i++)
        {
        if (bd->sectors[i]==sectNum)
            return true;
        }
    return false;
}

Bool TWBubble::hasEntrance(USHORT sectNum) const
{
    if (bd->numEnts==0)
        return false;
    for (USHORT i=0; i<bd->numEnts; i++)
        {
        if (bd->entrances[i]==sectNum)
            return true;
        }
    return false;
}

Bool TWBubble::isBackDoor(USHORT sectNum) const
{
    if (bd->numBD==0)
        return false;
    for (USHORT i=0; i<bd->numBD; i++)
        {
        if (bd->back[i]==sectNum)
            return true;
        }
    return false;
}
#endif

Bool TWBubble::addBackDoor(USHORT sectNum)
{
    if (bd==NULL)
        initData();
    if (bd->numBD==0)
        {
        bd->back = new USHORT[1];
        if (bd->back==NULL)
            return false;
        bd->back[0]=sectNum;
        bd->numBD++;
        }
    else
        {
        USHORT *tempArray = new USHORT[bd->numBD+1];
        if (tempArray==NULL)
            return false;
        memcpy(tempArray, bd->back, sizeof(USHORT)*bd->numBD);
        tempArray[bd->numBD]=sectNum;
        bd->numBD++;
        delete[] bd->back;
        bd->back=tempArray;
        }
    return true;
}

Bool TWBubble::isBogus(void)
{
    if (!bd->bogus)
        return false;
    TWSector tsect;
    TWWarp twarp;
    for (int i=1; i<bd->numSectors; i++)
        {
        if (bd->numEnts>0)
            {
            if (hasEntrance(bd->sectors[i]))
                continue;
            }
        tsect=Globals::sectors[bd->sectors[i]-1];
        for (int j=0; j<tsect.numberOfInWarps(); j++)
            {
            twarp=tsect.inWarpAtPosition(j);
            if (twarp.isOneWay())
                {
                if (!hasSector(twarp.sector()))
                    return true;
                }
            }
        }
    return false;
}

void TWBubble::calcStardock(void)
{
    TWPlot *cplot;
    USHORT sdsect;
    char fromsd[4], tosd[4];

    sdsect=Globals::sectors.stardockSector();

    if (sdsect==0)
        {
        bd->sdhops=0;
        return;
        }
    bd->sdhops = new char[8];
    if (bd->sdhops==NULL)
        OutOfMemory();

    cplot = new TWPlot;

    cplot->setStart(bd->entrances[0]);
    cplot->setDest(sdsect);

    if (!cplot->findCourse())
        {
        strcpy(tosd, "?");
        }
    else
        {
        sprintf(tosd, "%d", cplot->courseLength());
        }

    cplot->setStart(sdsect);
    cplot->setDest(bd->entrances[0]);

    if (!cplot->findCourse())
        {
        strcpy(fromsd, "?");
        }
    else
        {
        sprintf(fromsd, "%d", cplot->courseLength());
        }

    sprintf(bd->sdhops, "%s/%s", tosd, fromsd);
    delete cplot;
}

void TWBubble::calcTerra(void)
{
    TWPlot *cplot;
    char fromt[4], tot[4];

    bd->terrahops = new char[8];
    if (bd->terrahops==NULL)
        OutOfMemory();

    cplot = new TWPlot;

    cplot->setStart(bd->entrances[0]);
    cplot->setDest(1);

    if (!cplot->findCourse())
        {
        strcpy(tot, "?");
        }
    else
        {
        sprintf(tot, "%d", cplot->courseLength());
        }

    cplot->setStart(1);
    cplot->setDest(bd->entrances[0]);

    if (!cplot->findCourse())
        {
        strcpy(fromt, "?");
        }
    else
        {
        sprintf(fromt, "%d", cplot->courseLength());
        }

    sprintf(bd->terrahops, "%s/%s", tot, fromt);
    delete cplot;
}

void TWBubble::calcUser(USHORT sect)
{
    TWPlot *cplot;
    char fromu[4], tou[4];

    bd->userSect=sect;

    bd->userhops = new char[8];
    if (bd->userhops==NULL)
        OutOfMemory();

    cplot = new TWPlot;

    cplot->setStart(bd->entrances[0]);
    cplot->setDest(sect);

    if (!cplot->findCourse())
        {
        strcpy(tou, "?");
        }
    else
        {
        sprintf(tou, "%d", cplot->courseLength());
        }

    cplot->setStart(sect);
    cplot->setDest(bd->entrances[0]);

    if (!cplot->findCourse())
        {
        strcpy(fromu, "?");
        }
    else
        {
        sprintf(fromu, "%d", cplot->courseLength());
        }

    sprintf(bd->userhops, "%s/%s", tou, fromu);
    delete cplot;
}

void TWBubble::calcClass0(void)
{
    TWPlot *cplot;
    USHORT c0sect;
    char fromp[4], top[4];

    c0sect=Globals::sectors.class0_1Sector();

    if (c0sect==0)
        {
        bd->c01hops=0;
        }
    else
        {
        bd->c01hops = new char[8];

        if (bd->c01hops==NULL)
            OutOfMemory();

        cplot = new TWPlot;

        cplot->setStart(bd->entrances[0]);
        cplot->setDest(c0sect);

        if (!cplot->findCourse())
            {
            strcpy(top, "?");
            }
        else
            {
            sprintf(top, "%d", cplot->courseLength());
            }

        cplot->setStart(c0sect);
        cplot->setDest(bd->entrances[0]);

        if (!cplot->findCourse())
            {
            strcpy(fromp, "?");
            }
        else
            {
            sprintf(fromp, "%d", cplot->courseLength());
            }

        sprintf(bd->c01hops, "%s/%s", top, fromp);
        delete cplot;
        }

    c0sect=Globals::sectors.class0_2Sector();

    if (c0sect==0)
        {
        bd->c02hops=0;
        }
    else
        {
        bd->c02hops = new char[8];

        if (bd->c02hops==NULL)
            OutOfMemory();

        cplot = new TWPlot;

        cplot->setStart(bd->entrances[0]);
        cplot->setDest(c0sect);

        if (!cplot->findCourse())
            {
            strcpy(top, "?");
            }
        else
            {
            sprintf(top, "%d", cplot->courseLength());
            }

        cplot->setStart(c0sect);
        cplot->setDest(bd->entrances[0]);

        if (!cplot->findCourse())
            {
            strcpy(fromp, "?");
            }
        else
            {
            sprintf(fromp, "%d", cplot->courseLength());
            }

        sprintf(bd->c02hops, "%s/%s", top, fromp);
        delete cplot;
        }
}

void TWBubble::writeTo(ofstream& outFile) const
{
    USHORT sectNum, i, j;
    UCHAR warps, sectsOnLine=0;
    TWSector tsect;
    TWWarp currWarp;

    if (bd->sdhops!=0 || bd->c01hops!=0 || bd->c02hops!=0 || bd->terrahops!=0)
        {
        outFile << "Distances:";
        if (bd->terrahops!=0)
            {
            outFile << "  Terra=" << bd->terrahops;
            }
        if (bd->sdhops!=0)
            {
            outFile << "  Stardock(" << Globals::sectors.stardockSector()
                    << ")=" << bd->sdhops;
            }
        if (bd->c01hops!=0)
            {
            outFile << "  Class 0(" << Globals::sectors.class0_1Sector()
                    << ")=" << bd->c01hops;
            }
        if (bd->c02hops!=0)
            {
            outFile << "  Class 0(" << Globals::sectors.class0_2Sector()
                    << ")=" << bd->c02hops;
            }
        if (bd->userhops!=0)
            {
            outFile << "  " << bd->userSect << "=" << bd->userhops;
            }
        outFile << endl << endl;
        }

    outFile << (bd->numSectors-bd->numEnts) << "-sector ";
    if (Globals::bogusBubbles)
        outFile << "bogus ";
    outFile << "bubble with ";
    if (bd->numEnts==1)
        {
        outFile << "entrance at " << bd->sectors[0] << "." << endl << endl;
        }
    else
        {
        outFile << bd->numEnts << " entrances at:" << endl;
        for (i=0; i<bd->numEnts; i++)
            {
            sectsOnLine++;
            outFile << setw(Globals::sectWidth+1) << bd->entrances[i];
            if ((((sectsOnLine+1)*(Globals::sectWidth+1))>79) || i==(bd->numEnts-1))
                {
                sectsOnLine=0;
                outFile << endl;
                }
            }
        outFile << endl;
        }
    outFile << "Sectors in bubble:" << endl;;
    sectsOnLine=0;
    for (i=1; i<bd->numSectors; i++)
        {
        sectNum=bd->sectors[i];
        if (hasEntrance(sectNum))
            {
            if (i==(bd->numSectors-1) && sectsOnLine!=0)
                outFile << endl;
            continue;
            }
        else if (isBackDoor(sectNum))
            {
            outFile << "(" << setw(Globals::sectWidth) << sectNum << ") ";
            }
        else
            outFile << " " << setw(Globals::sectWidth) << sectNum << "  ";
        sectsOnLine++;
        if ((((sectsOnLine+1)*(Globals::sectWidth+3))>79) || i==(bd->numSectors-1))
            {
            sectsOnLine=0;
            outFile << endl;
            }
        }
    outFile << endl;
    for (i=0; i<bd->numSectors; i++)
        {
        sectNum=bd->sectors[i];
        tsect=Globals::sectors[sectNum-1];
        outFile << "Sector " << setw(Globals::sectWidth) << sectNum;
        if (Globals::showPorts)
            {
            UCHAR ptype=Globals::sectors[sectNum-1].portType();
            if (Globals::sectors[sectNum-1].portIsHidden())
                outFile << " *";
            else
                outFile << "  ";
            switch (ptype)
                {
                case portUndefined:
                    outFile << "     ";
                    break;

                case portBBB:
                    outFile << "(BBB)";
                    break;

                case portBBS:
                    outFile << "(BBS)";
                    break;

                case portBSB:
                    outFile << "(BSB)";
                    break;

                case portBSS:
                    outFile << "(BSS)";
                    break;

                case portSSS:
                    outFile << "(SSS)";
                    break;

                case portSBB:
                    outFile << "(SBB)";
                    break;

                case portSBS:
                    outFile << "(SBS)";
                    break;

                case portSSB:
                    outFile << "(SSB)";
                    break;

                case portSD:
                    outFile << "( 9 )";
                    break;

                case port0:
                    outFile << "( 0 )";
                    break;

                default:
                    outFile << "(???)";
                    break;
                }
            }
        outFile << "  -  ";
        warps=tsect.numberOfInWarps();
        for (j=0; j<warps; j++)
            {
            currWarp=tsect.inWarpAtPosition(j);
            if (!currWarp.isOneWay())
                continue;
            outFile << "<" << setw(Globals::sectWidth) << currWarp.sector()
                    << "> ";
            }
        warps=tsect.numberOfOutWarps();
        for (j=0; j<warps; j++)
            {
            currWarp=tsect.outWarpAtPosition(j);
            if (currWarp.isOneWay())
                {
                outFile << "(";
                }
            else if (hasEntrance(currWarp.sector()))
                {
                outFile << "{";
                }
            else if (!Globals::sectors[currWarp.sector()-1].isExplored())
                {
                outFile << "[";
                }
            else
                outFile << " ";
            outFile << setw(Globals::sectWidth) << currWarp.sector();
            if (currWarp.isOneWay())
                {
                outFile << ") ";
                }
            else if (hasEntrance(currWarp.sector()))
                {
                outFile << "} ";
                }
            else if (!Globals::sectors[currWarp.sector()-1].isExplored())
                {
                outFile << "] ";
                }
            else
                outFile << "  ";
            }
        outFile << endl;
        }
    outFile << endl
              << "----------------------------------------"
              << "----------------------------------------"
              << endl << endl;
    if (outFile.fail())
        FileError(REPORT_WRITE);
}

TWBubbleSet& TWBubbleSet::operator=(const TWBubbleSet& assign)
{
    if (this==&assign)
        return *this;

    for (int i=0; i<assign.numBubbles; i++)
        {
        *bubbles[i]=*assign.bubbles[i];
        }

    numBubbles=assign.numBubbles;

    return *this;
}

void TWBubbleSet::trimBubblesTo(TWBubbleSet& keepSet)
{
    USHORT i, j;
    Bool biggestBubble;

    for (i=0; i<numBubbles; i++)
        {
        biggestBubble=true;
        for (j=0; j<numBubbles; j++)
            {
            if (i==j)
                continue;
            if (*bubbles[i]==*bubbles[j])
                {
                if (j<i)
                    {
                    continue;
                    }
                else
                    {
                    biggestBubble=false;
                    break;
                    }
                }
            else if (bubbles[j]->containsBubble(*bubbles[i]))
                {
                biggestBubble=false;
                break;
                }
            }
        if (biggestBubble)
            {
            TWBubble *tbub = new TWBubble(*bubbles[i]);
            keepSet.addBubble(*tbub);
            }
        }
}

