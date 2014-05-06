PROJECT=QstComm
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)QstComm.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)QstComm.$(P)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /I "..\..\qt5" /EHsc $(SRC_LIST)
    $(LD) $(LFLAGS) /LIBPATH:..$(L)..$(L)lib$(L)qt5 $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
