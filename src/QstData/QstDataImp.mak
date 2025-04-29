PROJECT=QstDataImp
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)QstDataImp.$(O)

SRC_LIST=.$(L)QstDataImp.$(P)

build_all:
    $(CC) $(CFLAGS) /I "..\..\inc\bea" /I "..\..\inc\capstone" /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)bea /LIBPATH:..$(L)..$(L)lib$(L)stone $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
