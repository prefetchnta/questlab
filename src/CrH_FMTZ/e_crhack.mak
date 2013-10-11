PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)tjpgd.$(O) \
         .$(L)cr_bmp.$(O) \
         .$(L)cr_dds.$(O) \
         .$(L)cr_jpg.$(O) \
         .$(L)cr_pcx.$(O) \
         .$(L)cr_png.$(O) \
         .$(L)cr_tga.$(O) \
         .$(L)cr_zip.$(O) \
         .$(L)e_crhack.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)cr_bmp.$(C) \
         .$(L)cr_dds.$(C) \
         .$(L)cr_jpg.$(C) \
         .$(L)cr_pcx.$(C) \
         .$(L)cr_png.$(C) \
         .$(L)cr_tga.$(C) \
         .$(L)cr_zip.$(C) \
         .$(L)e_crhack.$(C) \
         .$(L)tjpgdec$(L)tjpgd.$(C)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
    del /Q $(PROJECT).exp
