#if (defined(__EMX__) || defined(__OS2__))
    #define INCL_DOSPROCESS
    #define INCL_VIO
    #include <os2.h>
#endif
#include <signal.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include "TWBFind.h"
#include "TWRemaps.hpp"
#include "TWTypes.hpp"
#include "Globals.hpp"
#include "TWSector.hpp"
#include "TWWarp.hpp"
#include "TWBubble.hpp"
#include "AbEnds.hpp"

extern "C" void AbortCatcher(int sigstuff);
void WarpDiff(ULONG totWarps, ULONG newWarps);
void SetSectWidth(void);
void SetNumWidth(ULONG number);

char spaceStr[20], backStr[20];
int numWidth;

int main(int argc, char* argv[], char* envp[])
{
    char sectFileName[256], plotFileName[256], binFileName[256],
         auxFileName[256], outFileName[256], genFileName[256],
         portFileName[256];
    char argtype[16], argdata[256], argtemp[256], *sepptr;
    USHORT numBubbles, i, sdsect=0, class0_1=0, class0_2=0, userSect=0;
    ULONG totWarps=0;
    Bool readBinData=false, writeBinData=false,
         readWarpData=false, readPlotData=false,
         doBubbleSearch=true, readAuxData=false,
         readPortData=false, warpSort=true,
         reportWarps=false, reportUpdate=false,
         genCIM=false, benchSearch=false,
         fromStardock=false, fromClass0=false,
         fromTerra=false, fromUser=false;
    Bool minSet=false, maxSet=false, sNumSet=false, eMultSet=false,
         fBinSet=false, fSectSet=false, fPlotSet=false, fOutSet=false,
         tunSet=false, searchSet=false, entSet=false, aUnexSet=false,
         auxSet=false, fakeSet=false, bubMaxSet=false, gobSet=false,
         repSet=false, quietSet=false, genSet=false, noExSet=false,
         benchSet=false, sdSet=false, fPortSet=false, c0_1Set=false,
         c0_2Set=false, frsdSet=false, frc0Set=false, frtSet=false,
         portsSet=false, fruSet=false;
    #if (defined(__OS2__) || defined(__EMX__)) // priority options
    Bool priSet=false, deltaSet=false;
    #endif
    ifstream sectFile, plotFile, portFile;
    ofstream outFile, genFile;
    fstream binFile, auxFile;
    TWBubbleSet bubbleSet, keepSet;
    TWZappedPorts zp;
    struct timeb start, finish;
    double startd, finishd, total;

    signal(SIGINT, AbortCatcher);

    strcpy(sectFileName, "");
    strcpy(portFileName, "");
    strcpy(binFileName, "");
    strcpy(auxFileName, "");
    strcpy(plotFileName, "");
    strcpy(outFileName, "bubbles.lst");
    strcpy(genFileName, "");

    if (argc==1)
        BadUsage("", USAGE_HELP);

    for (i=1; i<argc; i++)
        {
        if (strlen(argv[i])>255)
            BadUsage(argv[i], BAD_ARG_TOO_LONG);
        memset(argtype, 0, sizeof(argtype));
        memset(argdata, 0, sizeof(argdata));
        sepptr=strchr(argv[i], ':');
        if (sepptr==NULL)
            {
            strncpy(argtype, argv[i]+1, sizeof(argtype)-1);
            }
        else
            {
            strncpy(argdata, sepptr+1, sizeof(argdata)-1);
            strncpy(argtype, argv[i]+1,
                    (strlen(argv[i])-strlen(argdata))-2);
            }
        if (!strcmpi(argtype, "avoidunex"))
            {
            if (aUnexSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::avoidUnexplored=true;
            aUnexSet=true;
            }
        else if (!strcmpi(argtype, "bench"))
            {
            if (benchSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            benchSearch=true;
            benchSet=true;
            }
        else if (!strcmpi(argtype, "bogus"))
            {
            if (fakeSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::bogusBubbles=true;
            fakeSet=true;
            }
        else if (!strcmpi(argtype, "eatmem"))
            {
            if (gobSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::gobbleMemory=true;
            gobSet=true;
            }
        else if (!strcmpi(argtype, "noexpset"))
            {
            if (noExSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::noExpSet=true;
            noExSet=true;
            }
        else if (!strcmpi(argtype, "q"))
            {
            if (quietSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::quietMode=true;
            quietSet=true;
            }
        else if (!strcmpi(argtype, "warps"))
            {
            if (repSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            reportUpdate=true;
            reportWarps=true;
            repSet=true;
            }
        else if (!strcmpi(argtype, "tunnel"))
            {
            if (tunSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::tunnelEntrance=true;
            tunSet=true;
            }
        else if (!strcmpi(argtype, "update"))
            {
            if (searchSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            doBubbleSearch=false;
            searchSet=true;
            }
        else if (!strcmpi(argtype, "ports"))
            {
            if (portsSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::showPorts=true;
            portsSet=true;
            }
        else if (!strcmpi(argtype, "fromsd"))
            {
            if (frsdSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            fromStardock=true;
            frsdSet=true;
            }
        else if (!strcmpi(argtype, "fromc0"))
            {
            if (frc0Set)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            fromClass0=true;
            frc0Set=true;
            }
        else if (!strcmpi(argtype, "fromterra"))
            {
            if (frtSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            fromTerra=true;
            frtSet=true;
            }
        else if (!strcmpi(argtype, "from"))
            {
            if (fruSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            userSect=(USHORT)atoi(argdata);
            if (userSect==0)
                BadUsage(argdata, BADSECT_NOT_INTEGER);
            fromUser=true;
            fruSet=true;
            }
        else if (!strcmpi(argtype, "min"))
            {
            if (minSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::minSectors=(USHORT)atoi(argdata);
            if (Globals::minSectors==0)
                BadUsage(argdata, BAD_MIN_SECTORS);
            minSet=true;
            }
        else if (!strcmpi(argtype, "max"))
            {
            if (maxSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::maxSectors=(USHORT)atoi(argdata);
            if (Globals::maxSectors==0)
                BadUsage(argdata, BAD_MAX_SECTORS);
            maxSet=true;
            }
        else if (!strcmpi(argtype, "maxent"))
            {
            if (entSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::maxEntrances=(USHORT)atoi(argdata);
            if (Globals::maxEntrances==0 || Globals::maxEntrances>50)
                BadUsage(argdata, BAD_MAX_ENTRANCES);
            entSet=true;
            }
        else if (!strcmpi(argtype, "entmult"))
            {
            if (eMultSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::entMultiplier=(float)atof(argdata);
            if (Globals::entMultiplier<2.0 ||
                 Globals::entMultiplier>1000.0)
                BadUsage(argdata, BAD_ENT_MULTIPLIER);
            eMultSet=true;
            }
        else if (!strcmpi(argtype, "unisize"))
            {
            if (sNumSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::totalSectors=(USHORT)atoi(argdata);
            if (Globals::totalSectors==0)
                BadUsage(argdata, BAD_TOTAL_SECTORS);
            sNumSet=true;
            }
        else if (!strcmpi(argtype, "maxbub"))
            {
            if (bubMaxSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::maxBubbles=(ULONG)atol(argdata);
            if (Globals::maxBubbles==0)
                BadUsage(argdata, BAD_MAX_BUBBLES);
            bubMaxSet=true;
            }
        else if (!strcmpi(argtype, "stardock"))
            {
            if (sdSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            sdsect=(USHORT)atoi(argdata);
            if (sdsect==0)
                BadUsage(argdata, BADSECT_NOT_INTEGER);
            Globals::nixSD=true;
            sdSet=true;
            }
        else if (!strcmpi(argtype, "class0"))
            {
            if (c0_1Set && c0_2Set)
                BadUsage(argv[i], BAD_DUPE_OPTION);

            if (!c0_1Set)
                {
                class0_1=(USHORT)atoi(argdata);
                if (class0_1==0)
                    BadUsage(argdata, BADSECT_NOT_INTEGER);
                c0_1Set=true;
                Globals::nixClass0=true;
                }
            else
                {
                class0_2=(USHORT)atoi(argdata);
                if (class0_2==0)
                    BadUsage(argdata, BADSECT_NOT_INTEGER);
                c0_2Set=true;
                }
            }
        else if (!strcmpi(argtype, "zap"))
            {
            USHORT newZap=(USHORT)atoi(argdata);
            if (newZap==0)
                BadUsage(argdata, BADSECT_NOT_INTEGER);
            if (!zp.addZap(newZap))
                OutOfMemory();
            }
        else if (!strcmpi(argtype, "sectfile"))
            {
            if (fSectSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(sectFileName, 0, sizeof(sectFileName));
            strcpy(sectFileName, argdata);
            fSectSet=true;
            }
        else if (!strcmpi(argtype, "portfile"))
            {
            if (fPortSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(portFileName, 0, sizeof(portFileName));
            strcpy(portFileName, argdata);
            fPortSet=true;
            }
        else if (!strcmpi(argtype, "plotfile"))
            {
            if (fPlotSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(plotFileName, 0, sizeof(plotFileName));
            strcpy(plotFileName, argdata);
            fPlotSet=true;
            }
        else if (!strcmpi(argtype, "binfile"))
            {
            if (fBinSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(binFileName, 0, sizeof(binFileName));
            strcpy(binFileName, argdata);
            fBinSet=true;
            }
        else if (!strcmpi(argtype, "auxbinfile"))
            {
            if (auxSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(auxFileName, 0, sizeof(auxFileName));
            strcpy(auxFileName, argdata);
            auxSet=true;
            }
        else if (!strcmpi(argtype, "outfile"))
            {
            if (fOutSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(outFileName, 0, sizeof(outFileName));
            strcpy(outFileName, argdata);
            fOutSet=true;
            }
        else if (!strcmpi(argtype, "cimoutfile"))
            {
            if (genSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            memset(genFileName, 0, sizeof(genFileName));
            strcpy(genFileName, argdata);
            genCIM=true;
            genSet=true;
            }
        #if (defined(__OS2__) || defined(__EMX__)) // priority options
        else if (!strcmpi(argtype, "pclass"))
            {
            if (priSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            Globals::pclass=(ULONG)atoi(argdata);
            if (Globals::pclass==0 || Globals::pclass>4)
                BadUsage(argdata, BAD_PRIORITY_CLASS);
            priSet=true;
            }
        else if (!strcmpi(argtype, "pdelta"))
            {
            if (deltaSet)
                BadUsage(argv[i], BAD_DUPE_OPTION);
            if (!strcmpi(argdata, "0"))
                {
                Globals::pdelta=0;
                }
            else
                {
                Globals::pdelta=(LONG)atoi(argdata);
                if (Globals::pdelta==0 || Globals::pdelta<0 || Globals::pdelta>31)
                    BadUsage(argdata, BAD_PRIORITY_DELTA);
                }
            deltaSet=true;
            }
        #endif
        else if (!strcmpi(argtype, "h") || !strcmpi(argtype, "help") ||
                    !strcmpi(argtype, "?"))
            {
            BadUsage("", USAGE_HELP);
            }
        else
            BadUsage(argtype, BAD_PARAMETER);
        }

    if (strcmp(auxFileName, "")!=0)
        {
        readAuxData=true;
        auxFile.open(auxFileName, ios::nocreate | ios::in | ios::binary);
        if (auxFile.fail())
            BadUsage(auxFileName, BAD_SECTOR_FILE);
        }

    if (strcmp(binFileName, "")!=0)
        {
        writeBinData=true;
        binFile.open(binFileName, ios::nocreate | ios::in | ios::binary);
        if (!binFile.fail())
            readBinData=true;
        }

    if (readBinData)
        {
        TWBinaryHeader bh;
        binFile.read((char*)&bh, sizeof(bh));
        if (bh.sig1!=_TWBSIG_1_ || bh.sig2!=_TWBSIG_2_)
            FileError(INVALID_BINARY);
        if (bh.majorVer<_TWBMIN_MAJOR_)
            FileError(INVALID_BINARY);
        else if (bh.majorVer==_TWBMIN_MAJOR_)
            {
            if (bh.minorVer<_TWBMIN_MINOR_)
                FileError(INVALID_BINARY);
            }
        if (bh.sectorCount<100 || bh.sectorCount>65535)
            FileError(INVALID_BINARY);
        else if (bh.sectorCount!=Globals::totalSectors)
            {
            Globals::totalSectors=bh.sectorCount;
            }
        }

    if (readAuxData)
        {
        TWBinaryHeader bh;
        auxFile.read((char*)&bh, sizeof(bh));
        if (bh.sig1!=_TWBSIG_1_ || bh.sig2!=_TWBSIG_2_)
            FileError(INVALID_BINARY);
        if (bh.majorVer<_TWBMIN_MAJOR_)
            FileError(INVALID_BINARY);
        else if (bh.majorVer==_TWBMIN_MAJOR_)
            {
            if (bh.minorVer<_TWBMIN_MINOR_)
                FileError(INVALID_BINARY);
            }
        if (!readBinData)
            {
            if (bh.sectorCount<100 || bh.sectorCount>65535)
                FileError(INVALID_BINARY);
            else if (bh.sectorCount!=Globals::totalSectors)
                {
                Globals::totalSectors=bh.sectorCount;
                }
            }
        else
            {
            if (bh.sectorCount!=Globals::totalSectors)
                FileError(INVALID_BINARY);
            }
        }

    if (Globals::maxEntrances>Globals::maxSectors ||
         Globals::maxEntrances>(Globals::maxSectors-Globals::minSectors))
        {
        memset(argtemp, 0, sizeof(argtemp));
        sprintf(argtemp, "%u", Globals::maxEntrances);
        BadUsage(argtemp, BAD_MAX_ENTRANCES);
        }
    if (Globals::minSectors>Globals::maxSectors ||
         Globals::minSectors>Globals::totalSectors ||
         Globals::minSectors<2)
        {
        memset(argtemp, 0, sizeof(argtemp));
        sprintf(argtemp, "%u", Globals::minSectors);
        BadUsage(argtemp, BAD_MIN_SECTORS);
        }
    if (Globals::maxSectors>Globals::totalSectors || Globals::maxSectors<2)
        {
        memset(argtemp, 0, sizeof(argtemp));
        sprintf(argtemp, "%u", Globals::maxSectors);
        BadUsage(argtemp, BAD_MAX_SECTORS);
        }

    if (strcmp(sectFileName, "")!=0)
        {
        readWarpData=true;
        sectFile.open(sectFileName, ios::nocreate | ios::in);
        if (sectFile.fail())
            BadUsage(sectFileName, BAD_SECTOR_FILE);
        }

    if (strcmp(portFileName, "")!=0)
        {
        readPortData=true;
        Globals::checkHidden=true;
        portFile.open(portFileName, ios::nocreate | ios::in);
        if (portFile.fail())
            BadUsage(portFileName, BAD_PORT_FILE);
        }

    if (strcmp(plotFileName, "")!=0)
        {
        readPlotData=true;
        plotFile.open(plotFileName, ios::nocreate | ios::in);
        if (plotFile.fail())
            BadUsage(plotFileName, BAD_PLOT_FILE);
        }

    if (!readPlotData && !readWarpData && !readBinData && !readAuxData)
        BadUsage("", NO_SECTOR_DATA_FILE);

    if (readBinData && !readPlotData && !readWarpData && !readAuxData)
        {
        if (doBubbleSearch || reportWarps)
            {
            writeBinData=false;
            warpSort=false;
            }
        reportUpdate=false;
        }

    if (doBubbleSearch)
        {
        outFile.open(outFileName, ios::trunc | ios::out);
        if (outFile.fail())
            BadUsage(outFileName, BAD_REPORT_FILE);
        }

    if (genCIM)
        {
        genFile.open(genFileName, ios::trunc | ios::out);
        if (genFile.fail())
            BadUsage(genFileName, BAD_GEN_FILE);
        }

    if (!Globals::sectors.setNumberOfSectors(Globals::totalSectors))
        OutOfMemory();

    SetSectWidth();

    DisableCursor();

    if (readBinData)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Reading binary data for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Reading binary sector data..." << flush;
            }

        Globals::sectors.readBinaryData(binFile);
        cout << endl;
        binFile.close();
        if (reportWarps)
            {
            totWarps=Globals::sectors.totalWarps();
            cout << endl << "Total warps: " << totWarps << endl;
            }
        }

    if (readAuxData)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Reading auxiliary binary data for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Reading auxiliary binary sector data..." << flush;
            }
        Globals::sectors.readBinaryData(auxFile);
        cout << endl;
        auxFile.close();
        if (reportUpdate)
            {
            ULONG newWarps=Globals::sectors.totalWarps();
            WarpDiff(totWarps, newWarps);
            totWarps=newWarps;
            }
        }

    if (readPlotData)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Reading zero-turn plot data for path: "
                 << Globals::spaceStr << Globals::spaceStr << "    " << flush;
            }
        else
            {
            cout << endl << "Reading zero-turn plot data..." << flush;
            }
        Globals::sectors.readPlotData(plotFile);
        cout << endl;
        plotFile.close();
        if (reportUpdate)
            {
            ULONG newWarps=Globals::sectors.totalWarps();
            WarpDiff(totWarps, newWarps);
            totWarps=newWarps;
            }
        }

    if (readWarpData)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Reading warp data for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Reading sector warp data..." << flush;
            }
        Globals::sectors.readWarpData(sectFile);
        cout << endl;
        sectFile.close();
        if (reportUpdate)
            {
            ULONG newWarps=Globals::sectors.totalWarps();
            WarpDiff(totWarps, newWarps);
            totWarps=newWarps;
            }
        }

    if (zp.numberOfZaps()>0)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Zapping port for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Zapping ports..." << flush;
            }
        zp.zapPorts();
        cout << endl;
        }

    if (readPortData)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Reading port data for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Reading port data..." << flush;
            }
        Globals::sectors.readPortData(portFile);
        cout << endl;
        }

    if (sdsect!=0)
        Globals::sectors.setStardock(sdsect);

    if (class0_1!=0)
        Globals::sectors.setClass0_1(class0_1);

    if (class0_2!=0)
        Globals::sectors.setClass0_2(class0_2);

    if (warpSort)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Sorting outgoing warps for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Sorting outgoing warps..." << flush;
            }
        Globals::sectors.sortWarps();
        cout << endl;
        }

    if (writeBinData)
        {
        binFile.open(binFileName, ios::trunc | ios::out | ios::binary);
        if (binFile.fail())
            FileError(BINARY_WRITE);
        if (!Globals::quietMode)
            {
            cout << endl << "Writing binary data for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Writing binary sector data..." << flush;
            }
        Globals::sectors.writeBinaryData(binFile);
        cout << endl;
        binFile.close();
        }

    if (genCIM)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Writing CIM-style output for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Writing CIM-style sector output..." << flush;
            }
        Globals::sectors.genCIMData(genFile);
        genFile.close();
        cout << endl;
        }

    if (doBubbleSearch)
        {
        if (!Globals::quietMode)
            {
            cout << endl << "Setting one-way warp status for sector: "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Setting sector one-way warp status..." << flush;
            }
        Globals::sectors.setOneWayStatus();
        cout << endl;

        #if (defined(__OS2__) || defined(__EMX__))
        DosSetPriority(2, Globals::pclass, Globals::pdelta, 0);
        #endif

        if (benchSearch)
            ftime(&start);

        if (!Globals::quietMode)
            {
            cout << endl << "Prospecting potential bubbles with entrance : "
                 << Globals::spaceStr << flush;
            }
        else
            {
            cout << endl << "Prospecting for potential bubbles..." << flush;
            }
        Globals::sectors.findBubbles(bubbleSet);
        cout << endl;

        if (bubbleSet.numberOfBubbles()>1)
            {
            cout << endl << "Trimming " << bubbleSet.numberOfBubbles()
                 << " potential bubbles..." << endl << flush;
            bubbleSet.trimBubblesTo(keepSet);
            }

        numBubbles=keepSet.numberOfBubbles();
        cout << endl << "Number of bubbles found: " << numBubbles
             << endl << flush;

        if (benchSearch)
            {
            ftime(&finish);

            startd=(double)start.time+((double)start.millitm/1000.0);
            finishd=(double)finish.time+((double)finish.millitm/1000.0);

            total=finishd-startd;

            cout << endl << "Search time = " << setiosflags(ios::fixed)
                 << setprecision(3) << total << " seconds." << endl;
            }

        if (!Globals::quietMode)
            {
            if (fromStardock || fromClass0 || fromTerra)
                cout << endl << "Calculating distances and writing bubble: ";
            else
                cout << endl << "Writing bubble: ";
            SetNumWidth(numBubbles);
            cout << spaceStr << flush;
            }
        else
            {
            if (fromStardock || fromClass0 || fromTerra)
                cout << endl << "Calculating distances and writing bubbles..." << flush;
            else
                cout << endl << "Writing bubbles..." << flush;
            }
        for (i=0; i<numBubbles; i++)
            {
            if (!Globals::quietMode)
                {
                cout << backStr << setw(numWidth) << (i+1) << flush;
                }
            if (fromStardock)
                keepSet[i].calcStardock();
            if (fromClass0)
                keepSet[i].calcClass0();
            if (fromTerra)
                keepSet[i].calcTerra();
            if (fromUser)
                keepSet[i].calcUser(userSect);
            keepSet[i].writeTo(outFile);
            }
        cout << endl;

        outFile.close();
        }

    #ifdef __linux__
    cout << endl << flush; // put a blank line before prompt
    #endif

    #ifdef DEBUG_ENABLED
    Globals::debug.close();
    #endif
    return 0;
}

extern "C" void AbortCatcher(int sigstuff)
{
    cout << endl << endl << "Aborting." << endl;
    exit(1);
}

void WarpDiff(ULONG totWarps, ULONG newWarps)
{
    ULONG diff=newWarps-totWarps;

    cout << endl << "Total warps: " << newWarps << endl;
    cout << endl << "Warps added by last input file: " << diff
         << endl << flush;
}

void SetSectWidth(void)
{
    char tstring[20];

    sprintf(tstring, "%u", Globals::totalSectors);
    Globals::sectWidth=strlen(tstring);
    memset(Globals::spaceStr, 0, sizeof(Globals::spaceStr));
    memset(Globals::backStr, 0, sizeof(Globals::backStr));
    memset(Globals::spaceStr, ' ', Globals::sectWidth);
    strcpy(Globals::backStr, "\b");
    for (int i=1; i<Globals::sectWidth; i++)
        strcat(Globals::backStr, "\b");
}

void SetNumWidth(ULONG number)
{
    char tstring[20];

    sprintf(tstring, "%u", number);
    numWidth=strlen(tstring);
    memset(spaceStr, 0, sizeof(spaceStr));
    memset(backStr, 0, sizeof(backStr));
    memset(spaceStr, ' ', numWidth);
    strcpy(backStr, "\b");
    for (int i=1; i<numWidth; i++)
        strcat(backStr, "\b");
}
