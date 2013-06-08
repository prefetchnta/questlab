PROJECT=ocr3rd
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2008_x86.mak"

OBJ_LIST=.$(L)ocr3rd.$(O)

SRC_LIST=.$(L)ocr3rd.$(P)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)tess $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
    del /Q $(PROJECT).lib
