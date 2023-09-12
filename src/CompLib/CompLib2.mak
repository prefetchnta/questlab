PROJECT=CompLib2
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)CompLib.$(O) \
         .$(L)comp_lizard.$(O) \
         .$(L)lizard$(L)liblizard_static.lib

SRC_LIST=.$(L)CompLib.$(P) \
         .$(L)comp_lizard.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" /I. $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
