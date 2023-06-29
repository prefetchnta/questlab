PROJECT=LabAI
SUBSYSTEM=WINDOWS
BIN_NAME=$(PROJECT).dll
LIB_NAME=$(PROJECT).lib
INC_LABAI=..\..\inc\x64inc\labai
INC_OPENCV=..\..\inc\x64inc\opencv
!INCLUDE "../vc2019_x64.mak"

OBJ_LIST=dllmain.obj \
         imglab_opencv.obj \
         imglab_opencv_clss.obj \
         imglab_opencv_conv.obj

SRC_LIST=dllmain.cpp \
         imglab_opencv.cpp \
         imglab_opencv_clss.cpp \
         imglab_opencv_conv.cpp

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
