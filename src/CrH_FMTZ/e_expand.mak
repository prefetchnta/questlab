PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)szdd.$(O) \
         .$(L)sz20.$(O) \
         .$(L)e_expand.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_expand.$(C) \
         .$(L)expand$(L)szdd.$(C) \
         .$(L)expand$(L)sz20.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zExpand.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
