PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)e_tgl.$(O) \
         .$(L)tgl_acf.$(O) \
         .$(L)tgl_bmr.$(O) \
         .$(L)tgl_cgr.$(O) \
         .$(L)tgl_pac.$(O) \
         .$(L)tgl_ipac.$(O) \
         .$(L)tgl_sdc.$(O) \
         .$(L)tgl_iel1.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_tgl.$(C) \
         .$(L)tgl$(L)tgl_acf.$(C) \
         .$(L)tgl$(L)tgl_bmr.$(C) \
         .$(L)tgl$(L)tgl_cgr.$(C) \
         .$(L)tgl$(L)tgl_pac.$(C) \
         .$(L)tgl$(L)tgl_ipac.$(C) \
         .$(L)tgl$(L)tgl_sdc.$(C) \
         .$(L)tgl$(L)tgl_iel1.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zTGL.dll
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
