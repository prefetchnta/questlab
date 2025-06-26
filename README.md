
## 【基本概述】

　　一个试验性质的程序集。各个程序使用进程通讯的方式组合起来，实现一些有用的功能。作为项目 **GameViewer** 的后续版本，实现了一些游戏引擎。一个通用的数据分析逆向工具集。各种技术的大杂烩（评估的平台）。

　　以一个程序只做好一件事为主要设计思想，最终形成一个大而全的应用程序。实现可以自由添加任何功能，支持任何编程语言、开发工具、第三方框架做为扩展手段的宏伟目标。最后留下的核心只是一套统一的程序之间的交互协议，而不是某些编程语言、开发工具和第三方框架下的代码。

　　程序间协作的基本手段：(1) 使用网络通讯实现相互消息的通知。目前可以使用 **CMD / INI / XML** 形式的消息数据；(2) 使用磁盘文件实现非实时数据的交互；(3) 使用内存共享文件实现大量数据的交换。

## 【一些截图】

　　[SCREENSHOTS](https://github.com/prefetchnta/questlab/blob/master/pic/IMAGES.md "SCREENSHOTS")

## 【如何编译】

　　本系统内的项目全部使用 **Makefile** 作为工程的主要组织方式，没有必要不会使用 **IDE** 提供的工程文件。单个部件的编译可以执行项目目录下的批处理文件来实现。每个项目的编译环境要求见下面的说明。目前用到的开发环境有：

- **Borland C++Builder 6.0 Update4**
- **Microsoft Visual Studio 2010 SP1**
- **Microsoft Visual Studio 2013 Update5**
- **Microsoft Visual Studio 2022 Lastest**

## 【部件简介】

- **QuestLAB (VS2010 SP1)**
<p>　　系统启动程序 (离线)。在启动所有程序之前执行一些批处理命令很难实现的初始化操作。</p>

- **QstServ (VS2010 SP1)**
<p>　　消息广播服务器程序。负责把一个程序的消息广播给系统内其他程序，进程间网络交互的核心。</p>

- **QstLibs (VS2010 SP1)**
<p>　　基本功能函数封装库。提供基本功能给系统内或其他外部开发语言使用 (如 <b>VB / PB</b> 等)。</p>

- **QstMount (VS2010 SP1)**
<p>　　文件资源服务器程序。负责封包文件挂载，子文件加载，彻底解决包中包无限嵌套的问题。</p>

- **QstMenu (BCB6.0 UPD4)**
<p>　　整个系统的菜单程序。作为系统的主菜单与 <b>GUI</b> 外壳，负责各部件的调度与统一控制。</p>

- **QstCmdz (VS2010 SP1)**
<p>　　命令行消息发送程序。可以批量发送所有形式的消息，同时也是批命令脚本的执行工具。</p>

- **QstComm (VS2010 SP1)**
<p>　　通讯的收发监视程序。提供各种通讯接口的数据发送及数据接收的可视化功能。</p>

- **QstIndex (BCB6.0 UPD4)**
<p>　　正整数索引控制程序。可以控制与播放一个正整数索引值，主要用来控制多帧图片的帧序号。</p>

- **QstInfoz (BCB6.0 UPD4)**
<p>　　文本信息框显示程序。提供两个文本显示框，用于显示一些文字信息，如解析文件的信息。</p>

- **QstTree (BCB6.0 UPD4 + Virtual Treeview)**
<p>　　文件的树形列表程序。导入文件加载的主要发起者，<b>GameViewer</b> 文件列表的加强版。</p>

- **QstView2D (VS2010 SP1)**
<p>　　软件二维内容渲染器。实现数据的 <b>2D</b> 渲染及变换，<b>GameViewer 2D</b> 渲染部分的加强版。</p>

- **QstMedia (BCB6.0 UPD4)**
<p>　　音频视频文件播放器。实现音视频文件的播放，<b>GameViewer</b> 音频播放部分的加强版。</p>

- **QstText (BCB6.0 UPD4)**
<p>　　文本浏览执行编辑器。实现文本渲染与执行功能，<b>GameViewer</b> 文本浏览部分的加强版。</p>

- **QstData (BCB6.0 UPD4)**
<p>　　数据可感知探测工具。提供磁盘文件、封包内文件、字符串与二进制数据各种探测功能。</p>

- **QstHash (BCB6.0 UPD4)**
<p>　　数据哈希校验计算器。提供磁盘文件、封包内文件、字符串与二进制数据各种哈希功能。</p>

- **QstGraph (BCB6.0 UPD4)**
<p>　　数据的图表显示程序。提供一组不同形式的数据显示图表，用来渲染用户提供的各种数据。</p>

- **RunPython (VS2010 SP1)**
<p>　　<b>Python</b> 执行器 (离线)。用来运行 <b>Python 3.4</b> 的脚本扩展程序，使用自己目录下的库。</p>

- **RunLua (VS2010 SP1)**
<p>　　<b>Lua</b> 执行器 (离线)。用来运行 <b>Lua 5.3</b> 的脚本扩展程序，使用自己目录下的库。</p>

- **RunTcl (VS2010 SP1)**
<p>　　<b>Tcl</b> 执行器 (离线)。用来运行 <b>Tcl 8.6</b> 的脚本扩展程序，使用自己目录下的库。</p>

- **RunWebIE (BCB6.0 UPD4)**
<p>　　网页界面容器 (离线)。使用 <b>IE</b> 来做 <b>HTML</b> 渲染，实现一些原生程序很难做到的界面外壳。</p>

- **xSelectDir (BCB6.0 UPD4)**
<p>　　目录选择工具 (离线)。提供一个目录选择对话框，用于在某些操作中选择一个目录。</p>

- **xSelectFile (BCB6.0 UPD4)**
<p>　　文件选择工具 (离线)。提供一个文件选择对话框，用于在某些操作中选择一个文件。</p>

- **xImpScript (VS2010 SP1)**
<p>　　文件导入工具 (离线)。用于生成导入 <b>QstTree XML</b> 文件的工具，根据导入脚本来定制导入。</p>

- **xParamSet (BCB6.0 UPD4)**
<p>　　参数设置工具 (离线)。提供一个所有部件配置文件统一设置的 <b>GUI</b> 外壳。</p>

- **xIMGLAB (VS2010 SP1)**
<p>　　图像实验工具 (离线)。相对独立的一个工具，提供一个框架，通过插件来实现图像处理实验。</p>

- **ImportLib (XHTML + CSS)**
<p>　　导入脚本仓库 (离线)。基于网页的导入脚本列表，<b>GameViewer</b> 游戏导入列表的加强版。</p>

## 【插件介绍】

　　所有插件位于目录 **bin/plugin** 下。字母 **c** 开头的插件为命令扩展插件，用于扩展应用程序的消息命令； **f** 开头的插件为图片滤镜插件，用于实现二维图片的变换功能； **z** 开头的插件为 **CrHack/FMTz** 引擎插件，专门负责解析各类输入数据； **x** 开头的为复合插件，可以集中包含所有类型的插件功能模块。

## zKiriKiri (VS2010 SP1)

<p>支持加载 <b>KrKr</b> (吉里吉里) 游戏引擎里的几种文件格式：</p>

 - KiriKiri2 XP3 Archive (XP3)

## zFALCOM (VS2010 SP1)

<p>支持加载 <b>FALCOM</b> 公司开发的游戏里的几种专用的文件格式：</p>

 - FALCOM YS AIA Image File (\*.AIA)
 - FALCOM YS NNK Archive (\*.NI + \*.NA)
 - FALCOM ZLib Compressed file (\*.Z)

## zTGL (VS2010 SP1)

<p>支持加载 <b>TGL</b> 公司开发的游戏里的几种专用的文件格式：</p>

 - TGL IPAC Archive (IPAC)
 - TGL PAC_FILE Archive (PACF)
 - TGL LZSS Compressed file (IEL1)
 - TGL LZSS Compressed file (SDC)
 - TGL ACF0 Image File (0FCA)
 - TGL ACF1 Image File (1FCA)
 - TGL ACF2 Image File (2FCA)
 - TGL BMR Image File (BMR)
 - TGL CGR Image File (CGR)

## zEGO (VS2010 SP1)

<p>支持加载 <b>Studio e.go!</b> 公司开发的游戏里的几种文件格式：</p>

 - Studio e.go! Game Data Archive (32)
 - Studio e.go! Game Data Archive (64)

## zNScripter (VS2010 SP1)

<p>支持加载 <b>NScripter</b> 游戏引擎里的几种文件格式：</p>

 - Scripter3 ARchive (SAR)
 - NScripter Archive (NSA)
 - NScripter script file (nscript.dat)

## xOpenCV (VS2013 UPD5)

<p>基于 <b>OpenCV</b> 库的复合扩展插件，为 <b>QstView2D</b> 提供一些图像处理识别相关的图片滤镜和图形绘制命令扩展，并支持摄像头与视频文件的图片输入以及支持加载以下几种图片格式：</p>

 - Windows bitmaps - \*.bmp, \*.dib
 - JPEG files - \*.jpeg, \*.jpg, \*.jpe
 - JPEG 2000 files - \*.jp2
 - Portable Network Graphics - \*.png
 - WebP - \*.webp
 - Portable image format - \*.pbm, \*.pgm, \*.ppm
 - Sun rasters - \*.sr, \*.ras
 - TIFF files - \*.tiff, \*.tif

## xCrHackSet (VS2010 SP1)

<p>使用 <b>CrHack SDK</b> 的复合扩展插件，为 <b>QstView2D</b> 提供一些基本的图片滤镜和图形绘制命令扩展，通过这个插件32位程序可以调用一些64位的程序资源。</p>

## zExpand (VS2010 SP1)

<p>支持加载微软 <b>COMPRESS.EXE & EXPAND.EXE</b> 工具相关的几种文件格式：</p>

 - Microsoft Compressed (SZDD) file
 - Microsoft Compressed (SZ20) file

## zRAR (VS2010 SP1)

<p>基于 <b>UnRAR</b> 库的 <b>RAR</b> 封包文件读取插件，只支持一种文件格式：</p>

 - Roshal ARchive (RAR)

## zDShow (VS2010 SP1)

<p>基于 <b>DirectShow</b> 组件的音频视频播放插件，支持的文件格式取决于系统安装的解码器数量。</p>

 - Media file supported by DirectShow

## zFMOD (VS2010 SP1)

<p>基于 <b>FMOD Ex</b> 库的音频文件播放插件，支持的音频格式如下：</p>

 - AIFF - (Audio Interchange File Format)
 - ASF - (Advanced Streaming format, includes support for the audio tracks in video streams)
 - ASX - (playlist format - contains links to other audio files)
 - DLS - (DownLoadable Sound format for midi playback)
 - FLAC - (Lossless compression codec)
 - FSB - (FMOD sample bank format generated by FSBank and FMOD designer tool)
 - IT - (Impulse tracker sequenced mod format)
 - M3U - (playlist format - contains links to other audio files)
 - MID - (MIDI using operating system or custom DLS patches)
 - MOD - (Protracker / Fasttracker and others sequenced mod format)
 - MP2 - (MPEG I/II Layer 2)
 - MP3 - (MPEG I/II Layer 3, including VBR support)
 - OGG - (Ogg Vorbis format)
 - PLS - (playlist format - contains links to other audio files)
 - RAW - (Raw file format support)
 - S3M - (ScreamTracker 3 sequenced mod format)
 - VAG - (PS2 / PSP format, playable on all platforms)
 - WAV - (Microsoft Wave files, including compressed wavs)
 - WAX - (playlist format - contains links to other audio files)
 - WMA - (Windows Media Audio format)
 - XM - (FastTracker 2 sequenced format)
 - XMA - (Xbox 360 only)
 - XWMA - (Xbox 360 only)
 - AudioQueue - (Apple iOS devices only, supports AAC, ALAC and MP3 decoding in software and hardware)

## zDevIL (VS2010 SP1)

<p>基于 <b>DevIL / ResIL</b> 库的图片文件加载插件，支持的图片格式如下：</p>

 - Microsoft Windows Bitmap - .bmp extension
 - Dr. Halo - .cut extension
 - DooM walls - no specific extension
 - DooM flats - no specific extension
 - Microsoft Windows Icons and Cursors - .ico and .cur extensions
 - JPEG - .jpg, .jpe and .jpeg extensions
 - Amiga IFF (FORM ILBM) - .iff, .ilbm, .lbm extensions
 - Kodak PhotoCD - .pcd extension
 - ZSoft PCX - .pcx extension
 - PIC - .pic extension
 - Portable Network Graphics - .png extension
 - Portable Any Map - .pbm, .pgm, .ppm and .pnm extensions
 - Silicon Graphics - .sgi, .bw, .rgb and .rgba extensions
 - TrueVision Targa File - .tga, .vda, .icb and .vst extensions
 - Tagged Image File Format - .tif and .tiff extensions
 - C-Style Header - .h extension
 - Raw Image Data - any extension
 - Half-Life Model Texture - .mdl extension
 - Quake 2 Texture - .wal extension
 - Homeworld Texture - .lif extension
 - Multiple-image Network Graphics - .mng extension
 - Graphics Interchange Format - .gif extension
 - DirectDraw Surface - .dds extension
 - ZSoft Multi-PCX - .dcx extension
 - Adobe PhotoShop - .psd extension
 - PaintShop Pro - .psp extension
 - PIX - .pix extension
 - Pixar - .pxr extension
 - X Pixel Map - .xpm extension
 - Radiance High Dynamic Range - .hdr extension
 - Macintosh Icon - .icns extension
 - Jpeg 2000 - .jp2 extension
 - OpenEXR - .exr extension
 - Microsoft HD Photo - .wdp and .hdp extension
 - Valve Texture Format - .vtf extension
 - Wireless Bitmap - .wbmp extension
 - Sun Raster - .sun, .ras, .rs, .im1, .im8, .im24 and .im32 extensions
 - Interchange File Format - .iff extension
 - Gamecube Texture - .tpl extension
 - Flexible Image Transport System - .fit and .fits extensions
 - Digital Imaging and Communications in Medicine (DICOM) - .dcm and .dicom extensions
 - Call of Duty Infinity Ward Image - .iwi extension
 - Blizzard Texture Format - .blp extension
 - Heavy Metal: FAKK2 Texture - .ftx extension
 - Homeworld 2 - Relic Texture - .rot extension
 - Medieval II: Total War Texture - .texture extension
 - Digital Picture Exchange - .dpx extension
 - Unreal (and Unreal Tournament) Texture - .utx extension
 - MPEG-1 Audio Layer 3 - .mp3 extension

## zSYN (VS2003 SP1)

<p>支持加载 <b>Shin-ichi TSURUTA (SYN)</b> 设计的几种文件格式：</p>

 - G Compression Archive (GCA)
 - ARGB Image File (by Shin-ichi TSURUTA)

## zFreeImage (VS2010 SP1)

<p>基于 <b>FreeImage</b> 库的图片文件加载插件，支持的图片格式如下：</p>

 - Windows or OS/2 Bitmap File (\*.BMP)
 - Dr. Halo (\*.CUT)
 - DirectDraw Surface (\*.DDS)
 - ILM OpenEXR (\*.EXR)
 - Raw Fax format CCITT G3 (\*.G3)
 - Graphics Interchange Format (\*.GIF)
 - High Dynamic Range (\*.HDR)
 - Windows Icon (\*.ICO)
 - Amiga IFF (\*.IFF, \*.LBM)
 - JPEG-2000 codestream (\*.J2K, \*.J2C)
 - JPEG Network Graphics (\*.JNG)
 - JPEG-2000 File Format (\*.JP2)
 - Independent JPEG Group (\*.JPG, \*.JIF, \*.JPEG, \*.JPE)
 - JPEG XR image format (\*.JXR, \*.WDP, \*.HDP)
 - Commodore 64 Koala format (\*.KOA)
 - Multiple Network Graphics (\*.MNG)
 - Portable Bitmap (ASCII) (\*.PBM)
 - Portable Bitmap (BINARY) (\*.PBM)
 - Kodak PhotoCD (\*.PCD)
 - Zsoft Paintbrush PCX bitmap format (\*.PCX)
 - Portable Floatmap (\*.PFM)
 - Portable Graymap (ASCII) (\*.PGM)
 - Portable Graymap (BINARY) (\*.PGM)
 - Macintosh PICT (\*.PCT, \*.PICT, \*.PIC)
 - Portable Network Graphics (\*.PNG)
 - Portable Pixelmap (ASCII) (\*.PPM)
 - Portable Pixelmap (BINARY) (\*.PPM)
 - Adobe Photoshop (\*.PSD)
 - Sun Rasterfile (\*.RAS)
 - RAW camera image (many extensions)
 - Silicon Graphics SGI image format (\*.SGI)
 - Truevision Targa files (\*.TGA, \*.TARGA)
 - Tagged Image File Format (\*.TIF, \*.TIFF)
 - Wireless Bitmap (\*.WBMP)
 - Google WebP image format (\*.WEBP)
 - X11 Bitmap Format (\*.XBM)
 - X11 Pixmap Format (\*.XPM)

## CrH_FMTZ (VS2010 SP1)

<p>属于 <b>CrHack SDK</b> 本体的跨平台引擎插件，支持的文件格式如下 (只支持文件的常见模式)：</p>

 - Windows Bitmap File (BMP)
 - DirectDraw Surface (DDS)
 - Independent JPEG Group (JPG)
 - Zsoft Publisher's Paintbrush (PCX)
 - Portable Network Graphics (PNG)
 - Truevision Targa (TGA)
 - Phil Katz's ZIP Archive (PKZIP)

## 【基础组件】

- **CrH_CORE (VS2010 SP1)**
<p>　　属于 <b>CrHack SDK</b> 本体的跨平台核心基础库。</p>

- **CrH_MATH (VS2010 SP1)**
<p>　　属于 <b>CrHack SDK</b> 本体的跨平台运算函数库。</p>

- **CrH_GFX2 (VS2010 SP1)**
<p>　　属于 <b>CrHack SDK</b> 本体的跨平台软件 <b>2D</b> 渲染函数库。</p>

- **GFX2_GDI (VS2010 SP1)**
<p>　　使用 <b>Windows GDI</b> 作为图形接口的 <b>2D</b> 图形绘制插件。</p>

- **GFX2_DX8 (VS2010 SP1)**
<p>　　使用 <b>Direct3D 8</b> 作为图形接口的 <b>2D</b> 图形绘制插件。</p>

- **GFX2_DX9 (VS2010 SP1)**
<p>　　使用 <b>Direct3D 9</b> 作为图形接口的 <b>2D</b> 图形绘制插件。</p>

- **CrH_GFX3 (VS2010 SP1)**
<p>　　属于 <b>CrHack SDK</b> 本体的跨平台软件 <b>3D</b> 基础函数库。</p>

- **GFX3_D3D8 (VS2010 SP1)**
<p>　　使用 <b>Direct3D 8</b> 作为图形接口的 <b>3D</b> 图形绘制插件。</p>

- **GFX3_D3D9 (VS2010 SP1)**
<p>　　使用 <b>Direct3D 9</b> 作为图形接口的 <b>3D</b> 图形绘制插件。</p>

- **ResLoader (VS2010 SP1)**
<p>　　使用 <b>QuestLAB</b> 内部网络系统的外部资源文件加载执行插件。</p>

- **ResRouter (VS2010 SP1)**
<p>　　外部资源文件全局搜索路由插件。所有加载的封包文件都将被搜索。</p>

## 【附加组件】

- **CompLib (VS2010 SP1)**
<p>　　一些常用的数据压缩、解压缩函数封装库。</p>

- **HackLib (VS2010 SP1)**
<p>　　一些老旧的但常用的系统操作函数封装库。</p>

- **TessOCR (VS2010 SP1)**
<p>　　<b>Google Tesseract-OCR</b> 识别引擎封装库。</p>
