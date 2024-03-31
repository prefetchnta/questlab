PROJECT=CompLib1
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
         .$(L)comp_brotli.$(O) \
         .$(L)brotli$(L)libbrotlicommon.a \
         .$(L)brotli$(L)libbrotlidec.a \
         .$(L)brotli$(L)libbrotlienc.a \
         .$(L)comp_lz4.$(O) \
         .$(L)lz4$(L)liblz4_static.lib \
         .$(L)comp_qlz.$(O) \
         .$(L)comp_qlz1.$(O) \
         .$(L)comp_qlz2.$(O) \
         .$(L)comp_qlz3.$(O) \
         .$(L)comp_snappy.$(O) \
         .$(L)snappy$(L)snappy.lib \
         .$(L)comp_zstd.$(O) \
         .$(L)zstd$(L)libzstd_static.lib \
         .$(L)comp_lzma.$(O) \
         .$(L)LzFind.$(O) \
         .$(L)LzFindMt.$(O) \
         .$(L)LzmaDec.$(O) \
         .$(L)LzmaEnc.$(O) \
         .$(L)LzmaLib.$(O) \
         .$(L)Threads.$(O) \
         .$(L)comp_density.$(O) \
         .$(L)globals.$(O) \
         .$(L)algorithms.$(O) \
         .$(L)dictionaries.$(O) \
         .$(L)chameleon_decode.$(O) \
         .$(L)chameleon_encode.$(O) \
         .$(L)cheetah_decode.$(O) \
         .$(L)cheetah_encode.$(O) \
         .$(L)lion_decode.$(O) \
         .$(L)lion_encode.$(O) \
         .$(L)lion_form_model.$(O) \
         .$(L)buffer.$(O) \
         .$(L)header.$(O) \
         ..$(L)..$(L)lib$(L)gcc$(L)libgcc.a \
         ..$(L)..$(L)lib$(L)gcc$(L)libstdc++.a \
         ..$(L)..$(L)lib$(L)gcc$(L)libpthread.a \
         ..$(L)..$(L)lib$(L)gcc$(L)libmingwex.a

SRC_LIST=.$(L)CompLib.$(P) \
         .$(L)comp_ap.$(C) \
         .$(L)comp_blz.$(C) \
         /Tp .$(L)brieflz$(L)brieflz.$(C) \
         /Tp .$(L)brieflz$(L)blz_depack.$(C) \
         /Tp .$(L)brieflz$(L)blz_depacks.$(C) \
         .$(L)comp_brotli.$(C) \
         .$(L)comp_lz4.$(C) \
         .$(L)comp_qlz.$(C) \
         .$(L)comp_qlz1.$(C) \
         .$(L)comp_qlz2.$(C) \
         .$(L)comp_qlz3.$(C) \
         .$(L)comp_snappy.$(C) \
         .$(L)comp_zstd.$(C) \
         .$(L)comp_lzma.$(C) \
         .$(L)lzma$(L)LzFind.$(C) \
         .$(L)lzma$(L)LzFindMt.$(C) \
         .$(L)lzma$(L)LzmaDec.$(C) \
         .$(L)lzma$(L)LzmaEnc.$(C) \
         .$(L)lzma$(L)LzmaLib.$(C) \
         .$(L)lzma$(L)Threads.$(C) \
         /Tp .$(L)comp_density.$(C) \
         /Tp .$(L)density$(L)globals.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)algorithms.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)dictionaries.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)chameleon$(L)core$(L)chameleon_decode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)chameleon$(L)core$(L)chameleon_encode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)cheetah$(L)core$(L)cheetah_decode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)cheetah$(L)core$(L)cheetah_encode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)lion$(L)core$(L)lion_decode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)lion$(L)core$(L)lion_encode.$(C) \
         /Tp .$(L)density$(L)algorithms$(L)lion$(L)forms$(L)lion_form_model.$(C) \
         /Tp .$(L)density$(L)buffers$(L)buffer.$(C) \
         /Tp .$(L)density$(L)structure$(L)header.$(C)

build_all:
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" /I. $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..$(L)..$(L)bin$(L)
    move $(PROJECT).lib ..$(L)..$(L)tmp$(L)
    del /Q *.obj
    del /Q 2.manifest
    del /Q $(PROJECT).exp
