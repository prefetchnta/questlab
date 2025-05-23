PROJECT=QstHashImp
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)QstHashImp.$(O)

SRC_LIST=.$(L)QstHashImp.$(P)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
