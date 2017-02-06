PROJECT=kernel33
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)BackPort.$(O) \
         .$(L)kernel33.$(O) \
         .$(L)myKernel32.$(O)

SRC_LIST=.$(L)BackPort.$(C) \
         .$(L)kernel33.$(C) \
         .$(L)myKernel32.$(C)

build_all:
    $(CC) $(CFLAGS) /GS- $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /ENTRY:DllMain $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
