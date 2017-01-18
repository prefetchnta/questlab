PROJECT=HackLib
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)HackLib.$(O) \
         .$(L)FuncHook.$(O) \
         .$(L)WindowCtrl.$(O) \
         .$(L)ProcessCtrl.$(O) \
         .$(L)RemoteInject.$(O)

SRC_LIST=.$(L)HackLib.$(P) \
         .$(L)FuncHook.$(P) \
         .$(L)WindowCtrl.$(P) \
         .$(L)ProcessCtrl.$(P) \
         .$(L)RemoteInject.$(P)

build_all:
    $(CC) $(CFLAGS) /EHsc /GS- /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL /DEF:.$(L)$(PROJECT).def $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
