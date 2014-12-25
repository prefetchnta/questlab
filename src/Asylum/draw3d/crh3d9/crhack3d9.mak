PROJECT=CrHack3D9
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)asylum.$(O) \
         .$(L)crh3d9_wf_effect.$(O) \
         .$(L)crh3d9_wf_stuffz.$(O)

SRC_LIST=.$(L)asylum.$(P) \
         .$(L)draw3d$(L)crh3d9$(L)crh3d9_wf_effect.$(P) \
         .$(L)draw3d$(L)crh3d9$(L)crh3d9_wf_stuffz.$(P)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
