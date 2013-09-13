#if (defined(__OS2__) || defined(__EMX__))
    #define INCL_DOSPROCESS
    #include <os2.h>
#endif
#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#if (!defined(__linux__) && !defined(__FOOBAR__))
    #include <conio.h>
#endif
#include <stdlib.h>

#include "TWBFind.h"
#include "TWRemaps.hpp"
#include "TWSector.hpp"

TWSector::TWSector(TWSector& copy)
{
    initData();
    sdata->numInWarps=copy.sdata->numInWarps;
    if (sdata->numInWarps>0)
        {
        sdata->inWarps = new TWWarp[sdata->numInWarps];
        if (sdata->inWarps==NULL)
            OutOfMemory();
        memcpy(sdata->inWarps, copy.sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
        }
    sdata->numOutWarps=copy.sdata->numOutWarps;
    if (sdata->numOutWarps>0)
        {
        sdata->outWarps = new TWWarp[sdata->numOutWarps];
        if (sdata->outWarps==NULL)
            OutOfMemory();
        memcpy(sdata->outWarps, copy.sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
        }
    sdata->explored=copy.sdata->explored;
}

TWSector& TWSector::operator=(const TWSector& assign)
{
    if (this==&assign)
        return *this;
    nixData();
    initData();
    sdata->numInWarps=assign.sdata->numInWarps;
    sdata->numOutWarps=assign.sdata->numOutWarps;
    if (sdata->numInWarps>0)
        {
        sdata->inWarps = new TWWarp[sdata->numInWarps];
        if (sdata->inWarps==NULL)
            OutOfMemory();
        memcpy(sdata->inWarps, assign.sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
        }
    if (sdata->numOutWarps>0)
        {
        sdata->outWarps = new TWWarp[sdata->numOutWarps];
        if (sdata->outWarps==NULL)
            OutOfMemory();
        memcpy(sdata->outWarps, assign.sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
        }
    sdata->explored=assign.sdata->explored;
    return *this;
}

Bool TWSector::addInWarp(const TWWarp& newSect)
{
    if (sdata->numInWarps==0)
        {
        sdata->inWarps = new TWWarp[1];
        if (sdata->inWarps==NULL)
            return false;
        sdata->numInWarps++;
        sdata->inWarps[0]=newSect;
        }
    else
        {
        if (!this->hasInWarp(newSect.sector()))
            {
            TWWarp *tempArray = new TWWarp[sdata->numInWarps+1];
            if (tempArray==NULL)
                return false;
            memcpy(tempArray, sdata->inWarps, sizeof(TWWarp)*sdata->numInWarps);
            tempArray[sdata->numInWarps]=newSect;
            sdata->numInWarps++;
            delete[] sdata->inWarps;
            sdata->inWarps=tempArray;
            }
        }
    return true;
}

Bool TWSector::addOutWarp(const TWWarp& newSect)
{
    if (sdata->numOutWarps==0)
        {
        sdata->outWarps = new TWWarp[1];
        if (sdata->outWarps==NULL)
            return false;
        sdata->numOutWarps++;
        sdata->outWarps[0]=newSect;
        }
    else
        {
        if (!this->hasOutWarp(newSect.sector()))
            {
            if (this->numberOfOutWarps()>5)
                FileError(INCOMPATIBLE_INFO);
            TWWarp *tempArray = new TWWarp[sdata->numOutWarps+1];
            if (tempArray==NULL)
                return false;
            memcpy(tempArray, sdata->outWarps, sizeof(TWWarp)*sdata->numOutWarps);
            tempArray[sdata->numOutWarps]=newSect;
            sdata->numOutWarps++;
            delete[] sdata->outWarps;
            sdata->outWarps=tempArray;
            }
        }
    return true;
}

// Add your compiler's define if it optimizes better with the
// following functions inlined.  Be sure to modify the header
// accordingly.

#if (!defined(__IBMCPP__))
Bool TWSector::hasInWarp(USHORT testSect) const
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

Bool TWSector::hasOutWarp(USHORT testSect) const
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
#endif

void TWSectorArray::readWarpData(ifstream& sectFile)
{
    char sectLine[256], tokens[7][10], *toktemp;
    USHORT i, scount=0, lastSector=0;
    Bool noMoreTokens;
    USHORT lineNum=0, numTokens, sectNum=0, currWarpSect;
    TWWarp currWarp;

    sectFile.seekg(0);
    while (!sectFile.eof())
        {
        sectFile.getline(sectLine, 255);
        if (sectFile.eof())
            {
            if (!Globals::quietMode)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << sectNum << flush;
                }
            break;
            }
        if (sectFile.fail())
            FileError(SECTOR_READ);

        if (!strcmp(sectLine+strlen(sectLine)-1, "\r"))
            sectLine[strlen(sectLine)-1]=0x00; // Nix any CR

        lineNum++;
        noMoreTokens=false;
        numTokens=0;
        for (i=0; i<7; i++)
            {
            memset(tokens[i], 0, sizeof(tokens[i]));
            }
        toktemp=strtok(sectLine, " ");
        if (toktemp==NULL)
            continue;
        strcpy(tokens[numTokens], toktemp);
        numTokens++;
        while (!noMoreTokens)
            {
            toktemp=strtok(NULL, " ");
            if (toktemp==NULL)
                noMoreTokens=true;
            else
                {
                strcpy(tokens[numTokens], toktemp);
                numTokens++;
                }
            }
        sectNum=(USHORT)atoi(tokens[0]);
        if (sectNum==0)
            continue;
        if (sectNum>Globals::totalSectors)
            BadSector(tokens[0], lineNum, BADSECT_TOO_LARGE);
        if (sectNum<=lastSector)
            BadSector(tokens[0], lineNum, BADSECT_DUP_DATA);

        scount++;
        if (!Globals::quietMode)
            {
            if (scount%50==0)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << sectNum << flush;
                }
            }
        if (!Globals::noExpSet)
            sects[sectNum-1].setExplored(true);
        for (i=1; i<numTokens; i++)
            {
            currWarpSect=(USHORT)atoi(tokens[i]);
            if (currWarpSect==0)
                BadSector(tokens[i], lineNum, BADSECT_NOT_INTEGER);
            if (currWarpSect>Globals::totalSectors)
                BadSector(tokens[i], lineNum, BADSECT_TOO_LARGE);
            currWarp.setSector(currWarpSect);
            currWarp.setOneWay(false);
            if (!sects[sectNum-1].addOutWarp(currWarp))
                OutOfMemory();
            currWarp.setSector(sectNum);
            if (!sects[currWarpSect-1].addInWarp(currWarp))
                OutOfMemory();
            }
        }
}

void TWSectorArray::readPortData(ifstream& portFile)
{
    char portLine[256], orebuf[25], orgbuf[25], equbuf[25], *toktemp;
    Bool buyOre, buyOrg, buyEqu;
    USHORT lineNum=0, sectNum=0, scount=0, lastSect=0;
    TWPortType ptype;

    portFile.seekg(0);

    while (!portFile.eof())
        {
        portFile.getline(portLine, 255);
        if (portFile.eof())
            {
            cout << "\b\b\b\b\b" << setw(5) << sectNum << flush;
            break;
            }
        if (portFile.fail())
            FileError(PORT_READ);

        if (!strcmp(portLine+strlen(portLine)-1, "\r"))
            portLine[strlen(portLine)-1]=0x00; // Nix any CR

        lineNum++;
        toktemp=strtok(portLine, " "); // get sector
        if (toktemp==NULL)
            continue;
        sectNum=(USHORT)atoi(toktemp);
        if (sectNum==0)
            continue;
        if (sectNum>Globals::totalSectors)
            BadSector(toktemp, lineNum, BADSECT_TOO_LARGE);
        if (sectNum<=lastSect)
            BadSector(toktemp, lineNum, BADSECT_DUP_DATA);
        lastSect=sectNum;

        scount++;
        if (scount%50==0)
            {
            cout << "\b\b\b\b\b" << setw(5) << sectNum << flush;
            scount=0;
            }

        toktemp=strtok(NULL, "%"); // get ore string
        if (toktemp==NULL)
            BadPortData(portLine, lineNum, BADPORT_MALFORMED_PORT_STRING);
        strncpy(orebuf, toktemp, sizeof(orebuf)-1);

        toktemp=strtok(NULL, "%"); // get org string
        if (toktemp==NULL)
            BadPortData(portLine, lineNum, BADPORT_MALFORMED_PORT_STRING);
        strncpy(orgbuf, toktemp, sizeof(orgbuf)-1);

        toktemp=strtok(NULL, "%"); // get equ string
        if (toktemp==NULL)
            BadPortData(portLine, lineNum, BADPORT_MALFORMED_PORT_STRING);
        strncpy(equbuf, toktemp, sizeof(equbuf)-1);

        if (orebuf[0]=='-') // buying or selling ore
            {
            buyOre=true;
            orebuf[0]=' ';
            }
        else
            buyOre=false;
        if (orgbuf[1]=='-') // buying or selling org
            {
            buyOrg=true;
            orgbuf[1]=' ';
            }
        else
            buyOrg=false;
        if (equbuf[1]=='-') // buying or selling equ
            {
            buyEqu=true;
            equbuf[1]=' ';
            }
        else
            buyEqu=false;

        // determine port type
        if (buyOre && buyOrg && !buyEqu)
            ptype=portBBS;
        else if (buyOre && !buyOrg && buyEqu)
            ptype=portBSB;
        else if (!buyOre && buyOrg && buyEqu)
            ptype=portSBB;
        else if (!buyOre && !buyOrg && buyEqu)
            ptype=portSSB;
        else if (!buyOre && buyOrg && !buyEqu)
            ptype=portSBS;
        else if (buyOre && !buyOrg && !buyEqu)
            ptype=portBSS;
        else if (!buyOre && !buyOrg && !buyEqu)
            ptype=portSSS;
        else if (buyOre && buyOrg && buyEqu)
            ptype=portBBB;

        if (sects[sectNum-1].portType()==portSD)
            {
            if (ptype!=portBBB)
                sects[sectNum-1].setPortType(ptype);
            }
        else
            {
            sects[sectNum-1].setPortType(ptype);
            sects[sectNum-1].setPortHidden(false);
            }
        }
}

void TWSectorArray::readBinaryData(fstream& binFile)
{
    TWBinarySectorData bsdata;
    USHORT i, j;
    char numTemp[32];

    binFile.seekg(16);
    for (i=0; i<Globals::totalSectors; i++)
        {
        binFile.read((char*)&bsdata, sizeof(bsdata));
        if (binFile.eof())
            FileError(BIN_TOO_SMALL);
        if (binFile.fail())
            FileError(SECTOR_READ);

        if (!Globals::quietMode)
            {
            if ((i+1)%100==0)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }

        if (bsdata.portType==portSD)
            {
            if (Globals::nixSD)
                bsdata.portType=portUndefined;
            else
                sdsect=i+1;
            }
        else if (bsdata.portType==port0)
            {
            if (Globals::nixClass0)
                bsdata.portType=portUndefined;
            else
                {
                if (c01==0)
                    c01=i+1;
                else if (c02==0)
                    c02=i+1;
                else
                    bsdata.portType=portUndefined;
                }
            }
        sects[i].setPortType(bsdata.portType);
        if (Globals::checkHidden && bsdata.portType!=portUndefined)
            {
            if (bsdata.portType!=portSD && bsdata.portType!=port0)
                sects[i].setPortHidden(true);
            }
        sects[i].setExplored(bsdata.explored);
        for (j=0; j<MAX_WARPS; j++)
            {
            if (bsdata.outWarps[j]>Globals::totalSectors)
                {
                sprintf(numTemp, "%u", bsdata.outWarps[j]);
                BadSector(numTemp, i+1, BADSECT_TOO_LARGE);
                }
            if (bsdata.outWarps[j]!=0)
                {
                if (!sects[i].addOutWarp(TWWarp((USHORT)bsdata.outWarps[j])))
                    OutOfMemory();
                if (!sects[bsdata.outWarps[j]-1].addInWarp(TWWarp((USHORT)(i+1))))
                    OutOfMemory();
                }
            }
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

void TWSectorArray::writeBinaryData(fstream& binFile)
{
    TWBinarySectorData bsdata;
    USHORT i, j;
    TWBinaryHeader bh;

    memset(&bh, 0, sizeof(bh));
    bh.sig1=_TWBSIG_1_;
    bh.sig2=_TWBSIG_2_;
    bh.majorVer=Globals::majorVersion;
    bh.minorVer=Globals::minorVersion;
    bh.sectorCount=(ULONG)Globals::totalSectors;
    binFile.write((char*)&bh, sizeof(bh));
    if (binFile.fail())
        FileError(BINARY_WRITE);

    for (i=0; i<Globals::totalSectors; i++)
        {
        memset(&bsdata, 0, sizeof(bsdata));

        if (!Globals::quietMode)
            {
            if ((i+1)%100==0)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }

        bsdata.portType=sects[i].portType();
        bsdata.explored=sects[i].isExplored();
        for (j=0; j<sects[i].numberOfOutWarps(); j++)
            {
            bsdata.outWarps[j]=(USHORT)sects[i].outWarpAtPosition(j).sector();
            }

        binFile.write((char*)&bsdata, sizeof(bsdata));
        if (binFile.fail())
            FileError(BINARY_WRITE);
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

void TWSectorArray::readPlotData(ifstream& plotFile)
{
    char plotLine[256], sectString[32], *toktemp;
    const char tokStr[]=" > ";
    USHORT lineNum=0, startSect, endSect, sectNum;
    USHORT prevSect;
    Bool plotDone, startMatched, currExp;

    plotFile.seekg(0);
    while (!plotFile.eof())
        {
        plotFile.getline(plotLine, 255);

        if (plotFile.eof())
            break;

        if (plotFile.fail())
            FileError(SECTOR_READ);
        lineNum++;

        if (!strcmp(plotLine+strlen(plotLine)-1, "\r"))
            plotLine[strlen(plotLine)-1]=0x00; // Nix any CR

        toktemp=strtok(plotLine, tokStr);
        if (toktemp==NULL)
            continue;
        if (strcmp(toktemp, "FM")!=0)
            continue;
        else
            {
            toktemp=strtok(NULL, tokStr);
            if (toktemp==NULL)
                BadSector("N/A", lineNum, BADSECT_MALFORMED_PLOT);

            startSect=(USHORT)atoi(toktemp);
            if (startSect==0)
                BadSector(toktemp, lineNum, BADSECT_NOT_INTEGER);
            if (startSect>Globals::totalSectors)
                BadSector(toktemp, lineNum, BADSECT_TOO_LARGE);

            memset(plotLine, 0, sizeof(plotLine));
            plotFile.getline(plotLine, 255);
            lineNum++;

            if (plotFile.eof())
                break;
            if (plotFile.fail())
                FileError(SECTOR_READ);

            if (!strcmp(plotLine+strlen(plotLine)-1, "\r"))
                plotLine[strlen(plotLine)-1]=0x00; // Nix any CR

            toktemp=strtok(plotLine, tokStr);
            if (strcmp(toktemp, "TO")!=0)
                break;

            toktemp=strtok(NULL, tokStr);
            endSect=(USHORT)atoi(toktemp);
            if (endSect==0)
                BadSector(toktemp, lineNum, BADSECT_NOT_INTEGER);
            if (endSect>Globals::totalSectors)
                BadSector(toktemp, lineNum, BADSECT_TOO_LARGE);

            if (!Globals::quietMode)
                {
                cout << Globals::backStr << Globals::backStr << "\b\b\b\b"
                     << setw(Globals::sectWidth) << startSect << " -> "
                     << setw(Globals::sectWidth) << endSect
                     << flush;
                }

            plotDone=false;
            startMatched=false;
            while (!plotDone)
                {
                memset(plotLine, 0, sizeof(plotLine));
                plotFile.getline(plotLine, 255);
                lineNum++;
                if (plotFile.fail())
                    FileError(SECTOR_READ);

                if (!strcmp(plotLine+strlen(plotLine)-1, "\r"))
                    plotLine[strlen(plotLine)-1]=0x00; // Nix any CR

                toktemp=strtok(plotLine, tokStr);
                if (toktemp==NULL)
                    continue;

                if (!strcmp(toktemp, "***"))
                    break;
                if (!startMatched)
                    {
                    sectNum=(USHORT)atoi(toktemp);
                    if (sectNum==0)
                        BadSector(toktemp, lineNum, BADSECT_NOT_INTEGER);
                    if (sectNum>Globals::totalSectors)
                        BadSector(toktemp, lineNum, BADSECT_TOO_LARGE);
                    if (sectNum!=startSect)
                        BadSector("N/A", lineNum, BADSECT_MALFORMED_PLOT);
                    startMatched=true;
                    prevSect=sectNum;
                    }
                else
                    {
                    memset(sectString, 0, sizeof(sectString));
                    if (toktemp[0]=='(')
                        {
                        if (toktemp[strlen(toktemp)-1]!=')')
                            BadSector(toktemp, lineNum, BADSECT_MALFORMED_PLOT);
                        strncpy(sectString, toktemp+1, strlen(toktemp)-2);
                        currExp=false;
                        }
                    else
                        {
                        currExp=true;
                        strcpy(sectString, toktemp);
                        }
                    sectNum=(USHORT)atoi(sectString);
                    if (sectNum==0)
                        BadSector(sectString, lineNum, BADSECT_NOT_INTEGER);
                    if (sectNum>Globals::totalSectors)
                        BadSector(toktemp, lineNum, BADSECT_TOO_LARGE);
                    sects[sectNum-1].setExplored(currExp);
                    if (!sects[sectNum-1].addInWarp(TWWarp(prevSect)))
                        OutOfMemory();
                    if (!sects[prevSect-1].addOutWarp(TWWarp(sectNum)))
                        OutOfMemory();
                    prevSect=sectNum;
                    if (sectNum==endSect)
                        plotDone=true;
                    }
                do
                    {
                    toktemp=strtok(NULL, tokStr);
                    if (toktemp==NULL)
                        continue;

                    memset(sectString, 0, sizeof(sectString));
                    if (toktemp[0]=='(')
                        {
                        if (toktemp[strlen(toktemp)-1]!=')')
                            BadSector(toktemp, lineNum, BADSECT_MALFORMED_PLOT);
                        strncpy(sectString, toktemp+1, strlen(toktemp)-2);
                        currExp=false;
                        }
                    else
                        {
                        currExp=true;
                        strcpy(sectString, toktemp);
                        }
                    sectNum=(USHORT)atoi(sectString);
                    if (sectNum==0)
                        BadSector(sectString, lineNum, BADSECT_NOT_INTEGER);
                    sects[sectNum-1].setExplored(currExp);
                    if (!sects[sectNum-1].addInWarp(TWWarp(prevSect)))
                        OutOfMemory();
                    if (!sects[prevSect-1].addOutWarp(TWWarp(sectNum)))
                        OutOfMemory();
                    prevSect=sectNum;
                    if (sectNum==endSect)
                        plotDone=true;
                    } while (toktemp!=NULL);
                }
            }
        }
}

void TWSectorArray::setOneWayStatus(void)
{
    UCHAR warps;
    USHORT i, j, sectNum;
    TWWarp currWarp;

    for (i=0; i<Globals::totalSectors; i++)
        {
        if (!Globals::quietMode)
            {
            if ((i+1)%100==0)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }
        warps=sects[i].numberOfInWarps();
        for (j=0; j<warps; j++)
            {
            currWarp=sects[i].inWarpAtPosition(j);
            sectNum=currWarp.sector();
            if (sects[i].hasOutWarp(sectNum))
                sects[i].inWarpAtPosition(j).setOneWay(false);
            else
                {
                if (sects[i].numberOfOutWarps()!=0)
                    sects[i].inWarpAtPosition(j).setOneWay(true);
                }
            }
        warps=sects[i].numberOfOutWarps();
        for (j=0; j<warps; j++)
            {
            currWarp=sects[i].outWarpAtPosition(j);
            sectNum=currWarp.sector();
            if (sects[i].hasInWarp(sectNum))
                sects[i].outWarpAtPosition(j).setOneWay(false);
            else
                sects[i].outWarpAtPosition(j).setOneWay(true);
            }
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

void TWSectorArray::sortWarps(void)
{
    USHORT i;
    UCHAR numWarps;

    for (i=0; i<Globals::totalSectors; i++)
        {
        if (!Globals::quietMode)
            {
            if ((i+1)%100==0 || (i+1)==Globals::totalSectors)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }
        numWarps=sects[i].numberOfOutWarps();
        if (numWarps!=0)
            {
            qsort(sects[i].outWarpPointer(), numWarps,
                  sizeof(TWWarp), SortCompare);
            }
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

ULONG TWSectorArray::totalWarps(void)
{
    ULONG tot=0;

    for (USHORT i=0; i<numSects; i++)
        {
        tot+=(ULONG)(sects[i].numberOfOutWarps());
        }

    return tot;
}

void TWSectorArray::genCIMData(ofstream &genFile)
{
    UCHAR numWarps;
    char sstring[80], tstring[10];
    USHORT i;

    for (i=0; i<numSects; i++)
        {
        if (!Globals::quietMode)
            {
            if ((i+1)%100==0)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }
        if (Globals::avoidUnexplored)
            {
            if (!sects[i].isExplored())
                continue;
            }
        numWarps=sects[i].numberOfOutWarps();
        if (numWarps==0)
            continue;
        memset(sstring, 0, sizeof(sstring));
        sprintf(tstring, " %*d", Globals::sectWidth, (i+1));
        strcpy(sstring, tstring);
        for (int j=0; j<numWarps; j++)
            {
            sprintf(tstring, " %*d", Globals::sectWidth,
                    sects[i].outWarpAtPosition(j).sector());
            strcat(sstring, tstring);
            }
        genFile << sstring << endl;
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

void TWSectorArray::findBubbles(TWBubbleSet &bubbleSet)
{
    USHORT i, j, sectNum;
    UCHAR warps, uniIterator;
    ULONG uniMasks;

    for (i=0; i<numSects; i++)
        {
        #if (defined(__OS2__) || defined(__EMX__))
        if (Globals::pclass==3)
            {
            if ((i+1)%100==0)
                DosSleep(1);
            }
        #endif
        #ifdef __linux__
        if ((i+1)%50==0)
        #endif
        while (_kbhit())
            {
            if (_getch()==32)
                {
//                fflush(stdin);
                return;
                }
            }
        if (!Globals::quietMode)
            {
            if ((i+1)%10==0 || (i+1)==Globals::totalSectors)
                {
                cout << Globals::backStr << setw(Globals::sectWidth)
                     << (i+1) << flush;
                }
            }
        warps=sects[i].numberOfOutWarps();
        entrance=i+1;
        if (Globals::tunnelEntrance)
            {
            for (uniIterator=0; uniIterator<warps; uniIterator++)
                {
                currBubble = new TWBubble(entrance);
                if (currBubble==NULL)
                    OutOfMemory();
                if (!currBubble->addSector(sects[i].outWarpAtPosition(uniIterator).sector()))
                    OutOfMemory();
                bubbleCursor=1;
                uniSects = new USHORT[warps];
                if (uniSects==NULL)
                    OutOfMemory();
                numUniSects=0;
                for (j=0; j<warps; j++)
                    {
                    if (j!=uniIterator)
                        {
                        sectNum=sects[i].outWarpAtPosition(j).sector();
                        uniSects[numUniSects]=sectNum;
                        numUniSects++;
                        }
                    }
                if (!traverseBubble(bubbleSet))
                    {
                    return;
                    }
                }
            }
        else
            {
            uniMasks=(UCHAR)ipow(2L, (ULONG)warps);
            for (uniIterator=1; uniIterator<(uniMasks-1); uniIterator++)
                {
                currBubble = new TWBubble(entrance);
                if (currBubble==NULL)
                    OutOfMemory();
                bubbleCursor=1;
                uniSects = new USHORT[warps];
                if (uniSects==NULL)
                    OutOfMemory();
                numUniSects=0;
                for (j=0; j<warps; j++)
                    {
                    sectNum=sects[i].outWarpAtPosition(j).sector();
                    if ((uniIterator<<(31-j)) >> 31)
                        {
                        uniSects[numUniSects]=sectNum;
                        numUniSects++;
                        }
                    else
                        {
                        if (!currBubble->addSector(sectNum))
                            {
                            OutOfMemory();
                            }
                        }
                    }
                if (!traverseBubble(bubbleSet))
                    {
                    return;
                    }
                }
            }
        }
    if (!Globals::quietMode)
        {
        cout << Globals::backStr << setw(Globals::sectWidth)
             << Globals::totalSectors << flush;
        }
}

Bool TWSectorArray::addWarps(USHORT previous, USHORT current)
{
    UCHAR numWarps, i, j;
    TWWarp currWarp;
    USHORT sectNum;
    numWarps=sects[current-1].numberOfOutWarps();
    if ((currBubble->numberOfSectors()+numWarps)>(Globals::maxSectors+Globals::maxEntrances))
        {
        return false;
        }
    for (i=0; i<numWarps; i++)
        {
        currWarp=sects[current-1].outWarpAtPosition(i);
        sectNum=currWarp.sector();
        if (Globals::avoidUnexplored)
            {
            if (!sects[sectNum-1].isExplored())
                return false;
            }
        else
            {
            if (sects[sectNum-1].numberOfOutWarps()==0)
                return false;
            }
        if (currWarp.isOneWay())
            {
            if (!currBubble->addBackDoor(current))
                OutOfMemory();
            continue;
            }
        if (sectNum==previous)
            continue;
        if (currBubble->hasSector(sectNum))
            continue;
        for (j=0; j<numUniSects; j++)
            {
            if (sectNum==uniSects[j])
                return false;
            }
        if (!currBubble->addSector(sectNum))
            {
            OutOfMemory();
            }
        }
    numWarps=sects[current-1].numberOfInWarps();
    if ((currBubble->numberOfSectors()+numWarps)>(Globals::maxSectors+Globals::maxEntrances))
        {
        return false;
        }
    for (i=0; i<numWarps; i++)
        {
        currWarp=sects[current-1].inWarpAtPosition(i);
        sectNum=currWarp.sector();
        if (sectNum==previous)
            continue;
        if (currBubble->hasSector(sectNum))
            continue;
        for (j=0; j<numUniSects; j++)
            {
            if (sectNum==uniSects[j])
                return false;
            }
        if (!Globals::bogusBubbles)
            {
            if (!currBubble->addSector(sectNum))
                {
                OutOfMemory();
                }
            }
        else
            currBubble->setBogus(true);
        }
    return true;
}

Bool TWSectorArray::traverseBubble(TWBubbleSet& bubbleSet)
{
    USHORT sectNum;

    previous=entrance;
    bubbleGood=true;
    bubbleDone=false;
    while (!bubbleDone)
        {
        sectNum=currBubble->sectorAtPosition(bubbleCursor);
        if (sectNum==0)
            break;
        if (Globals::avoidUnexplored)
            {
            if (!sects[sectNum-1].isExplored())
                {
                bubbleGood=false;
                break;
                }
            }
        else
            {
            if (sects[sectNum-1].numberOfOutWarps()==0 &&
                !sects[sectNum-1].isExplored())
                {
                bubbleGood=false;
                break;
                }
            }
        if (!addWarps(previous, sectNum))
            {
            bubbleDone=true;
            bubbleGood=false;
            }
        else
            {
            if (Globals::maxEntrances>1)
                {
                if ((bubbleCursor+1>(USHORT)(currBubble->numberOfSectors() -
                     (bubbleCursor))*Globals::entMultiplier) &&
                     ((bubbleCursor+1)>Globals::minSectors) &&
                     (currBubble->numberOfSectors()-(bubbleCursor+1) <
                     Globals::maxEntrances))
                    {
                    USHORT tsect;
                    TWBubble *tempBubble = new TWBubble(*currBubble);
                    if (tempBubble==NULL)
                        OutOfMemory();
                    ULONG tempCursor=bubbleCursor;
                    while ((tempCursor+1)<tempBubble->numberOfSectors())
                        {
                        tempCursor++;
                        tsect=tempBubble->sectorAtPosition(tempCursor);
                        if (!tempBubble->addEntrance(tsect))
                            {
                            OutOfMemory();
                            }
                        }
                    if (Globals::bogusBubbles)
                        {
                        if (tempBubble->isBogus())
                            {
                            if (!bubbleSet.addBubble(*tempBubble))
                                {
                                maxBubblesMet();
                                return false;
                                }
                            }
                        else
                            {
                            delete tempBubble;
                            tempBubble=0;
                            }
                        }
                    else
                        {
                        if (!bubbleSet.addBubble(*tempBubble))
                            {
                            maxBubblesMet();
                            return false;
                            }
                        }
                    }
                }
            }
        previous=sectNum;
        bubbleCursor++;
        if ((bubbleCursor+1)>currBubble->numberOfSectors())
            bubbleDone=true;
        }
    if (bubbleGood && currBubble->numberOfSectors()>Globals::minSectors)
        {
        if (Globals::bogusBubbles)
            {
            if (currBubble->isBogus())
                {
                if (!bubbleSet.addBubble(*currBubble))
                    {
                    maxBubblesMet();
                    return false;
                    }
                }
            else
                {
                delete currBubble;
                currBubble=0;
                }
            }
        else
            {
            if (!bubbleSet.addBubble(*currBubble))
                {
                maxBubblesMet();
                return false;
                }
            }
        }
    else
        {
        delete currBubble;
        currBubble=0;
        }
    delete[] uniSects;
    uniSects=0;
    return true;
}

Bool TWZappedPorts::addZap(USHORT snum)
{
    if (numZaps==0)
        {
        zaps = new USHORT[1];
        if (zaps==NULL)
            return false;
        numZaps++;
        zaps[0]=snum;
        }
    else
        {
        USHORT *tempArray = new USHORT[numZaps+1];
        if (tempArray==NULL)
            return false;
        memcpy(tempArray, zaps, sizeof(USHORT)*numZaps);
        tempArray[numZaps]=snum;
        numZaps++;
        delete[] zaps;
        zaps=tempArray;
        }
    return true;
}

void TWZappedPorts::zapPorts(void)
{
    for (USHORT i=0; i<numZaps; i++)
        {
        if (!Globals::quietMode)
            {
            cout << Globals::backStr << setw(Globals::sectWidth) << zaps[i] << flush;
            }
        Globals::sectors[(zaps[i]-1)].killPort();
        }
}

