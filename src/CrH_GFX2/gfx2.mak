PROJECT=CrH_GFX2
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)gfx2.$(O) \
         .$(L)blit_c.$(O) \
         .$(L)bltint.$(O) \
         .$(L)draw2d.$(O) \
         .$(L)pixels.$(O) \
         .$(L)btfont.$(O) \
         .$(L)ucfont.$(O) \
         .$(L)surfmem.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)gfx2.$(C) \
         .$(L)blit_c.$(C) \
         .$(L)bltint.$(C) \
         .$(L)draw2d.$(C) \
         .$(L)pixels.$(C) \
         .$(L)btfont.$(C) \
         .$(L)ucfont.$(C) \
         .$(L)surfmem.$(C)

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
