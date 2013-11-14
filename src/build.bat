@echo off
rem =====================
cd QstHash
del ..\..\tmp\*.obj /Q
make -fQstHash.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstIndex
del ..\..\tmp\*.obj /Q
make -fQstIndex.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstInfoz
del ..\..\tmp\*.obj /Q
make -fQstInfoz.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstMedia
del ..\..\tmp\*.obj /Q
make -fQstMedia.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstMenu
del ..\..\tmp\*.obj /Q
make -fQstMenu.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstText
del ..\..\tmp\*.obj /Q
make -fQstText.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd QstTree
del ..\..\tmp\*.obj /Q
make -fQstTree.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd RunWebIE
del ..\..\tmp\*.obj /Q
make -fRunWebIE.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd xParamSet
del ..\..\tmp\*.obj /Q
make -fxParamSet.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
cd xSelectDir
del ..\..\tmp\*.obj /Q
make -fxSelectDir.mak
del ..\..\bin\*.tds /Q
cd..
rem =====================
call "%VS90COMNTOOLS%vsvars32.bat"
rem =====================
cd ocr3rd
nmake -focr3rd.mak
cd..
rem =====================
call "%VS100COMNTOOLS%vsvars32.bat"
rem =====================
cd QstCmdz
nmake -fQstCmdz.mak
cd..
rem =====================
cd QstLibs
nmake -fQstLibs.mak
cd..
rem =====================
cd QstMount
nmake -fQstMount.mak
cd..
rem =====================
cd QstServ
nmake -fQstServ.mak
cd..
rem =====================
cd QstView2D
nmake -fQstView2D.mak
cd..
rem =====================
cd QuestLAB
nmake -fQuestLAB.mak
cd..
rem =====================
cd xIMGLAB
nmake -fxIMGLAB.mak
cd..
rem =====================
cd xImpScript
nmake -fxImpScript.mak
cd..
rem =====================
cd CrHackSet
nmake -fxCrHackSet.mak
cd..
rem =====================
cd OpenCV
nmake -fxOpenCV.mak
cd..
rem =====================
cd QstHash
nmake -fQstHashImp.mak
cd..
rem =====================
cd ResLoader
nmake -fResLoader.mak
cd..
rem =====================
cd ResRouter
nmake -fResRouter.mak
cd..
rem =====================
