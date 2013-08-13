PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)e_falcom.$(O) \
         .$(L)flcm_nnk.$(O) \
         .$(L)flcm_z.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_falcom.$(C) \
         .$(L)falcom$(L)flcm_nnk.$(C) \
         .$(L)falcom$(L)flcm_z.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zFALCOM.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
