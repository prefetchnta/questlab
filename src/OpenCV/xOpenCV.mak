PROJECT=xOpenCV
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2013_x86.mak"

OBJ_LIST=.$(L)xOpenCV.$(O) \
         .$(L)cOpenCV.$(O) \
         .$(L)fOpenCV.$(O) \
         .$(L)zOpenCV.$(O)

SRC_LIST=.$(L)xOpenCV.$(P) \
         .$(L)cOpenCV.$(P) \
         .$(L)fOpenCV.$(P) \
         .$(L)zOpenCV.$(P)

build_all:
    $(CC) $(CFLAGS) /I "..\..\inc\opencv" /EHsc /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)opencv $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
