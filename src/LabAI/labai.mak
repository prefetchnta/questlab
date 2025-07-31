PROJECT=LabAI
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
LIB_NAME=$(PROJECT).lib
INC_LABAI=..\..\inc\x64inc\labai
INC_OPENCV=..\..\inc\x64inc\opencv
!INCLUDE "../vc2022_x64.mak"
CFLAGS=$(CFLAGS) /std:c++20 /EHsc /utf-8

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

OBJ_ZXING_CPP=Barcode.obj \
              BarcodeFormat.obj \
              BinaryBitmap2.obj \
              BitArray2.obj \
              BitMatrix2.obj \
              BitMatrixIO.obj \
              BitSource2.obj \
              CharacterSet.obj \
              ConcentricFinder.obj \
              Content.obj \
              DecodeHints.obj \
              ECI.obj \
              Error.obj \
              GenericGF2.obj \
              GenericGFPoly2.obj \
              GlobalHistogramBinarizer.obj \
              GridSampler.obj \
              GTIN.obj \
              HRI.obj \
              HybridBinarizer.obj \
              MultiFormatReader.obj \
              MultiFormatWriter.obj \
              PerspectiveTransform.obj \
              ReadBarcode.obj \
              ReedSolomonDecoder.obj \
              ReedSolomonEncoder.obj \
              ResultPoint2.obj \
              TextDecoder.obj \
              TextEncoder.obj \
              TextUtfEncoding.obj \
              Utf.obj \
              WhiteRectDetector.obj \
              AZDecoder.obj \
              AZDetector.obj \
              AZEncoder.obj \
              AZHighLevelEncoder.obj \
              AZReader.obj \
              AZToken.obj \
              AZWriter.obj \
              DMBitLayout.obj \
              DMDataBlock.obj \
              DMDecoder.obj \
              DMDetector.obj \
              DMECEncoder.obj \
              DMHighLevelEncoder.obj \
              DMReader.obj \
              DMSymbolInfo.obj \
              DMVersion.obj \
              DMWriter.obj \
              zueci.obj \
              MCBitMatrixParser.obj \
              MCDecoder.obj \
              MCReader.obj \
              ODCodabarReader.obj \
              ODCodabarWriter.obj \
              ODCode39Reader.obj \
              ODCode39Writer.obj \
              ODCode93Reader.obj \
              ODCode93Writer.obj \
              ODCode128Patterns.obj \
              ODCode128Reader.obj \
              ODCode128Writer.obj \
              ODDataBarCommon.obj \
              ODDataBarExpandedBitDecoder.obj \
              ODDataBarExpandedReader.obj \
              ODDataBarLimitedReader.obj \
              ODDataBarReader.obj \
              ODDXFilmEdgeReader.obj \
              ODEAN8Writer.obj \
              ODEAN13Writer.obj \
              ODITFReader.obj \
              ODITFWriter.obj \
              ODMultiUPCEANReader.obj \
              ODReader.obj \
              ODUPCAWriter.obj \
              ODUPCEANCommon.obj \
              ODUPCEWriter.obj \
              ODWriterHelper.obj \
              PDFBarcodeValue.obj \
              PDFBoundingBox.obj \
              PDFCodewordDecoder.obj \
              PDFDecoder.obj \
              PDFDetectionResult.obj \
              PDFDetectionResultColumn.obj \
              PDFDetector.obj \
              PDFEncoder.obj \
              PDFHighLevelEncoder.obj \
              PDFModulusGF.obj \
              PDFModulusPoly.obj \
              PDFReader.obj \
              PDFScanningDecoder.obj \
              PDFWriter.obj \
              ZXBigInteger.obj \
              QRBitMatrixParser.obj \
              QRCodecMode.obj \
              QRDataBlock.obj \
              QRDecoder.obj \
              QRDetector.obj \
              QREncoder.obj \
              QRErrorCorrectionLevel.obj \
              QRFormatInformation.obj \
              QRMaskUtil.obj \
              QRMatrixUtil.obj \
              QRReader.obj \
              QRVersion.obj \
              QRWriter.obj

SRC_ZXING_CPP=zxing\Barcode.cpp \
              zxing\BarcodeFormat.cpp \
              zxing\BinaryBitmap2.cpp \
              zxing\BitArray2.cpp \
              zxing\BitMatrix2.cpp \
              zxing\BitMatrixIO.cpp \
              zxing\BitSource2.cpp \
              zxing\CharacterSet.cpp \
              zxing\ConcentricFinder.cpp \
              zxing\Content.cpp \
              zxing\DecodeHints.cpp \
              zxing\ECI.cpp \
              zxing\Error.cpp \
              zxing\GenericGF2.cpp \
              zxing\GenericGFPoly2.cpp \
              zxing\GlobalHistogramBinarizer.cpp \
              zxing\GridSampler.cpp \
              zxing\GTIN.cpp \
              zxing\HRI.cpp \
              zxing\HybridBinarizer.cpp \
              zxing\MultiFormatReader.cpp \
              zxing\MultiFormatWriter.cpp \
              zxing\PerspectiveTransform.cpp \
              zxing\ReadBarcode.cpp \
              zxing\ReedSolomonDecoder.cpp \
              zxing\ReedSolomonEncoder.cpp \
              zxing\ResultPoint2.cpp \
              zxing\TextDecoder.cpp \
              zxing\TextEncoder.cpp \
              zxing\TextUtfEncoding.cpp \
              zxing\Utf.cpp \
              zxing\WhiteRectDetector.cpp \
              zxing\aztec\AZDecoder.cpp \
              zxing\aztec\AZDetector.cpp \
              zxing\aztec\AZEncoder.cpp \
              zxing\aztec\AZHighLevelEncoder.cpp \
              zxing\aztec\AZReader.cpp \
              zxing\aztec\AZToken.cpp \
              zxing\aztec\AZWriter.cpp \
              zxing\datamatrix\DMBitLayout.cpp \
              zxing\datamatrix\DMDataBlock.cpp \
              zxing\datamatrix\DMDecoder.cpp \
              zxing\datamatrix\DMDetector.cpp \
              zxing\datamatrix\DMECEncoder.cpp \
              zxing\datamatrix\DMHighLevelEncoder.cpp \
              zxing\datamatrix\DMReader.cpp \
              zxing\datamatrix\DMSymbolInfo.cpp \
              zxing\datamatrix\DMVersion.cpp \
              zxing\datamatrix\DMWriter.cpp \
              zxing\libzueci\zueci.c \
              zxing\maxicode\MCBitMatrixParser.cpp \
              zxing\maxicode\MCDecoder.cpp \
              zxing\maxicode\MCReader.cpp \
              zxing\oned\ODCodabarReader.cpp \
              zxing\oned\ODCodabarWriter.cpp \
              zxing\oned\ODCode39Reader.cpp \
              zxing\oned\ODCode39Writer.cpp \
              zxing\oned\ODCode93Reader.cpp \
              zxing\oned\ODCode93Writer.cpp \
              zxing\oned\ODCode128Patterns.cpp \
              zxing\oned\ODCode128Reader.cpp \
              zxing\oned\ODCode128Writer.cpp \
              zxing\oned\ODDataBarCommon.cpp \
              zxing\oned\ODDataBarExpandedBitDecoder.cpp \
              zxing\oned\ODDataBarExpandedReader.cpp \
              zxing\oned\ODDataBarLimitedReader.cpp \
              zxing\oned\ODDataBarReader.cpp \
              zxing\oned\ODDXFilmEdgeReader.cpp \
              zxing\oned\ODEAN8Writer.cpp \
              zxing\oned\ODEAN13Writer.cpp \
              zxing\oned\ODITFReader.cpp \
              zxing\oned\ODITFWriter.cpp \
              zxing\oned\ODMultiUPCEANReader.cpp \
              zxing\oned\ODReader.cpp \
              zxing\oned\ODUPCAWriter.cpp \
              zxing\oned\ODUPCEANCommon.cpp \
              zxing\oned\ODUPCEWriter.cpp \
              zxing\oned\ODWriterHelper.cpp \
              zxing\pdf417\PDFBarcodeValue.cpp \
              zxing\pdf417\PDFBoundingBox.cpp \
              zxing\pdf417\PDFCodewordDecoder.cpp \
              zxing\pdf417\PDFDecoder.cpp \
              zxing\pdf417\PDFDetectionResult.cpp \
              zxing\pdf417\PDFDetectionResultColumn.cpp \
              zxing\pdf417\PDFDetector.cpp \
              zxing\pdf417\PDFEncoder.cpp \
              zxing\pdf417\PDFHighLevelEncoder.cpp \
              zxing\pdf417\PDFModulusGF.cpp \
              zxing\pdf417\PDFModulusPoly.cpp \
              zxing\pdf417\PDFReader.cpp \
              zxing\pdf417\PDFScanningDecoder.cpp \
              zxing\pdf417\PDFWriter.cpp \
              zxing\pdf417\ZXBigInteger.cpp \
              zxing\qrcode\QRBitMatrixParser.cpp \
              zxing\qrcode\QRCodecMode.cpp \
              zxing\qrcode\QRDataBlock.cpp \
              zxing\qrcode\QRDecoder.cpp \
              zxing\qrcode\QRDetector.cpp \
              zxing\qrcode\QREncoder.cpp \
              zxing\qrcode\QRErrorCorrectionLevel.cpp \
              zxing\qrcode\QRFormatInformation.cpp \
              zxing\qrcode\QRMaskUtil.cpp \
              zxing\qrcode\QRMatrixUtil.cpp \
              zxing\qrcode\QRReader.cpp \
              zxing\qrcode\QRVersion.cpp \
              zxing\qrcode\QRWriter.cpp

OBJ_HYPERLPR3=camera_buffer.obj \
              hyper_lpr_context.obj \
              inference_helper.obj \
              inference_helper_mnn.obj \
              classification_engine.obj \
              plate_classification.obj \
              det_arch.obj \
              det_backbone.obj \
              det_header.obj \
              plate_detector.obj \
              plate_recognition.obj \
              recognition_engine.obj \
              mnn_adapter.obj \
              hyper_lpr_sdk.obj

SRC_HYPERLPR3=hyperlpr\v3\buffer_module\camera_buffer.cpp \
              hyperlpr\v3\context_module\hyper_lpr_context.cpp \
              hyperlpr\v3\inference_helper_module\inference_helper.cpp \
              hyperlpr\v3\inference_helper_module\inference_helper_mnn.cpp \
              hyperlpr\v3\nn_implementation_module\classification\classification_engine.cpp \
              hyperlpr\v3\nn_implementation_module\classification\plate_classification.cpp \
              hyperlpr\v3\nn_implementation_module\detect\det_arch.cpp \
              hyperlpr\v3\nn_implementation_module\detect\det_backbone.cpp \
              hyperlpr\v3\nn_implementation_module\detect\det_header.cpp \
              hyperlpr\v3\nn_implementation_module\detect\plate_detector.cpp \
              hyperlpr\v3\nn_implementation_module\recognition\plate_recognition.cpp \
              hyperlpr\v3\nn_implementation_module\recognition\recognition_engine.cpp \
              hyperlpr\v3\nn_module\mnn_adapter.cpp \
              hyperlpr\v3\hyper_lpr_sdk.cc

OBJ_LIST=dllmain.obj \
         imglab_ncnn_dect.obj \
         imglab_zxing_dect.obj \
         imglab_crhack_draw.obj \
         imglab_opencv_base.obj \
         imglab_opencv_dect.obj \
         imglab_opencv_intr.obj \
         imglab_opencv_proc.obj \
         imglab_hyperlpr_dect.obj \
         $(OBJ_OCV_WECHAT) $(OBJ_ZXING_CPP) $(OBJ_HYPERLPR3)

SRC_LIST=dllmain.cpp \
         imglab_ncnn_dect.cpp \
         imglab_zxing_dect.cpp \
         imglab_crhack_draw.cpp \
         imglab_opencv_base.cpp \
         imglab_opencv_dect.cpp \
         imglab_opencv_intr.cpp \
         imglab_opencv_proc.cpp \
         imglab_hyperlpr_dect.cpp

FLAGS_OCV_WECHAT=/I $(INC_OPENCV)

FLAGS_ZXING_CPP=/I $(INC_OPENCV) /I.\zxing /D "ZXING_READERS" /Zc:__cplusplus \
                /wd4101 /wd4127 /wd4244 /wd4245 /wd4267 /wd4305 /wd4456 /wd4457 /wd4458 /wd4459 /wd4706

FLAGS_HYPERLPR3=/I $(INC_OPENCV) /I.\hyperlpr\v3\ /I.\mnn /D "_CRT_SECURE_NO_WARNINGS" /D "INFERENCE_HELPER_ENABLE_MNN" \
                /openmp /wd4065 /wd4091 /wd4100 /wd4189 /wd4244 /wd4267 /wd4305

build_all:
    if not exist $(INC_OPENCV) mklink /J $(INC_OPENCV) $(INC_OPENCV)4
    $(CC) $(CFLAGS) $(FLAGS_OCV_WECHAT) $(SRC_OCV_WECHAT)
    $(CC) $(CFLAGS) $(FLAGS_ZXING_CPP) $(SRC_ZXING_CPP)
    $(CC) $(CFLAGS) $(FLAGS_HYPERLPR3) $(SRC_HYPERLPR3)
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" /I $(INC_LABAI) /I $(INC_OPENCV) /openmp $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST)
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..\..\bin\x64bin
    move $(LIB_NAME) ..\..\lib\x64lib
    del /Q *.obj
    del /Q *.exp
    del /Q 2.manifest
    rmdir $(INC_OPENCV)
