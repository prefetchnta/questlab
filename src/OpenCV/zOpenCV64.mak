PROJECT=zOpenCV64
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2022_x64.mak"

OBJ_LIST=.$(L)zOpenCV64.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)zOpenCV64.$(P)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /EHsc /I "..\..\inc\x64inc\opencv4" $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)x64bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
