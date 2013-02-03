PROJECT=QstLibs
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)QstLibs.$(O)

SRC_LIST=.$(L)QstLibs.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /DEF:.$(L)$(PROJECT).def $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
