PROJECT=ths2days
SUBSYSTEM=CONSOLE
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)ths2days.$(O) \
         .$(L)toolbox.res

SRC_LIST=.$(L)ths2days.$(P)

build_all:
    $(CC) $(CFLAGS) $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q 2.manifest
