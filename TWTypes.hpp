#ifndef _TWTYPES_HPP_
    #define _TWTYPES_HPP_

    #define ULONG unsigned long
    #define LONG long
    #define USHORT unsigned short
    #define UCHAR unsigned char
    #define CHAR char

    enum TWPortTypeConstants {portUndefined, portBBS, portBSB, portSBB, portSSB,
                              portSBS, portBSS, portSSS, portBBB, portSD, port0};

    typedef UCHAR TWPortType;
    typedef UCHAR Bool;
    #if (((__IBMCPP__ < 400) || defined(__TCPLUSPLUS__)) && !defined(__GNUC__))
        enum BooleanConstants {false=0, true=1};
    #endif
    #ifndef NULL
        #define NULL (void*)0
    #endif
#endif
