PROJECT=xCrHackSet
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)xCrHackSet.$(O) \
         .$(L)cCrHackSet.$(O) \
         .$(L)fCrHackSet.$(O) \
         .$(L)zbar_decode.$(O)

SRC_LIST=.$(L)xCrHackSet.$(P) \
         .$(L)cCrHackSet.$(P) \
         .$(L)fCrHackSet.$(P) \
         .$(L)zbar_decode.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)zbar $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
