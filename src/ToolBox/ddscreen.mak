PROJECT=ddscreen
SUBSYSTEM=CONSOLE
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2022_x64.mak"

OBJ_LIST=.$(L)ddscreen.$(O) \
         .$(L)toolbox.res

SRC_LIST=.$(L)ddscreen.$(P)

build_all:
    $(CC) $(CFLAGS) /I "..\..\inc\x64inc\labai" $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)x64bin$(L)
    del /Q *.obj
    del /Q 2.manifest
