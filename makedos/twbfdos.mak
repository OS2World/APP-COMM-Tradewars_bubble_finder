.AUTODEPEND

.PATH.obj = .

#		*Translator Definitions*
CC = tcc +TWBFDOS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = H:\PROGLANG\TC\LIB
INCLUDEPATH = H:\PROGLANG\TC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 twplot.obj \
 globals.obj \
 twbfind.obj \
 twbubble.obj \
 twremaps.obj \
 twsector.obj \
 twwarp.obj \
 abends.obj

#		*Explicit Rules*
.\twbfdos.exe: twbfdos.cfg $(EXE_dependencies)
  $(TLINK) /x/c/L$(LIBPATH) @&&|
c0l.obj+
.\twplot.obj+
.\globals.obj+
.\twbfind.obj+
.\twbubble.obj+
.\twremaps.obj+
.\twsector.obj+
.\twwarp.obj+
.\abends.obj
.\twbfdos
		# no map file
emu.lib+
mathl.lib+
cl.lib
|
    upx twbfdos.exe
    del *.obj


#		*Individual File Dependencies*
twplot.obj: twbfdos.cfg twplot.cpp 

globals.obj: twbfdos.cfg globals.cpp 

twbfind.obj: twbfdos.cfg twbfind.cpp 

twbubble.obj: twbfdos.cfg twbubble.cpp 

twremaps.obj: twbfdos.cfg twremaps.cpp 

twsector.obj: twbfdos.cfg twsector.cpp 

twwarp.obj: twbfdos.cfg twwarp.cpp 

abends.obj: twbfdos.cfg abends.cpp 

#		*Compiler Configuration File*
twbfdos.cfg: twbfdos.mak
  copy &&|
-ml
-ff-
-G
-O
-Z
-b-
-vi-
-w-bei
-w-lin
-w-obi
-w-ofp
-w-ovl
-wpin
-wamb
-wamp
-wasm
-wcln
-wdef
-w-rng
-w-rvl
-wsig
-wnod
-w-eff
-w-aus
-w-rch
-w-rpt
-wstv
-w-voi
-w-big
-wucp
-wuse
-w-zdi
-w-stu
-w-dpu
-w-eas
-w-hid
-w-ncf
-w-dsz
-w-ccc
-n.
-I$(INCLUDEPATH)
-L$(LIBPATH)
| twbfdos.cfg


