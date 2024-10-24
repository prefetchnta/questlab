PROJECT=LabAI
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
LIB_NAME=$(PROJECT).lib
INC_LABAI=..\..\inc\x64inc\labai
INC_OPENCV=..\..\inc\x64inc\opencv
!INCLUDE "../vc2019_x64.mak"

OBJ_OCV_WECHAT=binarizermgr.obj \
               decodermgr.obj \
               imgsource.obj \
               wechat_qrcode.obj \
               align.obj \
               ssd_detector.obj \
               super_scale.obj \
               binarizer.obj \
               binarybitmap.obj \
               errorhandler.obj \
               luminance_source.obj \
               reader.obj \
               result.obj \
               resultpoint.obj \
               bitarray.obj \
               bitmatrix.obj \
               bitsource.obj \
               bytematrix.obj \
               characterseteci.obj \
               decoder_result.obj \
               detector_result.obj \
               greyscale_luminance_source.obj \
               greyscale_rotated_luminance_source.obj \
               grid_sampler.obj \
               imagecut.obj \
               kmeans.obj \
               perspective_transform.obj \
               str.obj \
               stringutils.obj \
               unicomblock.obj \
               adaptive_threshold_mean_binarizer.obj \
               fast_window_binarizer.obj \
               global_histogram_binarizer.obj \
               hybrid_binarizer.obj \
               simple_adaptive_binarizer.obj \
               genericgf.obj \
               genericgfpoly.obj \
               reed_solomon_decoder.obj \
               error_correction_level.obj \
               format_information.obj \
               qrcode_reader.obj \
               version.obj \
               bitmatrixparser.obj \
               datablock.obj \
               datamask.obj \
               decoded_bit_stream_parser.obj \
               decoder.obj \
               mode.obj \
               alignment_pattern.obj \
               alignment_pattern_finder.obj \
               detector.obj \
               finder_pattern.obj \
               finder_pattern_finder.obj \
               finder_pattern_info.obj \
               pattern_result.obj

SRC_OCV_WECHAT=opencv\wechat_qrcode\binarizermgr.cpp \
               opencv\wechat_qrcode\decodermgr.cpp \
               opencv\wechat_qrcode\imgsource.cpp \
               opencv\wechat_qrcode\wechat_qrcode.cpp \
               opencv\wechat_qrcode\detector\align.cpp \
               opencv\wechat_qrcode\detector\ssd_detector.cpp \
               opencv\wechat_qrcode\scale\super_scale.cpp \
               opencv\wechat_qrcode\zxing\binarizer.cpp \
               opencv\wechat_qrcode\zxing\binarybitmap.cpp \
               opencv\wechat_qrcode\zxing\errorhandler.cpp \
               opencv\wechat_qrcode\zxing\luminance_source.cpp \
               opencv\wechat_qrcode\zxing\reader.cpp \
               opencv\wechat_qrcode\zxing\result.cpp \
               opencv\wechat_qrcode\zxing\resultpoint.cpp \
               opencv\wechat_qrcode\zxing\common\bitarray.cpp \
               opencv\wechat_qrcode\zxing\common\bitmatrix.cpp \
               opencv\wechat_qrcode\zxing\common\bitsource.cpp \
               opencv\wechat_qrcode\zxing\common\bytematrix.cpp \
               opencv\wechat_qrcode\zxing\common\characterseteci.cpp \
               opencv\wechat_qrcode\zxing\common\decoder_result.cpp \
               opencv\wechat_qrcode\zxing\common\detector_result.cpp \
               opencv\wechat_qrcode\zxing\common\greyscale_luminance_source.cpp \
               opencv\wechat_qrcode\zxing\common\greyscale_rotated_luminance_source.cpp \
               opencv\wechat_qrcode\zxing\common\grid_sampler.cpp \
               opencv\wechat_qrcode\zxing\common\imagecut.cpp \
               opencv\wechat_qrcode\zxing\common\kmeans.cpp \
               opencv\wechat_qrcode\zxing\common\perspective_transform.cpp \
               opencv\wechat_qrcode\zxing\common\str.cpp \
               opencv\wechat_qrcode\zxing\common\stringutils.cpp \
               opencv\wechat_qrcode\zxing\common\unicomblock.cpp \
               opencv\wechat_qrcode\zxing\common\binarizer\adaptive_threshold_mean_binarizer.cpp \
               opencv\wechat_qrcode\zxing\common\binarizer\fast_window_binarizer.cpp \
               opencv\wechat_qrcode\zxing\common\binarizer\global_histogram_binarizer.cpp \
               opencv\wechat_qrcode\zxing\common\binarizer\hybrid_binarizer.cpp \
               opencv\wechat_qrcode\zxing\common\binarizer\simple_adaptive_binarizer.cpp \
               opencv\wechat_qrcode\zxing\common\reedsolomon\genericgf.cpp \
               opencv\wechat_qrcode\zxing\common\reedsolomon\genericgfpoly.cpp \
               opencv\wechat_qrcode\zxing\common\reedsolomon\reed_solomon_decoder.cpp \
               opencv\wechat_qrcode\zxing\qrcode\error_correction_level.cpp \
               opencv\wechat_qrcode\zxing\qrcode\format_information.cpp \
               opencv\wechat_qrcode\zxing\qrcode\qrcode_reader.cpp \
               opencv\wechat_qrcode\zxing\qrcode\version.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\bitmatrixparser.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\datablock.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\datamask.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\decoded_bit_stream_parser.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\decoder.cpp \
               opencv\wechat_qrcode\zxing\qrcode\decoder\mode.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\alignment_pattern.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\alignment_pattern_finder.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\detector.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\finder_pattern.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\finder_pattern_finder.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\finder_pattern_info.cpp \
               opencv\wechat_qrcode\zxing\qrcode\detector\pattern_result.cpp

OBJ_LIST=dllmain.obj \
         imglab_opencv.obj \
         imglab_opencv_clss.obj \
         imglab_opencv_conv.obj \
         imglab_opencv_intr.obj \
         $(OBJ_OCV_WECHAT)

SRC_LIST=dllmain.cpp \
         imglab_opencv.cpp \
         imglab_opencv_clss.cpp \
         imglab_opencv_conv.cpp \
         imglab_opencv_intr.cpp \
         $(SRC_OCV_WECHAT)

build_all:
    if not exist $(INC_OPENCV) mklink /J $(INC_OPENCV) $(INC_OPENCV)4
    $(CC) $(CFLAGS) /EHsc /D "_CR_BUILD_DLL_" /I $(INC_LABAI) /I $(INC_OPENCV) $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..\..\bin\x64bin
    move $(LIB_NAME) ..\..\lib\x64lib
    del /Q *.obj
    del /Q *.exp
    del /Q 2.manifest
    rmdir $(INC_OPENCV)
