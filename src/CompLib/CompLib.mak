PROJECT=CompLib
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
!INCLUDE "../vc2010_x86.mak"

OBJ_LIST=.$(L)CompLib.$(O) \
         .$(L)comp_ap.$(O) \
         .$(L)aplib$(L)aplib.lib \
         .$(L)comp_blz.$(O) \
         .$(L)brieflz.$(O) \
         .$(L)blz_depack.$(O) \
         .$(L)blz_depacks.$(O) \
         .$(L)comp_jc1.$(O) \
         .$(L)jcalg1$(L)jcalg1.lib \
         .$(L)comp_lz4.$(O) \
         ..$(L)..$(L)lib$(L)libgcc.a \
         .$(L)lz4$(L)liblz4_static.lib \
         .$(L)comp_7z.$(O) \
         .$(L)LzFind.$(O) \
         .$(L)LzFindMt.$(O) \
         .$(L)LzmaDec.$(O) \
         .$(L)LzmaEnc.$(O) \
         .$(L)LzmaLib.$(O) \
         .$(L)Threads.$(O)

SRC_LIST=.$(L)CompLib.$(P) \
         .$(L)comp_ap.$(C) \
         .$(L)comp_blz.$(C) \
         /Tp .$(L)brieflz$(L)brieflz.$(C) \
         /Tp .$(L)brieflz$(L)blz_depack.$(C) \
         /Tp .$(L)brieflz$(L)blz_depacks.$(C) \
         .$(L)comp_jc1.$(C) \
         .$(L)comp_lz4.$(C) \
         .$(L)comp_7z.$(C) \
         .$(L)lzma$(L)LzFind.$(C) \
         .$(L)lzma$(L)LzFindMt.$(C) \
         .$(L)lzma$(L)LzmaDec.$(C) \
         .$(L)lzma$(L)LzmaEnc.$(C) \
         .$(L)lzma$(L)LzmaLib.$(C) \
         .$(L)lzma$(L)Threads.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
