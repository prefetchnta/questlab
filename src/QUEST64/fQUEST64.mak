PROJECT=fQUEST64
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2019_x64.mak"

OBJ_LIST=.$(L)fQUEST64.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)fQUEST64.$(P)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /EHsc $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)x64bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
