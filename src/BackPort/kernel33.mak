PROJECT=kernel33
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)BackPort.$(O) \
         .$(L)kernel33.$(O)

SRC_LIST=.$(L)BackPort.$(C) \
         .$(L)kernel33.$(C)

build_all:
    $(CC) $(CFLAGS) /GS- /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /ENTRY:DllMain /DEF:.$(L)$(PROJECT).def $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
