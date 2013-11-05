PROJECT=CrH_EXTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)extz.$(O) \
         .$(L)net_win32.$(O) \
         .$(L)xmm_dshow.$(O) \
         .$(L)xmm_win32.$(O) \
         .$(L)$(PROJECT).res


SRC_LIST=.$(L)extz.$(C) \
         .$(L)net_win32.$(C) \
         .$(L)xmm_dshow.$(P) \
         .$(L)xmm_win32.$(C)

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
