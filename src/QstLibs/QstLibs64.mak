PROJECT=QstLibs64
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
LIB_NAME=$(PROJECT).lib
!INCLUDE "../vc2019_x64.mak"

OBJ_LIST=.$(L)QstLibs.$(O)

SRC_LIST=.$(L)QstLibs.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..\..\bin\x64bin
    move $(LIB_NAME) ..\..\lib\x64lib
    del /Q *.obj
    del /Q *.exp
    del /Q 2.manifest
