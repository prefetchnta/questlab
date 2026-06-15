PROJECT=LabAI
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
LIB_NAME=$(PROJECT).lib
INC_LABAI=..\..\inc\x64inc\labai
INC_OPENCV=..\..\inc\x64inc\opencv
!INCLUDE "../vc2022_x64.mak"
CFLAGS=$(CFLAGS) /std:c++20 /EHsc /utf-8

OBJ_LIST=dllmain.obj \
         imglab_ncnn_base.obj \
         imglab_ncnn_dect.obj \
         imglab_zxing_dect.obj \
         imglab_crhack_draw.obj \
         imglab_opencv_base.obj \
         imglab_opencv_dect.obj \
         imglab_opencv_intr.obj \
         imglab_opencv_proc.obj \
         imglab_hyperlpr_dect.obj

SRC_LIST=dllmain.cpp \
         imglab_ncnn_base.cpp \
         imglab_ncnn_dect.cpp \
         imglab_zxing_dect.cpp \
         imglab_crhack_draw.cpp \
         imglab_opencv_base.cpp \
         imglab_opencv_dect.cpp \
         imglab_opencv_intr.cpp \
         imglab_opencv_proc.cpp \
         imglab_hyperlpr_dect.cpp

build_all:
    if not exist $(INC_OPENCV) mklink /J $(INC_OPENCV) $(INC_OPENCV)4
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" /wd4996 /I $(INC_LABAI) /I $(INC_OPENCV) /openmp $(SRC_LIST)
    $(LD) $(LFLAGS) /DLL $(OBJ_LIST) LabAIex.lib
    $(MT) $(MFLAGS)
    move $(BIN_NAME) ..\..\bin\x64bin
    move $(LIB_NAME) ..\..\lib\x64lib
    del /Q *.obj
    del /Q *.exp
    del /Q 2.manifest
    rmdir $(INC_OPENCV)
