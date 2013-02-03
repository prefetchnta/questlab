PROJECT=CrH_FMTZ
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2003_x86.mak"

OBJ_LIST=.$(L)fmtz.$(O) \
         .$(L)gcax.$(O) \
         .$(L)bgra.$(O) \
         .$(L)e_syn.$(O)

SRC_LIST=.$(L)fmtz.$(C) \
         .$(L)e_syn.$(C) \
         .$(L)syn$(L)gcax.$(P) \
         .$(L)syn$(L)bgra.$(C)

build_all:
    $(CC) $(CFLAGS) /EHsc /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)gcasdk $(OBJ_LIST)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)zSYN.dll
    del /Q *.obj
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
