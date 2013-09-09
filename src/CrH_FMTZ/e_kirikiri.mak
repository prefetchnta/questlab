PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)krkr_xp3.$(O) \
         .$(L)e_kirikiri.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_kirikiri.$(C) \
         .$(L)kirikiri$(L)krkr_xp3.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zKiriKiri.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
