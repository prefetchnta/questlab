PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)flc_aia.$(O) \
         .$(L)flc_nnk.$(O) \
         .$(L)flc_zed.$(O) \
         .$(L)e_falcom.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_falcom.$(C) \
         .$(L)falcom$(L)flc_aia.$(C) \
         .$(L)falcom$(L)flc_nnk.$(C) \
         .$(L)falcom$(L)flc_zed.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zFALCOM.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
