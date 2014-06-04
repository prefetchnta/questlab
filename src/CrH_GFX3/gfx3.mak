PROJECT=CrH_GFX3
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)gfx3.$(O) \
         .$(L)bound.$(O) \
         .$(L)camera.$(O) \
         .$(L)frustum.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)gfx3.$(C) \
         .$(L)bound.$(C) \
         .$(L)camera.$(C) \
         .$(L)frustum.$(C)

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