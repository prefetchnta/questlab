PROJECT=QstTrade
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).exe
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)QstTrade.$(O) \
         .$(L)QstTrade_Amount.$(O) \
         .$(L)QstTrade_Candle.$(O) \
         .$(L)QstTrade_Config.$(O) \
         .$(L)QstTrade_Indctr.$(O) \
         .$(L)$(PROJECT).res

SRC_LIST=.$(L)QstTrade.$(P) \
         .$(L)QstTrade_Amount.$(P) \
         .$(L)QstTrade_Candle.$(P) \
         .$(L)QstTrade_Config.$(P) \
         .$(L)QstTrade_Indctr.$(P)

build_all:
    $(RC) $(RFLAGS)
    $(CC) $(CFLAGS) /EHsc $(SRC_LIST)
    $(LD) $(LFLAGS) $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    del /Q *.obj
    del /Q *.res
    del /Q 2.manifest
