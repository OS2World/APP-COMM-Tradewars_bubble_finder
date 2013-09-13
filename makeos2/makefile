# makefile
# Created by IBM WorkFrame/2 MakeMake at 18:36:54 on 15 Feb 2000
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj 

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl30 /Q /Fi /Si /O /G5 /Gf /Gi /Gx /Ft- /C %s

{h:\twbubble}.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl30 /Q /Fi /Si /O /G5 /Gf /Gi /Gx /Ft- /C %s

all: \
    .\twbfos2.exe

.\twbfos2.exe: \
    .\TWSector.obj \
    .\TWWarp.obj \
    .\AbEnds.obj \
    .\Globals.obj \
    .\TWBFind.obj \
    .\TWBubble.obj \
    .\TWPlot.obj \
    .\TWRemaps.obj \
    {$(LIB)}con3216.lib
    @echo " Link::Linker "
    icc.exe @<<
     /Q /B" /exepack:2 /st:102400 /packd /optfunc     "
     /Fetwbfos2.exe 
     con3216.lib 
     .\TWSector.obj
     .\TWWarp.obj
     .\AbEnds.obj
     .\Globals.obj
     .\TWBFind.obj
     .\TWBubble.obj
     .\TWPlot.obj
     .\TWRemaps.obj
<<

.\TWSector.obj: \
    h:\twbubble\TWSector.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp \
    {h:\twbubble;$(INCLUDE);}TWSector.hpp \
    {h:\twbubble;$(INCLUDE);}TWBubble.hpp \
    {h:\twbubble;$(INCLUDE);}TWRemaps.hpp

.\TWRemaps.obj: \
    h:\twbubble\TWRemaps.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWRemaps.hpp

.\TWPlot.obj: \
    h:\twbubble\TWPlot.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp \
    {h:\twbubble;$(INCLUDE);}TWSector.hpp \
    {h:\twbubble;$(INCLUDE);}TWBubble.hpp \
    {h:\twbubble;$(INCLUDE);}TWPlot.hpp

.\TWBubble.obj: \
    h:\twbubble\TWBubble.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp \
    {h:\twbubble;$(INCLUDE);}TWSector.hpp \
    {h:\twbubble;$(INCLUDE);}TWBubble.hpp

.\TWBFind.obj: \
    h:\twbubble\TWBFind.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp \
    {h:\twbubble;$(INCLUDE);}TWSector.hpp \
    {h:\twbubble;$(INCLUDE);}TWBubble.hpp \
    {h:\twbubble;$(INCLUDE);}TWRemaps.hpp

.\Globals.obj: \
    h:\twbubble\Globals.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp \
    {h:\twbubble;$(INCLUDE);}TWSector.hpp \
    {h:\twbubble;$(INCLUDE);}TWBubble.hpp

.\AbEnds.obj: \
    h:\twbubble\AbEnds.cpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp \
    {h:\twbubble;$(INCLUDE);}TWBFind.h \
    {h:\twbubble;$(INCLUDE);}Globals.hpp \
    {h:\twbubble;$(INCLUDE);}AbEnds.hpp

.\TWWarp.obj: \
    h:\twbubble\TWWarp.cpp \
    {h:\twbubble;$(INCLUDE);}TWWarp.hpp \
    {h:\twbubble;$(INCLUDE);}TWTypes.hpp
