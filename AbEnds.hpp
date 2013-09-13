#include "TWTypes.hpp"

#ifndef _ABENDS_HPP_
#define _ABENDS_HPP_

void BadUsage(char* cause, ULONG type);

void BadSector(char* sect, USHORT line, ULONG type);

void BadPortData(char* badData, USHORT lineNum, ULONG type);

void FileError(ULONG type);

void OutOfMemory(void);

#endif
