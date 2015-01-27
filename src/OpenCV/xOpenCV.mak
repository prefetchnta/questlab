PROJECT=xOpenCV
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)xOpenCV.$(O) \
         .$(L)cOpenCV.$(O) \
         .$(L)fOpenCV.$(O) \
         .$(L)zOpenCV.$(O) \
         .$(L)bitstream.$(O) \
         .$(L)codedata.$(O) \
         .$(L)container.$(O) \
         .$(L)ecidecoder.$(O) \
         .$(L)formatinfo.$(O) \
         .$(L)galois.$(O) \
         .$(L)imagereader.$(O) \
         .$(L)libdecodeqr.$(O)

SRC_LIST=.$(L)xOpenCV.$(P) \
         .$(L)cOpenCV.$(P) \
         .$(L)fOpenCV.$(P) \
         .$(L)zOpenCV.$(P) \
         .$(L)libdecodeqr$(L)bitstream.$(P) \
         .$(L)libdecodeqr$(L)codedata.$(P) \
         .$(L)libdecodeqr$(L)container.$(P) \
         .$(L)libdecodeqr$(L)ecidecoder.$(P) \
         .$(L)libdecodeqr$(L)formatinfo.$(P) \
         .$(L)libdecodeqr$(L)galois.$(P) \
         .$(L)libdecodeqr$(L)imagereader.$(P) \
         .$(L)libdecodeqr$(L)libdecodeqr.$(P)

build_all:
    $(CC) $(CFLAGS) /I "..\..\ocv" /EHsc /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /LIBPATH:..$(L)..$(L)lib$(L)opencv $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)plugin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
