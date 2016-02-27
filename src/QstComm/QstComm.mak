PROJECT=QstComm
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2010_x86.mak"
QT=..\moc

OBJ_LIST=.$(L)QstComm.$(O) \
         .$(L)QstCommInt.$(O) \
         .$(L)QstComm_rs232.$(O) \
         .$(L)QstComm_tcpv4.$(O) \
         .$(L)QstComm_udpv4.$(O) \
         .$(L)QstComm_main.$(O) \
         .$(L)QstComm_tran.$(O) \
         .$(L)QstComm_show.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)QstComm.$(P) \
         .$(L)QstCommInt.$(P) \
         .$(L)QstComm_rs232.$(P) \
         .$(L)QstComm_tcpv4.$(P) \
         .$(L)QstComm_udpv4.$(P) \
         .$(L)QstComm_main.$(P) \
         .$(L)QstComm_tran.$(P) \
         .$(L)QstComm_show.$(P)

build_all:
    $(QT) QstCommInt.h > QstCommInt.$(P)
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /I "..\..\inc\qt5" /EHsc $(SRC_LIST)
    $(LD) $(LFLAGS) /LIBPATH:..$(L)..$(L)lib$(L)qt5 $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
    del /Q QstCommInt.$(P)
