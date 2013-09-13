#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include "TWBFind.h"
#include "Globals.hpp"
#include "AbEnds.hpp"

void BadUsage(char* cause, ULONG type)
{
    cout << endl;
    cout << "Tradewars 2002 Bubble Finder v" << (int)Globals::majorVersion << "."
          << setfill('0') << setw(2) << (int)Globals::minorVersion;
    cout << "; by Mike Ruskai <thannymeister@yahoo.com>" << endl;
    cout << "This software is released to the public domain." << endl << endl;
    cout << "Usage: twbfind.exe [options]" << endl << endl;
    cout << "Options:" << endl << endl;
    cout << "/avoidunex - avoid unexplored sectors when finding bubbles" << endl;
    cout << "/bench - report bubble search time" << endl;
    cout << "/bogus - find bogus bubbles (one-way sectors in)" << endl;
    cout << "/eatmem - eat up memory to increase speed during search" << endl;
    cout << "/noexpset - don't set as explored any non-binary input" << endl;
    cout << "/q - suppress progress output, and only report results" << endl;
    cout << "/warps - display total warps, and number added by input files" << endl;
    cout << "/tunnel - use only one bubble route from primary entrance" << endl;
    cout << "/update - update binary database only - no bubble search" << endl;
    cout << "/ports - show ports in bubble report" << endl;
    cout << "/fromsd - calculate distance from stardock" << endl;
    cout << "/fromc0 - calculate distance from class 0 ports" << endl;
    cout << "/fromterra - calculate distance from Terra" << endl;
    cout << "/from:# - calculate distance from sector" << endl;
    cout << "/min:# - minimum number of sectors in bubble; default 5" << endl;
    cout << "/max:# - maximum number of sectors in bubble; default 50" << endl;
    cout << "/maxent:# - maximum number of entrances for bubbles (1 to 50); default 1" << endl;
    cout << "/entmult:# - ent multiplier for min bubble size (2.0 to 1000.0); default 2.0" << endl;
    cout << "/unisize:# - number of sectors in universe (100 to 65535); default 1000" << endl;
    cout << "/maxbub:# - reserve storage for bubbles (1 to 4.3 billion); default 1024" << endl;
    cout << "/stardock:# - specify stardock sector" << endl;
    cout << "/class0:# - specify second or third class 0 sector" << endl;
    cout << "/zap:# - remove port from binary data (destroyed, etc.)" << endl;
    #if (defined(__OS2__) || defined(__EMX__))
    cout << "/pclass:# - priority class (1 to 4); default=2" << endl;
    cout << "/pdelta:# - priority delta (0 to 31); default=0" << endl;
    #endif
    cout << "/sectfile:<filename> - CIM sector data file" << endl;
    cout << "/plotfile:<filename> - capture of CIM course plots (zero-turn mapping)" << endl;
    cout << "/binfile:<filename> - binary sector database file" << endl;
    cout << "/auxbinfile:<filename> - second binary database to combine info with" << endl;
    cout << "/portfile:<filename> - CIM port data file" << endl;
    cout << "/outfile:<filename> - report file; default 'bubbles.lst'" << endl;
    cout << "/cimoutfile:<filename> - write CIM-style data to file" << endl;
    cout << endl;
    cout << "Example: twbfind /min:6 /max:25 /unisize:5000 /sectfile:sectors.txt";
    cout << endl << endl;
    cout << "The above searches for bubbles with a minimum of 6 sectors and a maximum of";
    cout << endl << "25 sectors, with one entrance, in a 5000-sector universe, with data";
    cout << endl << "read from 'sectors.txt', and found bubbles written to the file";
    cout << endl << "'bubbles.lst'." << endl;

    switch (type)
        {
        case USAGE_HELP:
            break;

        case BADSECT_NOT_INTEGER:
            cout << endl << "Invalid sector number: " << cause << endl;
            break;

        case BAD_MIN_SECTORS:
            cout << endl << "Invalid minimum number of sectors: " << cause << endl;
            break;

        case BAD_MAX_SECTORS:
            cout << endl << "Invalid maximum number of sectors: " << cause << endl;
            break;

        case BAD_SECTOR_FILE:
            cout << endl << "Bad sector data filename: " << cause << endl;
            break;

        case BAD_REPORT_FILE:
            cout << endl << "Bad report filename: " << cause << endl;
            break;

        case BAD_TOTAL_SECTORS:
            cout << endl << "Invalid number of sectors: " << cause << endl;
            break;

        case BAD_PARAMETER:
            cout << endl << "Invalid parameter: " << cause << endl << endl;
            break;

        case BAD_MAX_ENTRANCES:
            cout << endl << "Invalid maximum number of entrances: " << cause << endl;
            break;

        case BAD_ENT_MULTIPLIER:
            cout << endl << "Invalid entrance multiplier: " << cause << endl;
            break;

        case BAD_DUPE_OPTION:
            cout << endl << "Duplicate option: " << cause << endl;
            break;

        case NO_SECTOR_DATA_FILE:
            cout << endl << "No sector data file provided." << endl;
            break;

        case BAD_MAX_BUBBLES:
            cout << endl << "Invalid maximum number of bubbles: " << cause << endl;
            break;

        case BAD_ARG_TOO_LONG:
            cout << endl << "Argument is too long: " << cause << endl;
            break;

        #if (defined(__OS2__) || defined(__EMX__))
        case BAD_PRIORITY_CLASS:
            cout << endl << "Invalid priority class: " << cause << endl;
            break;

        case BAD_PRIORITY_DELTA:
            cout << endl << "Invalid priority delta: " << cause << endl;
            break;
        #endif
        }

    #ifdef __linux__
    cout << endl << flush; // put a blank line before prompt
    #endif

    exit(1);
}

void BadSector(char* sectNum, USHORT lineNum, ULONG type)
{
    cout << endl << endl;
    cout << "Bad sector data on line " << lineNum << ": " << sectNum << endl;
    cout << endl;
    switch (type)
        {
        case BADSECT_UNSPECIFIED:
            break;

        case BADSECT_TOO_LARGE:
            cout << "Exceeds number of sectors (" << Globals::totalSectors << ")." << endl;
            break;

        case BADSECT_NOT_INTEGER:
            cout << "Not a valid integer." << endl;
            break;

        case BADSECT_DUP_DATA:
            cout << "Duplicate sector data." << endl;
            break;

        case BADSECT_MALFORMED_PLOT:
            cout << "Malformed plot data." << endl;
            break;
        }

    #ifdef __linux__
    cout << endl << flush; // put a blank line before prompt
    #endif

    exit(2);
}

void BadPortData(char* badData, USHORT lineNum, ULONG type)
{
    cout << endl << endl;
    cout << "Bad port data on line " << lineNum << ": " << badData << endl;
    cout << endl;
    switch (type)
        {
        case BADPORT_NOT_INTEGER:
            cout << "Not a valid integer." << endl;
            break;

        case BADPORT_MALFORMED_PORT_STRING:
            cout << "Malformed port string." << endl;
            break;
        }
    exit(2);
}

void FileError(ULONG type)
{
    cout << endl << endl;
    switch (type)
        {
        case SECTOR_READ:
            cout << "Error reading sector file." << endl;
            break;

        case REPORT_WRITE:
            cout << "Error writing to report file." << endl;
            break;

        case BINARY_WRITE:
            cout << "Error writing to binary sector file." << endl;
            break;

        case INVALID_BINARY:
            cout << "Specified binary file is not valid." << endl;
            break;

        case INCOMPATIBLE_INFO:
            cout << "More than 6 warps for a given sector were found." << endl;
            cout << endl << "Either invalid plot data was provided, or the sources are from different" << endl;
            cout << "universes." << endl;
            break;

        case BIN_TOO_SMALL:
            cout << "Specified binary file has too few sectors." << endl;
            break;
        }

    #ifdef __linux__
    cout << endl << flush; // put a blank line before prompt
    #endif

    exit(4);
}


void OutOfMemory(void)
{
    cout << endl;
    cout << "Out of memory." << endl;

    #ifdef __linux__
    cout << endl << flush; // put a blank line before prompt
    #endif

    exit(3);
}
