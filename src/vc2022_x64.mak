##
##=====================================
##      公用定义项
##=====================================
##
L=^\
C=c
O=obj
P=cpp

##
##=====================================
##      命令行工具
##=====================================
##
RC=rc.exe /nologo
MT=mt.exe /nologo
CC=cl.exe /nologo
PP=cl.exe /nologo
AR=lib.exe /nologo
LD=link.exe /nologo

##
##=====================================
##      静态库生成公用参数
##=====================================
##
AFLAGS=/OUT:$(LIB_NAME) /LTCG /LIBPATH:.

##
##=====================================
##      资源编译的公用参数
##=====================================
##
RFLAGS=/d "UNICODE" /d "_UNICODE" /fo.\$(PROJECT).res .\$(PROJECT).rc

##
##=====================================
##      MANIFESTs 公用参数
##=====================================
##
MFLAGS=/outputresource:$(BIN_NAME)^;^#2 /manifest ".\\2.manifest"

##
##=====================================
##      编译命令行公用参数
##=====================================
##
CFLAGS=/c /O2 /Oi /Ot /GF /GL /GS /Gy /Oy- /WX- /Gm- /MD /W4 \
       /wd4819 /Zc:forScope /Zc:inline /fp:precise /arch:AVX /arch:AVX2 \
       /I "..\..\..\CrHack\inc" /I "..\..\..\CrHack\inc\l3rd\dxsdk" \
       /D "NDEBUG" /D "WIN64" /D "_WIN64" /D "UNICODE" /D "_UNICODE" \
       /errorReport:prompt

##
##=====================================
##      可执行生成公用参数
##=====================================
##
LFLAGS=/OUT:$(BIN_NAME) /INCREMENTAL:NO /SUBSYSTEM:$(SUBSYSTEM) \
       /MANIFEST /MANIFESTFILE:".\\2.manifest" /OPT:REF /OPT:ICF \
       /LTCG /DYNAMICBASE /NXCOMPAT /ERRORREPORT:PROMPT /MACHINE:X64 \
       /LIBPATH:..\..\lib\x64lib kernel32.lib user32.lib gdi32.lib \
       comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib \
       CrH_CORE.lib
