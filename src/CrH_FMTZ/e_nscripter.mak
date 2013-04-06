PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)nscr_nsa.$(O) \
         .$(L)nscr_sar.$(O) \
         .$(L)nscr_dat.$(O) \
         .$(L)e_nscripter.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_nscripter.$(C) \
         .$(L)nscripter$(L)nscr_nsa.$(C) \
         .$(L)nscripter$(L)nscr_sar.$(C) \
         .$(L)nscripter$(L)nscr_dat.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zNScripter.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
