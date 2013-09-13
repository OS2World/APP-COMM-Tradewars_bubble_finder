#ifndef _TWBFIND_H_
#define _TWBFIND_H_

#define USAGE_HELP                       0
#define BAD_MIN_SECTORS                  1
#define BAD_MAX_SECTORS                  2
#define BAD_SECTOR_FILE                  3
#define BAD_REPORT_FILE                  4
#define BAD_TOTAL_SECTORS                5
#define BAD_PARAMETER                    6
#define BAD_MAX_ENTRANCES                7
#define BAD_PRIORITY_CLASS               8
#define BAD_PRIORITY_DELTA               9
#define BAD_ENT_MULTIPLIER               10
#define BAD_PLOT_FILE                    11
#define NO_SECTOR_DATA_FILE              12
#define BAD_DUPE_OPTION                  13
#define BAD_MAX_BUBBLES                  14
#define BAD_GEN_FILE                     15
#define BAD_ARG_TOO_LONG                 16
#define BAD_PORT_FILE                    17
#define BADSECT_UNSPECIFIED              100
#define BADSECT_TOO_LARGE                101
#define BADSECT_NOT_INTEGER              102
#define BADSECT_DUP_DATA                 103
#define BADSECT_MALFORMED_PLOT           104
#define SECTOR_READ                      201
#define REPORT_WRITE                     202
#define BINARY_WRITE                     203
#define INVALID_BINARY                   204
#define INCOMPATIBLE_INFO                205
#define BIN_TOO_SMALL                    206
#define PORT_READ                        207
#define BADPORT_MALFORMED_PORT_STRING    300
#define BADPORT_NOT_INTEGER              301

#define MAX_WARPS 6
#define MAX_COURSE 256

#define _TWBSIG_1_ 0xADDE3412
#define _TWBSIG_2_ 0x2143CEFA

#define _TWBMIN_MAJOR_ 0
#define _TWBMIN_MINOR_ 14

#endif
