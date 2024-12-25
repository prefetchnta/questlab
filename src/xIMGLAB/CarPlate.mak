PROJECT=CarPlate
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).ilab
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)CarPlate.$(O)

SRC_LIST=.$(L)CarPlate.$(P)

build_all:
    $(CC) $(CFLAGS) /I "..\..\inc\opencv" /EHsc /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)opencv $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
