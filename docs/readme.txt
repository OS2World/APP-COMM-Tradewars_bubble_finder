               README for Tradewars 2002 bubble finder v0.15


                  Contents ..........................  1

               I. Legal Stuff .......................  2

              II. Overview ..........................  2
    
             III. Getting Sector Data ...............  3

              IV. Running the Program ...............  6

               V. Output ............................  11

              VI. Limitations .......................  14
 
             VII. Performance .......................  14

            VIII. Notes on Program Behavior .........  14

              IX. How it Works ......................  16
                                                       
               X. Compiling the Program .............  17 

              XI. Author Information ................  18

             XII. History ...........................  18





























                                   Page 1
I. Legal Stuff

This program is provided free of charge, and I'm not liable for any damage 
it may do, nor do I guarantee any level of functionality.  You know the 
drill.

If you make any modifications, I require that you make an effort to contact 
me before distributing the modified form.  

================================================================================

II. Overview

If you don't play the BBS online door game Tradewars 2002, then you'll find 
this particular program quite useless indeed.

If you do play, however, you may find it extremely useful.  

You may still, however, not be sure what a "bubble" in the game is.  

If you've been playing for any amount of time, you've probably come to 
realize what a tunnel is, specifically, an arrangement of sectors something 
like this:

3343
     \
2231 - 556 - 783 - 3259 - 895
     /
1011

In this case, 895 would be a dead-end sector.  Only one way in, and one 
way out.  The more sectors like 783 and 3259 between it and 556, the better.

The helper program that I use, Trade Wars Helper version 8.8, is able to 
find tunnels like the above, but it's not able to handle something like 
this:

3343               4490
     \              |
2231 - 556 - 783 - 3259 - 895
     /              |
1011               2333

TW Helper would report the above as three one-sector tunnels, whereas in 
reality is is a 5-sector bubble, with the entrance at 556.

There are two other helper programs that I know of out there, entitled TWar 
and TWATS.  TWar claims to be able to find "tunnels" with non-sequential 
sectors (which apparently covers the above), and TWATS claims to be able to 
find large bubbles.  And for all I know, they both do the job very nicely 
indeed.  But the evaluation versions are too crippled to test this feature, 
and I see no compelling reason to pay for either one, since they appear to 
do nothing that TW Helper doesn't do as well or better.  

Except for the bubble finding, of course.  After spending about 5 seconds 
deciding that TW Helper's /I and /G commands together would take too much 

                                   Page 2
time, I set out to write a program that would find bubbles automatically.

It was quite a learning experience, and it's still possible that there's 
more that I haven't thought of which will make it into a future version.

Something new to the program is finding bubbles with more than one 
entrance.  Something like the following:


3343               4490                            3234
     \              |                            /
2231 - 556 - 783 - 3259 - 895 - 443 - 559 - 1235 - 4454
     /              |               \            \
1011               2333               990 - 784    1903

The entrances in the above would be 556 and 1235.  It shouldn't take much 
imagination to see how more entrances would fit in.

See notes on program behavior for caveats with multiple-entrance bubbles.

New to versions 0.13 and later is the ability to use CIM course plot
captures, which is what helper programs use to do zero-turn mapping.  What
these captures provide is the route between one sector and another.  For
each sector in the route, it tells us two things:

    1)  The sector in question has an outwarp to the next sector in the
        route.

    2)  The sector in question has an inwarp from the previous sector in
        the route.

With enough course plots, you can get complete information about a sector
without exploring it.

See the notes on program behavior for the potential downside to this
ability (which is why it can be avoided altogether or disabled).

================================================================================

III. Getting Sector Data

The program has four types of input:

    1) Binary database.
    2) CIM course plot session capture.
    3) CIM sector information.
    4) CIM port information.

You can use any one of the above, or any combination of them.  They are
read in the order listed above, so that the most accurate information is
read last.  The binary database, of course, is created by a previous run of
the program, which used any of the CIM captures for input.

Details on the different files:

1) Binary database - this file is created by TWBFind, if you specify a

                                   Page 3
binary file name on the command line.  The data is written after reading in
any data from the other two file formats.  The file consists of 16 bytes of
data for every sector.  There are two bytes each for the maximum of six
outwarps, an explored byte, and three reserved bytes. A typical hex editor
will show you one sector per line, with a smily face in the first column
from the left indicating that it's explored.

2)  CIM course plot session capture - this file is created by you, using
your communications program with a script.  How exactly you do this will
vary, of course.  In your comm script, you'll want to enter CIM mode (see
below), and plot various courses, which will show how sectors are
connected.  Plots are done by pressing "F", then entering the start and end
sectors.

The more plots, the more information.  The only way to get all information
without exploring is to plot a course from each sector to every other
sector, and back again.  In a universe with just 1000 sectors, this means
998,000 plots.  With 5000 sectors, it's 24,990,000 plots.  And with 20,000
sectors, you'd need to do 399,960,000 plots.  Needless to say, doing all
those plots would be time consuming.  So time consuming, in fact, that
actually exploring the universe would take considerably less time.

So, what you're looking for is a way to get a lot of warp information in
relatively few course plots.  The simplest way to do this is by using a
simple algorithm that picks two sectors, and plots a course in both
directions, where every sector is covered at least once.

Here are a few methods in the form [begin condition]; [cycle activity];
[end condition] <cycles required>, where S is the number of sectors in the
universe, N is the current sector during the cycle (Nb is the begin, Ne is
the end, when two sector values are required), and B is the base sector for
the first method:

    Level - N=S; B to N, N to B1; N=N-1 <S*2-2>

    Var   - N=S; N to N-1, N-1 to N; N=N-2 <S>

    Fold  - Nb=1, Ne=S; Nb to Ne, Ne to Nb; Nb=Nb+1, Ne=Ne-1 <S>

    Split - Nb=1, Ne=S/2+1; Nb to Ne, Ne to Nb; Nb=Nb+1, Ne=Ne+1 <S>

So the Level method, with a base sector of 1, would be 1 to 5000, 5000 to
1, 1 to 4999, 4999 to 1, etc.

The Var method would be 5000 to 4999, 4999 to 5000, 4998 to 4997, 4997 to
4998, etc.

The Fold method would be 1 to 5000, 5000 to 1, 2 to 4999, 4999 to 2, etc.

The Split method would be 1 to 2501, 2501 to 1, 2 to 2502, 2502 to 2, etc.

The least effective of the above four methods is Level, even though it does
twice as many plots as the others (and hence, takes twice as long to
performa as the others).

The other three methods are essentially identical in effectiveness, but

                                   Page 4
will vary from universe to universe.  They are not, however identical in
which warps they find, only in how many.  They will each find warps that
the others did not.

I gathered ZTM data from two test universes, one 5000 sectors, and one
20,000 sectors.  I then used a program to analyze their effectiveness, and
generate a chart.  The chart, followed by an explanation:

          /---------------------------------------\
          |             5,000 Sectors             |
          |---------------------------------------|
          |  Warps  | % Total | Bubbles | % Error |
/---------|---------|---------|---------|---------|
| Actual  |  13489  | 100.00  |  62/0   |   0.00  |
|  Level  |   9756  |  72.33  |  27/377 | 664.52  |
|    Var  |  12971  |  96.16  |  56/39  |  72.58  |
|   Fold  |  12956  |  96.05  |  55/41  |  77.42  |
|  Split  |  12952  |  96.02  |  54/39  |  75.81  |
|     LV  |  13291  |  98.53  |  58/16  |  32.26  |
|     LF  |  13285  |  98.49  |  59/13  |  25.81  |
|     LS  |  13253  |  98.25  |  58/18  |  35.48  |
|     VF  |  13395  |  99.30  |  57/5   |  16.13  |
|     VS  |  13385  |  99.23  |  57/10  |  24.19  |
|     FS  |  13386  |  99.24  |  57/9   |  22.58  |
|    LVF  |  13443  |  99.66  |  59/3   |   9.68  |
|    LVS  |  13444  |  99.67  |  59/5   |  12.90  |
|    LFS  |  13440  |  99.64  |  59/3   |   9.68  |
|    VFS  |  13448  |  99.70  |  57/5   |  16.13  |
|   LVFS  |  13467  |  99.84  |  59/2   |   8.06  |
\-------------------------------------------------/

          /---------------------------------------\
          |            20,000 Sectors             |
          |---------------------------------------|
          |  Warps  | % Total | Bubbles | % Error |
/---------|---------|---------|---------|---------|
| Actual  |  53835  | 100.00  | 200/0   |   0.00  |
|  Level  |  38789  |  72.05  |  46/738 | 446.00  |
|    Var  |  52162  |  96.89  | 188/150 |  81.00  |
|   Fold  |  52155  |  96.88  | 188/147 |  79.50  |
|  Split  |  52191  |  96.95  | 194/145 |  75.50  |
|     LV  |  53125  |  98.68  | 192/59  |  33.50  |
|     LF  |  53116  |  98.66  | 193/46  |  26.50  |
|     LS  |  53084  |  98.60  | 196/55  |  29.50  |
|     VF  |  53503  |  99.38  | 193/18  |  12.50  |
|     VS  |  53492  |  99.36  | 196/28  |  16.00  |
|     FS  |  53486  |  99.35  | 196/22  |  13.00  |
|    LVF  |  53637  |  99.63  | 194/11  |   8.50  |
|    LVS  |  53633  |  99.62  | 196/10  |   7.00  |
|    LFS  |  53628  |  99.62  | 196/9   |   6.50  |
|    VFS  |  53683  |  99.72  | 196/4   |   4.00  |
|   LVFS  |  53722  |  99.79  | 196/4   |   4.00  |
\-------------------------------------------------/

The first column indicates how the information was acquired.  The actual
information is from a complete CIM sector listing.  The combinations of the

                                   Page 5
various ZTM methods are indicated by the first letter of their names in a
string, so LVS is a combination of Level, Var, and Split.

The warps column shows how many warps are in the universe, and were found
by the various methods, along with their combinations.  The bubbles column
shows the result of a /MIN:3 /MAX:25 /TUNNEL run of TWBFind.  The format is
valid/invalid.  The percent error for bubbles is the number of valid
bubbles not found, plus the number of invalid bubbles found, divided by the
actual number of valid bubbles.

You can clearly see that the Level method is fairly useless by itself, and
that the other three methods do indeed have the same basic level of
effectiveness.

You can also see that the best results are achieved by combining several of
the methods.  While the Level method alone is fairly poor, it does augment
the other methods fairly well.

You'll usually get good results by using Var, Fold, and Split together.
Though as the 5000-sector universe shows, the Level method can sometimes
provide a significant amount of additional information.

3)  CIM sector information - this file is also created by you, but is just
a simple capture of what the game spits out.  To make this file, turn the
capture mode of your comm program on, enter CIM mode (see below), and press
"I".  After the scrolling display of sector information has stopped, stop
the capture.  That capture file can be used as is by TWBFind.

4)  CIM port information - this is a capture of port information, which can
be made by pressing "R" in CIM.

To enter CIM mode in Tradewars 2002, go to the ship computer, and enter the
following sequence, using your numeric keypad (on the right side of the
keyboard):

ALT-200
ALT-201
ALT-202
ALT-203
ALT-204
ALT-205

If you didn't mess up, you'll be met with a plain looking ":" for a prompt.

Note that with some later versions of TW2002, you don't need to be at the
ship computer prompt, and can also just press "^" to enter CIM.

From here, you can enter the CIM commands necessary to get data.

================================================================================

IV. Running the Program

Once you've exported the sector data from the game, you're ready to run the 
program. 


                                   Page 6
The options are available by simply running the program without parameters, 
or with /h or /?, and are as follows:

/avoidunex - avoid unexplored sectors when finding bubbles
/bench - report bubble search time
/bogus - find bogus bubbles (one-way sectors in)
/eatmem - eat up memory to increase speed during search
/noexpset - don't set as explored any non-binary input
/q - suppress progress output, and only report results
/warps - display total warps, and number added by input files
/tunnel - use only one bubble route from primary entrance
/update - update binary database only - no bubble search
/ports - show ports in bubble report
/fromsd - calculate distance from stardock
/fromc0 - calculate distance from class 0 ports
/fromterra - calculate distance from Terra
/from:# - calculate distance from sector
/min:# - minimum number of sectors in bubble; default 5
/max:# - maximum number of sectors in bubble; default 50
/maxent:# - maximum number of entrances for bubbles (1 to 50); default 1
/entmult:# - ent multiplier for min bubble size (2.0 to 1000.0); default 2.0
/unisize:# - number of sectors in universe (100 to 65535); default 1000
/maxbub:# - reserve storage for bubbles (1 to 4.3 billion); default 1024
/stardock:# - specify stardock sector
/class0:# - specify second or third class 0 sector
/zap:# - remove port from binary data (destroyed, etc.)
/pclass:# - priority class (1 to 4); default=2
/pdelta:# - priority delta (0 to 31); default=0
/sectfile:<filename> - CIM sector data file
/plotfile:<filename> - capture of CIM course plots (zero-turn mapping)
/binfile:<filename> - binary sector database file
/auxbinfile:<filename> - second binary database to combine info with
/portfile:<filename> - CIM port data file
/outfile:<filename> - report file; default 'bubbles.lst'
/cimoutfile:<filename> - write CIM-style data to file

Detailed explanations:

Option /avoidunex -  This tells the program to consider all unexplored
sectors to be unreliable, whether or not any outwarps are defined for them
(from CIM course plots).  The default behavior is to consider a sector's
information to be reliable for the purposes of finding bubbles, if it has
at least one outwarp defined.  This is to utilize CIM course plot data for
zero-turn mapping.  Since you can store this information in a binary
database, this option allows you to do only bubble searches with explored
sectors, to get more accurate results (searches based on zero-turn mapping
data are potentially unreliable - depending on what methods you use).

Option /bench -   When this is used, the program will time the bubble search
(and only the bubble searchf), and report how long it took.

Option /bogus -   Use this switch to find only bogus bubbles, which appear
good from the inside, but have a one-way sector leading into them from
behind the entrance.  This data will be useful for finding enemies who
weren't very careful about checking their bubble.  Those with the one-way
entrance far back in the bubble will be especially good, since the

                                   Page 7
least-defended planets will likely be in those "safer" sectors.  When this
option is used, *only* bogus bubbles will be found and reported.  The
bubble data file will clearly mark them as such.

Option /eatmem -  This switch tells the program to use fixed-length arrays
for bubble data, rather than dynamically extended ones.  That is, instead
of expanding the arrays as necessary to add new sectors and entrances,
enough space to accomodate the maximum number of sectors and entrances for
the current search is allocated when the bubble is initialized.  This makes
the search operation faster, at the expense of requiring more memory during
operation.  With /M3 /X100 /E4 /Z3 on a 5000-sector universe (502 potential
bubbles, 172 actual), the result is 116KB more memory required, with a 20%
increase in speed.  This option should be used cautiously with the DOS
version.

Option /noexpset -  This tells the program to treat all forms of non-binary
input as unreliable - sectors are not marked as explored, even though a CIM
sector file would normally imply this.  This is for the possibility of
someone providing you with a CIM sector display, which doesn't match what
you've actually explored.  This allows the /avoidunex option to function
normally.

Option /q -  This sets quiet mode, which suppresses iterative progress
displays (things which have a ':' followed by a sector count, for example).
Instead, a simple string is printed to show what's being done.

Option /warps -  This tells the program to display the number of known warps
after reading each input file, and for all other than the normal binary
file, a count of how many warps were added is also printed.  This is
primarily useful for keeping track of how effective various ZTM algorithms
are.

Option /tunnel -  When this option is used, the program doesn't consider all
combinations of the bubble entrance's warps as either ways back to the
universe, or parts of the potential bubble.  Instead, it will consider only 
one warp at a time as part of the potential bubble, with all other sectors 
as ways back to the universe.  This reduces the number of iterations, 
decreasing runtime, and also prevents the situation described in the second 
note on program operation.

Option /update -  Use this to avoid doing a bubble search, and only update
the specified binary sector database with new information, from either a
standard CIM sector info file, a CIM course plot capture, or a CIM port
info file.

Option /ports -  With this option, the program will show any known ports in
the sector list of the bubble output.  If both the binary file and a CIM
port file are used as input, the program will check for ports read from the
former, but not the latter.  Any such ports, which are either blocked or
destroyed, will be listed with a '*' next to them.  This may be an
indication that an enemy has moved into a bubble and deployed fighters.

Option /fromsd -  This tells the program to calculate the distance between
the bubble's primary entrance and Stardock, in both directions.  If the
location of Stardock isn't know, this option is ignored.  Stardock's
location is specified by the /stardock option, and also kept track of in

                                   Page 8
the binary database.

Option /fromc0 -  This tells the program to calculate the distance between
the bubble's primary entrance and any known class 0 ports (not including
sector 1 - see /fromterra).  The location of class 0 ports is specified by
the /class0 option, and also kept track of in the binary database.

Option /fromterra -  This tells the program to calculate the distance
between the bubble's primary entrance and Terra (assumed to be sector 1).

Option /from:# -  This allows you to specify a specific sector for distance
calculation.  The distances calculated will be between this sector and the
bubble's primary entrance.

Option /min:# -  This specifies the minimum number of sectors that a bubble
must contain to be saved and reported.  When dead ends have been reached 
while traversing the potential bubble, if the number of sectors found 
(minus the number of entrances) is less than this number, the bubble is 
scrapped.  The default value is 5 sectors.

Option /max:# -  This specifies the maximum number of sectors to traverse
while searching for a bubble, before giving up and assuming that it's a
lost cause.  If the number of sectors in a bubble (not counting the
entrances) exceeds this number before all dead ends are reached, the
potential bubble is scrapped.  The larger the number, the more potential
bubbles you can find, but the longer the program takes to run.  Using the
default will probably be fine for any standard game.  With Gold games, you
should try a larger number, with multiple entrances, to find the bubble
structures that the Gold-enabled Big Bang can be configured to create.  The
largest such structure can be up to 19900 sectors, in a 20000-sector
universe with one bubble allowed, so consider that when choosing this
number in a Gold game. The default value is 50 sectors.

Option /maxent:# -  This specifies the number of entrances that the bubble
may have.  There's no theoretical limit (that's less than the number of
sectors, anyway), but practicality prompted me to put in a somewhat 
arbitrary limit of 50 entrances.  This will allow one to locate the large 
bubble structures in Gold games, which can have many entrances.  The 
default value is 1 entrance.

Option /entmult:# -  This specifies how many sectors a bubble must contain
relative to the number of entrances for it to be saved.  If the current
bubble can be saved with 3 entrances, for example, and the value for this
option is set to 3, then the bubble must have at least nine non-entrance
sectors to actually be saved.  The default and minimum value is 2.  The
maximum value is 1000 (which would, say, require 3000 sectors for a
3-entrance bubble). Fractional numbers are allowed, so you can specify 2.5,
7.2, etc.

Option /unisize:# -  This specifies the number of sectors in the universe.
Here is where the amount of storage set aside for sector information is
determined. If the CIM file contains information for a sector number
greater than this value, the program will abort.  The maximum value that
the DOS version will support is 16382.  The OS/2 and DPMI versions support
up to 65535 sectors (currently, TW Gold supports only 20000 sectors).  The
default value is 1000 sectors.

                                   Page 9

Option /maxbub:# -  Use this to specify a maximum number of bubbles other
than the default of 1024.  This number represents the maximum number of
potential bubbles that will be found before the search is terminated, and
those found trimmed and reported.  How much memory is reserved depends on
the memory model of your system.  The included executables all use 4 bytes
to store a memory pointer, so the default value reserves 4KB of memory.  A
64-bit flat addressing model would reserve 8 bytes per bubble, or 8KB of
memory. Memory-constrained DOS systems may want to specify a value lower
than 1024 here, to maximize the amount of memory available for the search.
For a search with a 5000-sector universe using /M3, with all else default,
256 would be a safe value (which uses 3KB of memory less than the default).

Option /stardock:# -  Use this to specify the Stardock sector for the
universe.  It will be saved if you specify a binary file.  If you make a
mistake, just use this option again, and the location of Stardock will be
updated in the binary file.

Option /class0:# -  Us this to specify the location of a class 0 port.
This option can be used twice on the same command line.  It overrides any
class 0 ports stored in the binary database.

Option /zap:# -  Use this to remove ports from the binary database.  You'd
want to do this when a port is destroyed, or permanently blocked (by an
ally, or an extremely powerful enemy, etc.).  This option can be used any
number of times on the same command line.

Option /pclass:# -  This is specific to the OS/2 version, and specifies the
priority class that the program runs in while searching for bubbles.  
Allowable values are integers from 1 to 4, where 1 denotes idle-class, 2 
denotes standard class, 3 denotes time-critical class, and 4 denotes 
foreground-server class.  The order of precedence, from lowest priority to 
highest, is 1-2-4-3.  This is only put into effect after the sector 
information has been loaded, and the bubble search has begun.  The idea 
here was to be able to run the program at idle priority in the background 
for a long search (for very large bubbles), while doing other things.  If 
you specify a value of 3, which is the highest priority, then a timeslice 
will be forfeited every 100 sectors, to allow you the chance to abort it 
(otherwise, keyboard input won't be processed in a timely fashion).  The 
result is that the program will run somewhat slower at time-critical 
priority than otherwise, unless there are other high-priority programs that 
are being unfriendly to the CPU.  The default value is 2.

Option /pdelta:# -  This is specific to the OS/2 version, and specifies the
priority delta value.  This is a subdivision of the priority class, with 
valid values from 0 to 31.  Giving a value above 0 will put the priority
slightly above other such programs, making this program run somewhat faster
if other CPU-intensive tasks are running.  The default value is 0.

Option /sectfile:<filename> -  This specifies the name of the file which
contains the CIM data capture.  Either this option, or one of the following
two options must be used.

Option /plotfile:<filename> -  This specifies the name of the CIM course
plot file, which is a capture of any number of course plots you made with a
comm program script.  This is useful for finding potential bubbles before

                                   Page 10
actually exploring, to get a head start in the game.

Option /binfile:<filename> -  This specifies the name of the binary
database file, which will be created after reading other sector information
in, and read from if it exists beforehand.  This is always optional, though
it's a good idea to use.  If no database exists, and you don't specify
other methods of reading sector information, the program will abort with an
error.

Option /auxbinfile:<filename> -  This specifies the name of an auxiliary
binary database file, which will have its contents read in, combined with
existing data, and written out to the primary binary database file.  That
is, if you use it as it was designed.  Since it's easier to deal with this
way, you can also use this simply as an additional input file format,
without specifying a binary database to which changes should be made.  The
main usage you should have for this is in combination with the /U switch,
to combine two databases from different sources (CIM sector data, one or
more CIM course plots).

Option /portfile:<filename> -  This specifies the name of a CIM port info
file.  This is entirely optional, but you must use this to acquire port
information if you want meangingful output with the /ports option.  The
ports are stored in the binary file, but you won't see indications of
blocked ports without using this.

Option /outfile:<filename> -  This specifies the name of the file which the
found bubbles will be written to.  The specified file will be overwritten
if it exists.  You may also use anything which the operating system can
handle as a file, such as a named pipe, communications port, network
printer name, etc.  The default is "bubbles.lst".

Option /cimoutfile:<filename> -  This specifies the name of a file to which
CIM-style sector information will be written.  If you have another helper
program which doesn't know how to deal with ZTM info (course plots), you
can read them in with this program, and output a format that the other
helper knows how to read.

There is no space between the switch and the option data, so assuming a 
sector data file of "sectors.txt", you might do the following:

twbfind.exe /sectfile:sectors.txt /min:4 /max:20 /unisize:5000

This would read in the sector data from "sectors.txt", search for bubbles 
with a minimum of 4 sectors, and a maximum of 20 sectors, then write them 
to "bubbles.lst".

=============================================================================

V. Output

Since all output is accomplished via stream I/O, you can use anything that 
the underlying operating system can open as a file for output.  This 
includes I/O devices, such as LPT1, CON, etc.

The format of the output is this:


                                   Page 11
12-sector bubble with entrance at 2381.

Sectors in bubble:
 4051   4230   3764   2107   847   2524   2657   2683   3579   3759  
 1740   3936  

Sector  2381  -     110    1818    4051    4230  
Sector  4051  -  { 2381}  3764  
Sector  4230  -  { 2381}
Sector  3764  -    2107    4051  
Sector  2107  -     847    3764  
Sector   847  -    2107    2524    2657    2683  
Sector  2524  -     847  
Sector  2657  -     847    3579  
Sector  2683  -     847    3759  
Sector  3579  -    1740    2657  
Sector  3759  -    2683    3936  
Sector  1740  -    3579  
Sector  3936  -    3759  

--------------------------------------------------------------------------------

11-sector bubble with 2 entrances at:
  4310  3901

Sectors in bubble:
 4374   91   1846   4258   4309   3537   3230   919   2314   4505  
 3186  

Sector  4310  -  < 3975>   3439    4134    4374  
Sector  4374  -      91    1846    4258    4309  { 4310}
Sector    91  -    4374  
Sector  1846  -    3537    4374  
Sector  4258  -    3230    4374  
Sector  4309  -     919    2314    4374  
Sector  3537  -    1846  
Sector  3230  -    4258    4505  
Sector   919  -    4309  
Sector  2314  -    3186    4309  
Sector  4505  -    3230  
Sector  3186  -    2314  { 3901}
Sector  3901  -  < 3873>    623     660    1545    1969    3186  

--------------------------------------------------------------------------------

The first line says how many sectors are in the bubble, and if there's one 
entrance, displays that.  If there is more than one entrance, they are 
displayed on the second line, wrapping as needed to additional lines.

After that, the sectors in the bubble are displayed, wrapping as needed, in 
summary.

The next part is a list of all sector bubbles (including entrances, this 
time), and their warps.

Sectors surrounded by <>'s denote one-way in-warps.

                                   Page 12

Sectors surrounded by ()'s denote one-way out-warps.

Sectors surrounded by []'s denote unexplored sectors.

Sectors surrounded by {}'s denote bubble entrances.

Unembellished sectors are both out- and in-warps.

If you use one of the /from* options, the information will appear at the
top of the bubble info, and look like this:

Distances:  Terra=2/2  Stardock(2478)=9/9

The numbers are in the order of to/from.  So in the above, the distance
from the bubble's primary entrance to Stardock, and back again, is 9 hops.

If you use the /ports option, the sector list will look like this:

Sector  381         -   3013   4455
Sector 3013  (BSB)  -    381   1425   4925

If the port is blocked, there'd be an asterik to the left of the '('.

I've determined that properly displaying all the possible bubbles is 
virtually impossible with stream I/O, so a formatted display would require 
drawing a bitmap of sorts.  I might write such a thing, if I can figure out 
how, and use the data above to do it.  Anyone else might want to try the 
same thing.

Short of that, drawing on a piece of paper and connecting the lines is 
fairly easy given the above.  Those using TW Helper can do a /G command 
for a sector in the middle of the bubble to get a nice graphical layout.  
TWAR for Windows has a similar mode (currently at alpha level), and TWATS 
may very well have one as well.

For the smaller bubbles, that is.  I have found a bubble as large as 198 
sectors, with nine entrances, from a 20000-sector Gold game. Short of a 
graphical scrolling display, getting a visual grip on that one will be 
nearly impossible.

Note:  With the new feature of finding potential bubbles based on zero-turn
mapping data (CIM course plots), the bubble display will likely be riddled
with sectors surrounded by []'s, denoting that they are unexplored.  This
is normal, and it does in fact mean that the bubble is potentially
unreliable.  Since those sectors aren't actually explored, the information
about their outwarps is not guaranteed to be complete.  All it takes is one
unknown outwarp to completely kill a bubble.

Note:  If you use the /F switch to search for fake bubbles, the first line
will look instead like this:

12-sector bogus bubble with entrance at 2381.

When you see that header, there will necessarily be a sector in the bubble
with a warp shown with <>'s around it, denoting the sector number which has

                                   Page 13
a one-way warp into the bubble, behind the entrance.

=============================================================================

VI. Limitations

The program was written to accommodate up to 65,535 sectors, since the Gold 
version of TW currently supports up to 20,000.  In the unlikely event that
the game ever exceeds 65,535 sectors, it would not be very difficult to
change datatypes to long integers to increase capacity (to over 4 billion
sectors - though obviously a substantial machine would be required for
this).

The 16-bit DOS version has an absolute sector limit of 16382.  Whether it 
will run to completion depends on the available memory, the search 
parameters, and the number of bubbles in the universe matching those 
parameters.

The DPMI, OS/2, Linux, and Win32 executables will handle the program design
limits.

=============================================================================

VII. Performance

Over the course of this program's development, I've gained a lot of
experience as a programmer.  As a result, this program is reasonably
efficient by this point, though there's almost certainly room for
improvement here and there.  On fast machines, you won't notice.

The program is processor-bound, so your memory and disk subsystems are 
insignificant.  Your video subsystem may also have an impact, especially if 
running in a windowed session of whatever operating system you're using.  
Time the execution with output redirected to nul, and you'll see the 
difference, or use the /Q option, which suppresses most of the output.

The greater the maximum number of sectors, of course, the longer it will 
take to run.  The greater the number of entrances to search for, the longer 
it will take to run.  And less obviously, the more information about the
universe you have, the longer it will take to run.

The /eatmem option will speed things up by about 20%, at the expense of
some additional memory (on the order of 100KB).  The /tunnel option reduces
the number of bubbles found (see notes on program behavior), but also
speeds things up considerably.

================================================================================

VIII. Notes on Program Behavior

1)  The program does consider one-way exits into explored sectors the end of 
a bubble extent.  It's conceivable that such outgoing warps lead to more 
isolated sectors, something like this:

             [sector]
                |

                                   Page 14
[entrance] - [sector] - [sector] - [sector] -> [sector] 
                                                  |
                                               [sector] -> [universe]

Where -> denotes a one-way warp.  

In this case, that section between the two one-way warps would not be 
counted as part of the bubble.  However, provided there were enough such 
sectors for the given parameters, they would be part of a separate bubble 
beginning with the sector after the first one-way warp.  

I personally think that since those sectors have no path back into the 
bubble except through the universe, they shouldn't be considered part of it 
anyway.  If they did have a way back into the bubble, they'd be found and 
counted in the first place, because one-way inwarps are added to the 
bubble.

2)  Due to the way the program works, the following is considered a bubble:

           [universe]
               |
[sector] - [entrance] - [sector] - [sector] - [sector]
               |
           [universe]

Where the first sector is a single dead-end off of the entrance.  I'd 
personally not consider it part of the bubble, but I'll leave that to the 
user to decide.  The issue becomes very complex when considering the 
programmatic exclusion of such constructs.

When more than one entrance is tested for, the degree of this convolution 
can increase.  All that can be guaranteed is that all sectors reported as 
in the bubble are isolated from the universe by the entrances.

Note:  This is not applicable when using the /t option, which will not 
allow bubble sectors to be separated by the primary entrance.

3)  When using standard CIM sector information, the bubble search bases
whether or not a sector is reliable only on whether or not it's explored.
As a result, the less data you have, the less bubbles you find.  But when
you're using course plot information, the search algorithm has to consider
as reliable any sector which has at least one outwarp defined for it.
Because of this, the opposite effect is true - the less dat, the more
bubbles you find.  The more faulty bubbles, that is.

If the search is at a point where only one sector is left to check, and it
has three outwarps defined, all of which lead to dead ends, you'll have a
bubble (provided it meets the search criteria).  But if that sector has an
outwarp that wasn't discovered during the course plot session, it'll be
just another link in the universe.

Even more so than usual, you'll need to check the reality of any found
bubbles by exploring all the sectors involved, and using avoids to check
for one-way sectors leading into the bubble.

See the section on input file formats for details on how to maximize the

                                   Page 15
reliability of your course plots.

4)  The fake bubble option makes a fairly simple change to the existing
algorithm:  Instead of adding one-way inwarps to the bubble, they are
excluded, and the bubble is marked as potentially bogus.  When it comes
time to save the bubble, any which aren't so marked are discarded.  Then if
the bubble has at least one sector with at least one inwarp that is oneway,
and not coming from a sector that's inside the bubble, it's saved.
The nature of the program is to consider as an entrance the sector(s)
having one or more connections to the outside universe.  Some people might
consider the first sector with no connection to the universe, except via
the entrance(s), to be the entrance, where the front-line defenses are
placed.  Some bogus bubbles may have the one-way entrance leading to this
sector, which gives you no advantage if the person has the defenses here. I
myself tend to use that strategy, so that someone doesn't run into my
defense except if plotting a course into my bubble.  If you put your
defenses in the entrance sector(s), someone could run into them with a
normal course plot.  It's because so many people don't care about that, and
put their defenses in the entrance(s) that bubbles are still considered
bogus when the one-way entrance leads to the first non-entrance sector(s).

5)  When calculating the distances from the primary bubble entrance to
another sector, the algorithm used will exactly match the route chosen by
TW, but only if you have complete information about all sectors on the
actual route.  If you're missing warp information, you may have a longer
route than necessary, or simply not be able to find a route at all.  With a
good set of course plots (see the section on input files), the routes will
be accurate about 98% of the time.  If the universe is 100% explored, the
routes will be correct 100% of the time, so long as you don't have avoids
set in the game.

=============================================================================

IX. How it Works

Here's the basic logic of how it works:

1)  For each sector, treat one or more outgoing warps as potential parts of 
a bubble with the current sector as an entrance.  Treat the rest of the 
outgoing warps as the way back to the rest of the universe.

2)  Each possible combination of the situation in 1 should be tested.  The 
number of combinations for N outgoing warps is 2 to the Nth power minus 2 
(the two cases where either all sectors would be potential bubble members, 
or all sectors the way back to the universe).  If the /t option is used, 
then the number of combinations is simply N, as only one outgoing warp is
considered part of the potential bubble.

3)  Put each potential bubble sector into a bubble structure.

4)  Loop through the bubble structure, adding all non-oneway outgoing and 
oneway incoming warps to the end of it (note:  the /f option ignores
one-way incoming warps to find bogus bubbles).  If multiple entrances are
being checked for, then whenever the bubble cursor exceeds the minimum
number of sectors, and the remaining sectors in the bubble do not exceed
the maximum number of entrances, the bubble is added.  This will make for a

                                   Page 16
lot of extra bubbles, that are later trimmed out.  The alternative approach
is to do an outer loop for each entrance, which takes hours for two
entrances, and hundreds of thousands of years for five entrances.  This
method takes barely more than single-entrance searches.  Starting with
versions 0.14, the difference is really minimal.  With /E5 /Z5 tacked onto
the end of a /T /X150 search, the run speed increased only by about 5%
(using the /G switch reduces the increase even further).

5)  If the number of sectors in the structure exceed the maximum number of 
bubble sectors to search for, abort this iteration, and go on with the next 
possible bubble structure based on the current sector.

6)  If the end of the bubble structure is reached before the max sectors 
are exceeded, that means dead-ends have been reached at all fronts, and the 
bubble is indeed isolated from the universe by the entrance.  If the number 
of sectors is larger than the minimum sectors indicated, save the bubble.

7)  After all sectors have been processed, go through all the saved 
bubbles, and eliminate any duplicates and those which are part of larger 
bubble structures.

8)  Print out the information (currently in a lame format).

================================================================================

X. Compiling the Program

The source for the program is included in the 'source.zip' archive.

The language is C++, and I've tried to make the program cross-platform, 
though it may or may not be necessary to make some small modifications 
before it will compile for you. 

There are a few defines in place that were necessary to make it compile 
under the compilers I've been using.  These may or may not need to be 
removed or altered to allow compilation on other compilers, for other 
platforms.

There's nothing sneaky going on, so just compile each .cpp file, and link 
all the objects together as one executable.  I keep separate sources for 
ease of editing.  There are makefiles for all the compilers that I used 
included.

The five included executables are:

twbfos2.exe  - 32-bit OS/2 VIO executable 
twbfdpmi.exe - 32-bit DPMI executable
twbflin      - 32-bit Linux ELF executable
twbfw32.exe  - 32-bit Win32 console executable
twbfdos.exe  - 16-bit DOS executable (8086 instruction set)

The first was compiled with IBM VisualAge C++ 3.08 for OS/2, the second 
with the DJGPP package (a DOS port of GCC), the third with EGCS, the fourth
with Mingw32/EGCS, and the fifth with an old copy of Borland C++ 3.0 for
DOS.


                                   Page 17
=============================================================================

XI. Author Information

The "I" all through the rest of this file refers to me, Mike Ruskai, and I 
can be reached via e-mail to thannymeister@yahoo.com, should you have a 
question, suggestion, or problem report.

=============================================================================

XII. History

10-11-98 - Initial 0.01 release.

10-13-98 - Forgot to check one memory allocation pointer, so it would crash 
           if the allocation failed.  Fixed this.

10-17-98 - Reorganized the source, putting stuff from main() into objects,
           to better facilitate any future expansion efforts.  Altered the
           output format to report all data concerning the connectivity of
           the bubble sectors, though it's not formatted to be visually
           meaningful.  Fixed an obscure glitch that would prevent some
           bubbles from being reported, due to a false assumption I made
           earlier.  Release this as version 0.02.

10-19-98 - Put a check in for duplicate sector data, so if you accidentally
           append a sector report to an existing one, instead of 
           overwriting it, the program will not get screwed up.  Release
           this as version 0.03.

10-20-98 - Fixed the comparisons to properly reflect minimum and maximum 
           sectors in bubbles.  Version 0.04, just to have a number.

10-22-98 - Got a hold of the DJGPP compiler, so we now have a DPMI 
           executable in the archive, that will handle any amount of 
           sectors.  A few minor source changes, nothing that affects
           functionality.  Call this version 0.05.

10-23-98 - Fixed stupid memory leak.  The DOS version should work OK with
           any maximum bubble size value for 5000 sectors, now, though it's 
           pretty slow doing it.  Made some source changes that should
           hopefully make it easier to compiler on other platforms.  We'll 
           call this version 0.06.

11-30-98 - Added the ability to abort a search and have all bubbles found 
           to that point trimmed and saved by pressing the space bar.

           Added priority control for the OS/2 version.  This makes it 
           possible to cripple your machine fairly well, by specifying
           priority class 3 (time critical).  Added a DosSleep() call every
           100 sectors if that's the case, so that the program can be 
           aborted.  The effect is that running with time critical will be
           slower than normal priority, unless you've a lot of 
           high-priority programs already hogging the system, in which case
           time critical priority will allow it to run normally.  The given
           priority is not set until the bubble search commences.  The 

                                   Page 18
           loading of warp information, and setting of one-way warps takes
           place at normal priority.  The idea there is to put a long 
           search at idle priority, to run in the background (/p1 /d31 is 
           the recommended setting for this).

           Added the ability to search for multiple-entrance bubbles.  This
           was inspired by the feature in TW Gold of explicitly creating
           bubbles at Big Bang, which could have several entrances.  
           There's no theoretical limit to the number of entrances that can 
           be searched for, but I've put an arbitrary limit of 50 on the 
           program, since anything above that is of extremely dubious 
           utility.

           Modified the output format to wrap lines, instead of putting 
           sectors on one long line.  

12-05-98   Gave up on the idea of including a Win32 console executable, 
           since it (compiled with EGCS using Mingw32) was too bulky and 
           slow to be of any use.  This is version 0.07.

01-08-99   Added the /t option to alter the way bubbles are searched for.
           Instead of considering all possible combinations of warps as 
           either ways back to the universe or parts of the potential 
           bubble, only one warp at a time is considered part of the 
           potential bubble.  This makes runtime a little bit faster, and 
           eliminates the possibility of finding bubbles that are split by
           the primary entrance, as explained in the second note on program 
           behavior.  This is version 0.08.

01-11-99   Changed the way sector data is stored so that the DOS version 
           now has a sector limit of 16382, instead of 5460.  Version 0.09
           will be the moniker for this puppy.

01-29-99   Replaced iterative loops with memcpy() calls, after realizing 
           that using the same pointers is not a problem.  This speeds 
           things up quite a bit when doing large searches.

           Changed the multiple-entrance support so that it wouldn't bother
           saving bubbles that didn't have twice as many sectors as the 
           number of entrances.  Then I added the /z parameter to allow the 
           user to define the multiplier (minimum is 2, maximum is 1000).  
           The result, even with just a multiplier of 2, is dramatically 
           faster multiple-entrance searches, with less garbage bubbles.  
           Setting this to a value of 10 or more will make sure that only 
           viable multiple-entrance bubbles are found, such as 20-sector 
           bubbles with 2 entrances, or 100-sector bubbles with 10 
           entrances; each being the equivalent of a single-entrance 
           10-sector bubble in terms of how much is protected per defended 
           sector.

           This is version 0.10.

03-11-99   Did a bit of optimizing, primarily by inlining small functions.

           Added file read/write error checking, just in case.


                                   Page 19
           This is version 0.11.

08-17-99   No real changes.  Just updating the e-mail address.

           Nevertheless, this is 0.12.

01-28-00   At the suggestion of a program user, added the ability to read
           in data captured by the user, doing CIM course plots.  This
           allows a pseudo-exploration of the universe for the purpose of
           finding likely bubbles to be actually explored.  The /W option
           is used to specify the capture file.

           Along with this, I added a binary sector database option, which
           will keep track of the outgoing warps that have been discovered
           using one or many CIM course plot sessions.  It will also keep
           track of whether or not the sectors are actually explored.  The
           /B option is used to specify this file.

           Added the /U option to allow updating the binary database
           without performing a bubble search.

           Added the /A option to use the old program behavior of avoiding
           unexplored sectors when searching for bubbles.  This allows you
           to use a binary database containing warp information for
           unexplored sectors without getting possibly unreliable search
           results.  Obviously, this is for when you've done actual
           exploration, and aren't trying to find potential bubbles using
           CIM course plots.

           The OS/2 executable is now compatible with only OS/2 Warp and
           above, because the code is compressed as well as the data.  In
           the unlikely event that someone with OS/2 2.x wants to run this
           program, they can uncompress and recompress it using the lxLite
           utility (use the /c:ver2x switch).

           Made a few changes necessary to compile a Linux version.  The
           Linux executable is different from the others in one respect - a
           keypress (to abort and save whatever bubbles have been found) is
           only checked for every 50 sectors, because the method of
           determining whether there's a keypress waiting or not in Linux
           is very slow.

           This is version 0.13.

02-03-00   Added ability to read a second binary file, for the purposes of
           combining the information contained in them.  This file is
           specified with the /C switch.  Though the program will simply
           treat it as another input file, the intended usage is something
           like this:

                twbfind /n5000 /bgame.dat /cauxdata.dat /u

           That would read in game.dat, then read in auxdata.dat, and write
           the information collected from both back to game.dat.  Two
           conditions will clue you in that the two data files are from
           different universes:

                                   Page 20

                1)  You get an error saying more than 6 warps were found
                    for a given sector.

                2)  You get an error about an integer being too large while
                    reading the second binary file.

           If using the new binary formt (see next comment), the size of
           the universe is stored at the front of the file, and any
           difference will be noted immediately.

           Changed the binary format somewhat, to allow for a 16-byte
           header at the beginning of the file.  The header contains a
           64-bit signature, to verify during read that the file is valid;
           the major and minor version level of the release of TWBFind that
           created the file, to allow for future changes; the size of the
           universe; and some reserved space for future additions.  The
           data itself is also altered a bit (see the options section for
           the details of how it's stored), to the point where adding the
           ability to read the old files is too much of a pain.  I created
           a conversion program, which can be downloaded in a separate
           archive (twbfcvt.zip), that will convert from the old to the
           new format.  With the new format, any time you specify an
           existing binary file as input, you don't need to specify the
           number of sectors in the universe on the command line (if you
           do, it will be overridden by the info in the binary file).

           In a trend that's sure to continue, I ripped off a neat idea
           from a program called TW Guru (the author of which is nowhere to
           be found).  If you use the /F switch, you will find fake bubbles
           instead of real ones.  That is, a bubble which appears kosher
           from the inside, but has a one-way sector going into it behind
           the entrance.  This function will be very useful for finding
           people who aren't scrupulous about checking the validity of the
           bubble(s) they find.  The best ones have the one-way entrance
           towards the back, where the least defended planets will likely
           be.

           I made two performance enhancements.  The first one was to
           change the bubbleset from a dynamically expanding array to a
           fixed-length array, the length of which is determined at run
           time.  The default length is 1024 bubbles, which is enough for
           any reasonable search.  This can be overridden with the /R
           switch.  For the included executables, the memory required is
           4 bytes per bubble (the size of a memory pointer).  The
           fixed-length array helps, of course, be preventing the need to
           allocate new memory, and copy all the bubbles to the new
           location, each time one was found and added.

           The second performance enhancement is only in effect when you
           use the /G switch, which tells the program to gobble up more
           memory to run faster.  When this is done, new bubble objects
           don't use dynamically sized arrays, but instead allocate enough
           memory at initialization to contain the largest allowable
           bubble (determined by /X and /E).  This negates the need to
           allocate memory and copy over data when adding sectors and

                                   Page 21
           entrances.  With search parameters of /M3 /X100 /E4 /Z3 on a
           5000-sector universe, the result was an additional 116KB of
           memory required during operation, and a 20% reduction in search
           time.  With /T /X25, the increase in memory requirements wasn't
           detectable, but the run time went down by over 30%, so this
           option is useful for even small searches on memory-tight
           machines.

           The DOS, DPMI, and Linux executables are now compressed with
           UPX.  This improves on PKLITE and DJP for the first two,
           respectively.

           I added a Win32 executable, using Mingw32/EGCS, which has
           improved quite a bit since I last tried it.  The executable
           speed is pretty good, even though I can't get rid of the
           annoying mouse cursor, or even the text cursor (the standard
           Win32 calls to do the latter simply don't work).  UPX supports
           PE compression, but a program called EZIP did a much better job
           (less than half the size of UPX's compressed EXE).  A bug
           somewhere in Mingw32 package (or perhaps Win95) causes the
           program to crash when you press CTRL-C (presumably at exit()).

           This is version 0.14.

02-18-00   I was running out of single-letter options, and those that were
           left didn't match the new options very well, so I redid the
           parameter system.  All options now have the following form:

                </|-><optname>[:<optvalue>]

           All existing options now have new names.  See the options
           section for details.

           Added the /Q option to suppress iterative progress output.
           Only simple strings will be output to indicate what task is
           being performed.

           Added the /BENCH option to keep track of and report how much
           time the bubble search took.

           Added the /NOEXPSET option to prevent the program from setting
           the explored status of any sector while reading in data from
           any source.  If you're importing the data from a CIM file that
           was generated by someone else, and therefore doesn't reflect
           sectors that you've actually explored, this will prevent the
           program from assuming that the sectors were explored, and
           storing them as such in the binary database.

           Added the /WARPS option, to display the total number of
           warps known after each input file is read, and report on how
           many new ones were discovered.  Useful for evaluating the
           effectiveness of ZTM plot algorithms.

           Added the /CIMOUTFILE:<filename> option, which generates a
           sector listing in the style of the "I" function in CIM.  When
           used in conjunction with the /AVOIDUNEX option, only those

                                   Page 22
           sectors marked as explored will be written.

           I added support for storing port information in the binary
           database file.  Only the type of port is stored, and it's stored
           in what was a reserved byte, so there's no incompatibility
           issues (v0.14 databases will just appear to have no ports).

           Added the /PORTFILE:<filename> option to read a CIM port
           capture.  Ports are stored in the binary database, if it's
           specified.

           Added the /PORTS option to display known ports alonside the
           sector list in a bubble printout.  When using both a binary
           database and CIM port file for input, ports which are read
           from the former, but not the latter, will be marked by an
           asterik.  These ports are blocked or destroyed, and in the
           former case may indicate where an enemy has decided to make
           a new home.

           Added the /ZAP:# option to specify sectors for which a port
           should be removed from the database.  You'd use this if a port
           were destroyed, or is permanently blocked.  This option may be
           used any number of times on the same command line.

           Added the /STARDOCK:# option to specify the location of
           Stardock, which will be stored in the binary database.

           Added the /CLASS0:# option to specify the location of up to
           two class 0 ports, which will be stored in the binary database.

           The previous two new options are for use in conjunction with a
           new feature, which is to figure out out far the primary entrance
           of a bubble is from certain sectors.  A standard course plot is
           done, which will exactly match that of the game, provided you
           have complete information about the universe.  If your
           information is complete, you will either get an incorrect figure
           for the distances, or a question mark, for where a course didn't
           exist.

           Added the /FROMSD option to indicate that bubbles should have
           their distance to and from Stardock calculated.  If the location
           of Stardock isn't specified (either by the /STARDOCK:# option,
           or from the binary database), this option will be ignored.

           Added the /FROMTERRA option for calculating the distance to and
           from Terra (which is assumed to be sector 1).

           Added the /FROMC0 option for calculating the distance to and
           from the known class 0 ports (other than sector 1).  If they
           aren't specified with the /CLASS0:# option, or in the database,
           this option is ignored.

           Added the /FROM:# option for calculating the distance to and
           from a specific sector.  Only one such sector per run is
           allowed.  Maybe I'll change this later.


                                   Page 23
           Fixed CIM reading in Linux executable (had to put a CR
           check/strip in).

           That's enough new stuff for now.  This is version 0.15.





















































                                   Page 25
