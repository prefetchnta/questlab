PROJECT=jump_cui
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2019_x64.mak"

OBJ_LIST=.$(L)jump_cui.$(O) \
         .$(L)toolbox.res

SRC_LIST=.$(L)jump_cui.$(P)

build_all:
    $(CC) $(CFLAGS) $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)x64bin$(L)
    del /Q *.obj
    del /Q 2.manifest
