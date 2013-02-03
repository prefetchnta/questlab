PROJECT=xImpScript
SUBSYSTEM=CONSOLE
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)xImpScript.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)xImpScript.$(P)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
