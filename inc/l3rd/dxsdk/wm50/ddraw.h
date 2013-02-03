//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*==========================================================================;
 *
 *  File:       ddraw.h
 *  Content:    DirectDraw include file
 *
 ***************************************************************************/
#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


// Other components use this define to tell if they have the DirectDraw
// defintitions presents or if they have to re-define stuff.

#define __DDRAW_INCLUDED__

/*
 * GUIDS used by DirectDraw objects
 */

DEFINE_GUID(IID_IDirectDraw,                  0x9c59509a,0x39bd,0x11d1,0x8c,0x4a,0x00,0xc0,0x4f,0xd9,0x30,0xc5);
DEFINE_GUID(IID_IDirectDrawSurface,           0x0b0e83e4,0xf37f,0x11d2,0x8b,0x15,0x00,0xc0,0x4f,0x68,0x92,0x92);
DEFINE_GUID(IID_IDirectDrawPalette,           0x6C14DB84,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);
DEFINE_GUID(IID_IDirectDrawClipper,           0x6C14DB85,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);
DEFINE_GUID(IID_IDirectDrawColorControl,      0x4B9F0EE0,0x0D7E,0x11D0,0x9B,0x06,0x00,0xA0,0xC9,0x03,0xA3,0xB8);
DEFINE_GUID(IID_IDirectDrawGammaControl,      0x69C11C3E,0xB46B,0x11D1,0xAD,0x7A,0x00,0xC0,0x4F,0xC2,0x9B,0x4E);

/*
 * Forward interface declerations
 */

struct IDirectDraw;
struct IDirectDrawSurface;
struct IDirectDrawPalette;
struct IDirectDrawClipper;
struct IDirectDrawColorControl;
struct IDirectDrawGammaControl;

typedef struct IDirectDraw              *LPDIRECTDRAW;
typedef struct IDirectDrawSurface       *LPDIRECTDRAWSURFACE;
typedef struct IDirectDrawPalette		*LPDIRECTDRAWPALETTE;
typedef struct IDirectDrawClipper		*LPDIRECTDRAWCLIPPER;
typedef struct IDirectDrawColorControl	*LPDIRECTDRAWCOLORCONTROL;
typedef struct IDirectDrawGammaControl  *LPDIRECTDRAWGAMMACONTROL;

/*
 * DirectDraw Structures
 */

/*
 * DDCOLORKEY
 */

typedef struct _DDCOLORKEY
{
    DWORD	dwColorSpaceLowValue;	// low boundary of color space that is to
					                // be treated as Color Key, inclusive
    DWORD	dwColorSpaceHighValue;	// high boundary of color space that is
					                // to be treated as Color Key, inclusive
} DDCOLORKEY, * LPDDCOLORKEY;

/*
 * DDARGB
 * Generic pixel format with 8-bit RGB and alpha components
 */

typedef struct _DDARGB
{
    BYTE blue;
    BYTE green;
    BYTE red;
    BYTE alpha;

} DDARGB, *LPDDARGB;

/*
 * DDBLTFX
 */

typedef struct _DDBLTFX
{
    DWORD	    dwSize;                 // size of structure
    DWORD	    dwROP;                  // Win32 raster operations
    DWORD	    dwFillColor;            // color in RGB or Palettized (Brush value for Win32 ROPs)
    DDCOLORKEY	ddckDestColorkey;		// DestColorkey override
    DDCOLORKEY	ddckSrcColorkey;		// SrcColorkey override

} DDBLTFX, * LPDDBLTFX;

/*
 * DDALPHABLTFX
 */

typedef struct _DDALPHABLTFX
{
    DWORD       dwSize;                 // size of structure
    DDARGB      ddargbScaleFactors;     // Constant scaling factors
    DWORD       dwFillColor;            // color in ARGB or Palettized

} DDALPHABLTFX, * LPDDALPHABLTFX;

/*
 * DDSCAPS
 * Caps bits defined below.
 */

typedef struct _DDSCAPS
{
    DWORD dwCaps;

} DDSCAPS, * LPDDSCAPS;

/*
 * DDCAPS
 */

#define DD_ROP_SPACE (256/32)       // space required to store ROP array

typedef struct _DDCAPS
{
    DWORD	dwSize;			        // size of the DDCAPS structure

    // Surface capabilities

    DWORD	dwVidMemTotal;          // total amount of video memory
    DWORD	dwVidMemFree;           // amount of free video memory
    DWORD	dwVidMemStride;         // video memory stride (0 if linear)

    DDSCAPS ddsCaps;                // surface caps

    DWORD	dwNumFourCCCodes;       // number of four cc codes

    // Palette capabilities

    DWORD	dwPalCaps;              // palette capabilities

    // Hardware blitting capabilities

    DWORD	dwBltCaps;              // driver specific capabilities
    DWORD	dwCKeyCaps;		        // color key blitting capabilities
    DWORD	dwAlphaCaps;	        // alpha blitting capabilities
    DWORD	dwRops[DD_ROP_SPACE];	// ROPS supported

    // Overlay capabilities

    DWORD   dwOverlayCaps;          // general overlay capabilities.

    DWORD	dwMaxVisibleOverlays;	// maximum number of visible overlays
    DWORD	dwCurrVisibleOverlays;	// current number of visible overlays

    DWORD	dwAlignBoundarySrc;	    // source rectangle alignment
    DWORD	dwAlignSizeSrc;		    // source rectangle byte size
    DWORD	dwAlignBoundaryDest;	// dest rectangle alignment
    DWORD	dwAlignSizeDest;	    // dest rectangle byte size

    DWORD	dwMinOverlayStretch;	// minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
    DWORD	dwMaxOverlayStretch;	// maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3

    // Miscalenous capabilies

    DWORD   dwMiscCaps;

} DDCAPS, * LPDDCAPS;

/*
 * DDPIXELFORMAT
 */

typedef struct _DDPIXELFORMAT
{
    DWORD	dwSize;			// size of structure
    DWORD	dwFlags;		// pixel format flags
    DWORD	dwFourCC;		// (FOURCC code)
    union
    {
	DWORD	dwRGBBitCount;		// how many bits per pixel
	DWORD	dwYUVBitCount;		// how many bits per pixel
	DWORD	dwAlphaBitDepth;	// how many bits for alpha channels
    };
    union
    {
	DWORD	dwRBitMask;		// mask for red bit
	DWORD	dwYBitMask;		// mask for Y bits
    };
    union
    {
	DWORD	dwGBitMask;		// mask for green bits
	DWORD	dwUBitMask;		// mask for U bits
    };
    union
    {
	DWORD	dwBBitMask;		// mask for blue bits
	DWORD	dwVBitMask;		// mask for V bits
	};
    union
    {
	DWORD	dwRGBAlphaBitMask;	// mask for alpha channel
    };

} DDPIXELFORMAT, * LPDDPIXELFORMAT;

/*
 * DDPIXELFORMAT dwFlags values.
 */

#define DDPF_ALPHAPIXELS		0x00000001
#define DDPF_ALPHA				0x00000002
#define DDPF_FOURCC				0x00000004
#define DDPF_PALETTEINDEXED     0x00000020
#define DDPF_RGB				0x00000040
#define DDPF_ALPHAPREMULT 		0x00008000

#define DDPF_VALID              (DDPF_ALPHAPIXELS	 | \
                                 DDPF_ALPHA			 | \
                                 DDPF_FOURCC         | \
                                 DDPF_PALETTEINDEXED | \
                                 DDPF_RGB			 | \
                                 DDPF_ALPHAPREMULT)

/*
 * DDSURFACEDESC
 */

typedef struct _DDSURFACEDESC
{
    DWORD		  dwSize;               // size of the DDSURFACEDESC structure
    DWORD		  dwFlags;              // determines what fields are valid
    DWORD		  dwHeight;             // height of surface to be created
    DWORD		  dwWidth;              // width of input surface
    LONG		  lPitch;               // bytes to next line down (return value only)
    LONG          lXPitch;              // bytes to next pixel right (return value only)
    DWORD		  dwBackBufferCount;    // number of back buffers requested
	DWORD		  dwRefreshRate;        // refresh rate (used when display mode is described)
    LPVOID		  lpSurface;            // pointer to the associated surface memory
    DDCOLORKEY    ddckCKDestOverlay;    // color key for destination overlay use
    DDCOLORKEY    ddckCKDestBlt;        // color key for destination blt use
    DDCOLORKEY    ddckCKSrcOverlay;     // color key for source overlay use
    DDCOLORKEY    ddckCKSrcBlt;         // color key for source blt use
    DDPIXELFORMAT ddpfPixelFormat;      // pixel format description of the surface
    DDSCAPS		  ddsCaps;              // direct draw surface capabilities
    DWORD		  dwSurfaceSize;        // Surface size, in bytes

} DDSURFACEDESC, * LPDDSURFACEDESC;

/*
 * DDSURFACEDESC dwFlags values.
 */

#define DDSD_CAPS		        0x00000001
#define DDSD_HEIGHT		        0x00000002
#define DDSD_WIDTH		        0x00000004
#define DDSD_PITCH		        0x00000008
#define DDSD_XPITCH             0x00000010
#define DDSD_BACKBUFFERCOUNT	0x00000020
#define DDSD_LPSURFACE		    0x00000800
#define DDSD_PIXELFORMAT	    0x00001000
#define DDSD_CKDESTOVERLAY	    0x00002000
#define DDSD_CKDESTBLT		    0x00004000
#define DDSD_CKSRCOVERLAY	    0x00008000
#define DDSD_CKSRCBLT		    0x00010000
#define DDSD_REFRESHRATE	    0x00040000
#define DDSD_SURFACESIZE	    0x00080000

#define DDSD_VALID              (DDSD_CAPS              | \
                                 DDSD_HEIGHT            | \
                                 DDSD_WIDTH             | \
                                 DDSD_PITCH             | \
                                 DDSD_XPITCH            | \
                                 DDSD_BACKBUFFERCOUNT   | \
                                 DDSD_LPSURFACE		    | \
                                 DDSD_PIXELFORMAT	    | \
                                 DDSD_CKDESTOVERLAY	    | \
                                 DDSD_CKDESTBLT		    | \
                                 DDSD_CKSRCOVERLAY	    | \
                                 DDSD_CKSRCBLT		    | \
                                 DDSD_REFRESHRATE		| \
                                 DDSD_SURFACESIZE)

#define DDSD_ENUM_VALID (DDSD_CAPS              | \
                         DDSD_HEIGHT            | \
                         DDSD_WIDTH             | \
                         DDSD_PIXELFORMAT)

/*
 * DDOVERLAYFX
 */

typedef struct _DDOVERLAYFX
{
    DWORD	   dwSize;                  // size of structure

    DWORD	   dwAlphaConstBitDepth;    // Bit depth used to specify alpha constant.
	DWORD	   dwAlphaConst;            // Constant to use as alpha channel.

    DDCOLORKEY dckDestColorkey;         // DestColorkey override
    DDCOLORKEY dckSrcColorkey;          // DestColorkey override

} DDOVERLAYFX, * LPDDOVERLAYFX;

/*
 * DDGAMMARAMP
 */

typedef struct _DDGAMMARAMP
{
    WORD red[256];
    WORD green[256];
    WORD blue[256];

} DDGAMMARAMP, * LPDDGAMMARAMP;

/*
 *  This is the structure within which DirectDraw returns data about the current graphics driver and chipset
 */

#define MAX_DDDEVICEID_STRING 512

typedef struct _DDDEVICEIDENTIFIER
{
    /*
     * These elements are for presentation to the user only. They should not be used to identify particular
     * drivers, since this is unreliable and many different strings may be associated with the same
     * device, and the same driver from different vendors.
     */

    wchar_t    szDriver[MAX_DDDEVICEID_STRING];
    wchar_t    szDescription[MAX_DDDEVICEID_STRING];

    /*
     * This element is the version of the DirectDraw/3D driver. It is legal to do <, > comparisons
     * on the whole 64 bits. Caution should be exercised if you use this element to identify problematic
     * drivers. It is recommended that guidDeviceIdentifier is used for this purpose.
     *
     * This version has the form:
     *  wProduct = HIWORD(liDriverVersion.HighPart)
     *  wVersion = LOWORD(liDriverVersion.HighPart)
     *  wSubVersion = HIWORD(liDriverVersion.LowPart)
     *  wBuild = LOWORD(liDriverVersion.LowPart)
     */

    LARGE_INTEGER liDriverVersion;


    /*
     * These elements can be used to identify particular chipsets. Use with extreme caution. 
     *   dwVendorId     Identifies the manufacturer. May be zero if unknown.
     *   dwDeviceId     Identifies the type of chipset. May be zero if unknown.
     *   dwSubSysId     Identifies the subsystem, typically this means the particular board. May be zero if unknown.
     *   dwRevision     Identifies the revision level of the chipset. May be zero if unknown.
     */

    DWORD   dwVendorId;
    DWORD   dwDeviceId;
    DWORD   dwSubSysId;
    DWORD   dwRevision;

    /*
     * This element can be used to check changes in driver/chipset. This GUID is a unique identifier for the
     * driver/chipset pair. Use this element if you wish to track changes to the driver/chipset in order to
     * reprofile the graphics subsystem.
     * This element can also be used to identify particular problematic drivers.
     */

    GUID    guidDeviceIdentifier;

} DDDEVICEIDENTIFIER, * LPDDDEVICEIDENTIFIER;

/*
 * DDCOLORCONTROL
 */

typedef struct _DDCOLORCONTROL
{
    DWORD 		dwSize;
    DWORD		dwFlags;
    LONG		lBrightness;
    LONG		lContrast;
    LONG		lHue;
    LONG 		lSaturation;
    LONG		lSharpness;
    LONG		lGamma;
    LONG		lColorEnable;

} DDCOLORCONTROL, * LPDDCOLORCONTROL;

/*
 * DDCOLORCONTROL dwFlags values.
 */

#define DDCOLOR_BRIGHTNESS		0x00000001
#define DDCOLOR_CONTRAST		0x00000002
#define DDCOLOR_HUE			    0x00000004
#define DDCOLOR_SATURATION		0x00000008
#define DDCOLOR_SHARPNESS		0x00000010
#define DDCOLOR_GAMMA			0x00000020
#define DDCOLOR_COLORENABLE		0x00000040

#define DDCOLOR_VALID           (DDCOLOR_BRIGHTNESS     | \
                                 DDCOLOR_CONTRAST       | \
                                 DDCOLOR_HUE            | \
                                 DDCOLOR_SATURATION     | \
                                 DDCOLOR_SHARPNESS      | \
                                 DDCOLOR_GAMMA          | \
                                 DDCOLOR_COLORENABLE)

/*
 * API's
 */

typedef BOOL (PASCAL * LPDDENUMCALLBACKEX)(LPGUID, LPWSTR, LPWSTR, LPVOID, HMONITOR);
extern HRESULT WINAPI DirectDrawEnumerateEx(LPDDENUMCALLBACKEX lpCallback, LPVOID lpContext, DWORD dwFlags);

extern HRESULT WINAPI DirectDrawCreate(LPGUID lpGUID, LPDIRECTDRAW *lplpDD, IUnknown *pUnkOuter);
extern HRESULT WINAPI DirectDrawCreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER *lplpDDClipper, IUnknown *pUnkOuter);

/*
 * DirectDrawEnumerateEx Flags
 */

/*
 * This flag causes enumeration of any GDI display devices which are part of
 * the Windows Desktop
 */
#define DDENUM_ATTACHEDSECONDARYDEVICES     0x00000001

/*
 * This flag causes enumeration of any GDI display devices which are not
 * part of the Windows Desktop
 */
#define DDENUM_DETACHEDSECONDARYDEVICES     0x00000002

#define DDENUM_VALID                        (DDENUM_ATTACHEDSECONDARYDEVICES | \
                                             DDENUM_DETACHEDSECONDARYDEVICES)

/*
 * Enumeration function pointer types
 */

typedef HRESULT (FAR PASCAL * LPDDENUMMODESCALLBACK)(LPDDSURFACEDESC, LPVOID);
typedef HRESULT (FAR PASCAL * LPDDENUMSURFACESCALLBACK)(LPDIRECTDRAWSURFACE, LPDDSURFACEDESC, LPVOID);

/*
 * IDirectDraw
 */

#undef INTERFACE
#define INTERFACE IDirectDraw
DECLARE_INTERFACE_(IDirectDraw, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDraw methods ***/
    STDMETHOD(CreateClipper)(THIS_ DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *) PURE;
    STDMETHOD(CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *) PURE;
    STDMETHOD(CreateSurface)(THIS_ LPDDSURFACEDESC, LPDIRECTDRAWSURFACE *, IUnknown *) PURE;
    STDMETHOD(EnumDisplayModes)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK) PURE;
    STDMETHOD(EnumSurfaces)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK) PURE;
    STDMETHOD(FlipToGDISurface)(THIS) PURE;
    STDMETHOD(GetCaps)(THIS_ LPDDCAPS, LPDDCAPS) PURE;
    STDMETHOD(GetDisplayMode)(THIS_ LPDDSURFACEDESC) PURE;
    STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE *) PURE;
    STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetScanLine)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL) PURE;
    STDMETHOD(RestoreDisplayMode)(THIS) PURE;
    STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD) PURE;
    STDMETHOD(SetDisplayMode)(THIS_ DWORD, DWORD,DWORD, DWORD, DWORD) PURE;
    STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD, HANDLE) PURE;
    STDMETHOD(GetAvailableVidMem)(THIS_ LPDDSCAPS, LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetSurfaceFromDC)(THIS_ HDC, LPDIRECTDRAWSURFACE *) PURE;
    STDMETHOD(RestoreAllSurfaces)(THIS) PURE;
    STDMETHOD(TestCooperativeLevel)(THIS) PURE;
    STDMETHOD(GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER, DWORD ) PURE;
};

/*
 * IDirectDrawPalette
 */

#undef INTERFACE
#define INTERFACE IDirectDrawPalette
DECLARE_INTERFACE_(IDirectDrawPalette, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDrawPalette methods ***/
    STDMETHOD(GetCaps)(THIS_ LPDWORD) PURE;
    STDMETHOD(GetEntries)(THIS_ DWORD, DWORD, DWORD, LPPALETTEENTRY) PURE;
    STDMETHOD(SetEntries)(THIS_ DWORD, DWORD, DWORD, LPPALETTEENTRY) PURE;
};

/*
 * IDirectDrawClipper
 */

#undef INTERFACE
#define INTERFACE IDirectDrawClipper
DECLARE_INTERFACE_(IDirectDrawClipper, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDrawClipper methods ***/
    STDMETHOD(GetClipList)(THIS_ LPRECT, LPRGNDATA, LPDWORD) PURE;
    STDMETHOD(GetHWnd)(THIS_ HWND *) PURE;
    STDMETHOD(IsClipListChanged)(THIS_ LPBOOL) PURE;
    STDMETHOD(SetClipList)(THIS_ LPRGNDATA, DWORD) PURE;
    STDMETHOD(SetHWnd)(THIS_ DWORD, HWND) PURE;
};

/*
 * IDirectDrawSurface
 */

#undef INTERFACE
#define INTERFACE IDirectDrawSurface
DECLARE_INTERFACE_(IDirectDrawSurface, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDrawSurface methods ***/
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT) PURE;
    STDMETHOD(Blt)(THIS_ LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX) PURE;
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID, LPDDENUMSURFACESCALLBACK) PURE;
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) PURE;
    STDMETHOD(Flip)(THIS_ LPDIRECTDRAWSURFACE, DWORD) PURE;
    STDMETHOD(GetBltStatus)(THIS_ DWORD) PURE;
    STDMETHOD(GetCaps)(THIS_ LPDDSCAPS) PURE;
    STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER *) PURE;
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY) PURE;
    STDMETHOD(GetDC)(THIS_ HDC *) PURE;
    STDMETHOD(GetFlipStatus)(THIS_ DWORD) PURE;
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG) PURE;
    STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE *) PURE;
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT) PURE;
    STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC) PURE;
    STDMETHOD(IsLost)(THIS) PURE;
    STDMETHOD(Lock)(THIS_ LPRECT, LPDDSURFACEDESC, DWORD, HANDLE) PURE;
    STDMETHOD(ReleaseDC)(THIS_ HDC) PURE;
    STDMETHOD(Restore)(THIS) PURE;
    STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER) PURE;
    STDMETHOD(SetColorKey)(THIS_ DWORD, LPDDCOLORKEY) PURE;
    STDMETHOD(SetOverlayPosition)(THIS_ LONG, LONG ) PURE;
    STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE) PURE;
    STDMETHOD(Unlock)(THIS_ LPRECT) PURE;
    STDMETHOD(UpdateOverlay)(THIS_ LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) PURE;
    STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD, LPDIRECTDRAWSURFACE) PURE;
    STDMETHOD(GetDDInterface)(THIS_ LPDIRECTDRAW *) PURE;
    STDMETHOD(AlphaBlt)(THIS_ LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDALPHABLTFX) PURE;
};

/*
 * IDirectDrawColorControl
 */

#undef INTERFACE
#define INTERFACE IDirectDrawColorControl
DECLARE_INTERFACE_(IDirectDrawColorControl, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDrawColorControl methods ***/
    STDMETHOD(GetColorControls)(THIS_ LPDDCOLORCONTROL) PURE;
    STDMETHOD(SetColorControls)(THIS_ LPDDCOLORCONTROL) PURE;
};

/*
 * IDirectDrawGammaControl
 */

#undef INTERFACE
#define INTERFACE IDirectDrawGammaControl
DECLARE_INTERFACE_(IDirectDrawGammaControl, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS)  PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirectDrawColorControl methods ***/
    STDMETHOD(GetGammaRamp)(THIS_ DWORD, LPDDGAMMARAMP) PURE;
    STDMETHOD(SetGammaRamp)(THIS_ DWORD, LPDDGAMMARAMP) PURE;
};

/*
 * DirectDraw Macros
 */

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                                \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |         \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

/*
 * Direct Draw Capability Flags
 */

/*
 * Surface Capability Flags
 */

#define DDSCAPS_ALPHA				    0x00000001
#define DDSCAPS_BACKBUFFER			    0x00000002
#define DDSCAPS_FLIP				    0x00000004
#define DDSCAPS_FRONTBUFFER			    0x00000008
#define DDSCAPS_OVERLAY				    0x00000010
#define DDSCAPS_PALETTE				    0x00000020
#define DDSCAPS_PRIMARYSURFACE		    0x00000040
#define DDSCAPS_SYSTEMMEMORY		    0x00000080
#define DDSCAPS_VIDEOMEMORY			    0x00000100
#define DDSCAPS_WRITEONLY			    0x00000200
#define DDSCAPS_READONLY                0x00000800
#define DDSCAPS_NOTUSERLOCKABLE         0x00002000
#define DDSCAPS_DYNAMIC                 0x00004000

#define DDSCAPS_VALID                   (DDSCAPS_ALPHA				 | \
                                         DDSCAPS_BACKBUFFER			 | \
                                         DDSCAPS_FLIP				 | \
                                         DDSCAPS_FRONTBUFFER		 | \
                                         DDSCAPS_OVERLAY			 | \
                                         DDSCAPS_PALETTE			 | \
                                         DDSCAPS_PRIMARYSURFACE		 | \
                                         DDSCAPS_SYSTEMMEMORY		 | \
                                         DDSCAPS_VIDEOMEMORY		 | \
                                         DDSCAPS_WRITEONLY			 | \
                                         DDSCAPS_READONLY            | \
                                         DDSCAPS_NOTUSERLOCKABLE     | \
                                         DDSCAPS_DYNAMIC)

/*
 * Palette Capability Flags
 */

#define DDPCAPS_PRIMARYSURFACE	0x00000010
#define DDPCAPS_ALPHA			0x00000400

#define DDPCAPS_VALID           (DDPCAPS_PRIMARYSURFACE | \
                                 DDPCAPS_ALPHA)

/*
 * DirectDraw Capability Flags
 */

/*
 * General hardware blitting capabilities (For DDCAPS dwBltCaps field)
 */

#define DDBLTCAPS_READSYSMEM            0x00000001
#define DDBLTCAPS_WRITESYSMEM           0x00000002
#define DDBLTCAPS_FOURCCTORGB           0x00000004
#define DDBLTCAPS_COPYFOURCC            0x00000008
#define DDBLTCAPS_FILLFOURCC            0x00000010

#define DDBLTCAPS_VALID                 (DDBLTCAPS_READSYSMEM  | \
                                         DDBLTCAPS_WRITESYSMEM | \
                                         DDBLTCAPS_FOURCCTORGB | \
                                         DDBLTCAPS_COPYFOURCC  | \
                                         DDBLTCAPS_FILLFOURCC)

/*
 * Hardware color key blitting capabilities (For DDCAPS dwCKeyCaps field)
 */

#define DDCKEYCAPS_DESTBLT              0x00000001
#define DDCKEYCAPS_DESTBLTCLRSPACE      0x00000002
#define DDCKEYCAPS_DESTBLTCLRSPACEYUV   0x00000004
#define DDCKEYCAPS_SRCBLT               0x00000200
#define DDCKEYCAPS_SRCBLTCLRSPACE       0x00000400
#define DDCKEYCAPS_SRCBLTCLRSPACEYUV    0x00000800
#define DDCKEYCAPS_BOTHBLT              0x00001000

#define DDCKEYCAPS_VALID                (DDCKEYCAPS_DESTBLT            | \
                                         DDCKEYCAPS_DESTBLTCLRSPACE    | \
                                         DDCKEYCAPS_DESTBLTCLRSPACEYUV | \
                                         DDCKEYCAPS_SRCBLT             | \
                                         DDCKEYCAPS_SRCBLTCLRSPACE     | \
                                         DDCKEYCAPS_SRCBLTCLRSPACEYUV  | \
                                         DDCKEYCAPS_BOTHBLT)

/*
 * Hardware alpha blitting capabilities (For DDCAPS dwAlphaCaps field)
 */

#define DDALPHACAPS_ALPHAPIXELS         0x00000001
#define DDALPHACAPS_ALPHASURFACE        0x00000002
#define DDALPHACAPS_ALPHAPALETTE        0x00000004
#define DDALPHACAPS_ALPHACONSTANT       0x00000008
#define DDALPHACAPS_ARGBSCALE           0x00000010
#define DDALPHACAPS_SATURATE            0x00000020
#define DDALPHACAPS_PREMULT             0x00000040
#define DDALPHACAPS_NONPREMULT          0x00000080
#define DDALPHACAPS_ALPHAFILL           0x00000800
#define DDALPHACAPS_ALPHANEG            0x00000100

#define DDALPHACAPS_VALID               (DDALPHACAPS_ALPHAPIXELS   | \
                                         DDALPHACAPS_ALPHASURFACE  | \
                                         DDALPHACAPS_ALPHAPALETTE  | \
                                         DDALPHACAPS_ALPHACONSTANT | \
                                         DDALPHACAPS_ARGBSCALE     | \
                                         DDALPHACAPS_SATURATE      | \
                                         DDALPHACAPS_PREMULT       | \
                                         DDALPHACAPS_NONPREMULT    | \
                                         DDALPHACAPS_ALPHAFILL     | \
                                         DDALPHACAPS_ALPHANEG)

/*
 * Overlay capabilities ( For DDCAPS dwOverlayCaps field)
 */

#define DDOVERLAYCAPS_FLIP                  0x00000001
#define DDOVERLAYCAPS_FOURCC                0x00000004
#define DDOVERLAYCAPS_ZORDER                0x00000008
#define DDOVERLAYCAPS_MIRRORLEFTRIGHT       0x00000010
#define DDOVERLAYCAPS_MIRRORUPDOWN          0x00000020
#define DDOVERLAYCAPS_CKEYSRC               0x00000040
#define DDOVERLAYCAPS_CKEYSRCCLRSPACE       0x00000080
#define DDOVERLAYCAPS_CKEYSRCCLRSPACEYUV    0x00000100
#define DDOVERLAYCAPS_CKEYDEST              0x00000200
#define DDOVERLAYCAPS_CKEYDESTCLRSPACE      0x00000400
#define DDOVERLAYCAPS_CKEYDESTCLRSPACEYUV   0x00000800
#define DDOVERLAYCAPS_CKEYBOTH              0x00001000
#define DDOVERLAYCAPS_ALPHADEST             0x00002000
#define DDOVERLAYCAPS_ALPHASRC              0x00008000
#define DDOVERLAYCAPS_ALPHADESTNEG          0x00002000
#define DDOVERLAYCAPS_ALPHASRCNEG           0x00008000
#define DDOVERLAYCAPS_ALPHACONSTANT         0x00010000
#define DDOVERLAYCAPS_ALPHAPREMULT          0x00040000
#define DDOVERLAYCAPS_ALPHANONPREMULT       0x00080000
#define DDOVERLAYCAPS_ALPHAANDKEYDEST       0x00100000
#define DDOVERLAYCAPS_OVERLAYSUPPORT        0x80000000

#define DDOVERLAYCAPS_VALID                 (DDOVERLAYCAPS_FLIP                | \
                                             DDOVERLAYCAPS_FOURCC              | \
                                             DDOVERLAYCAPS_ZORDER              | \
                                             DDOVERLAYCAPS_MIRRORLEFTRIGHT     | \
                                             DDOVERLAYCAPS_MIRRORUPDOWN        | \
                                             DDOVERLAYCAPS_CKEYSRC             | \
                                             DDOVERLAYCAPS_CKEYSRCCLRSPACE     | \
                                             DDOVERLAYCAPS_CKEYSRCCLRSPACEYUV  | \
                                             DDOVERLAYCAPS_CKEYDEST            | \
                                             DDOVERLAYCAPS_CKEYDESTCLRSPACE    | \
                                             DDOVERLAYCAPS_CKEYDESTCLRSPACEYUV | \
                                             DDOVERLAYCAPS_CKEYBOTH            | \
                                             DDOVERLAYCAPS_ALPHADEST           | \
                                             DDOVERLAYCAPS_ALPHASRC            | \
                                             DDOVERLAYCAPS_ALPHADESTNEG        | \
                                             DDOVERLAYCAPS_ALPHASRCNEG         | \
                                             DDOVERLAYCAPS_ALPHACONSTANT       | \
                                             DDOVERLAYCAPS_ALPHAPREMULT        | \
                                             DDOVERLAYCAPS_ALPHANONPREMULT     | \
                                             DDOVERLAYCAPS_ALPHAANDKEYDEST     | \
                                             DDOVERLAYCAPS_OVERLAYSUPPORT)

/*
 * Miscellaneous Capability Flags (For DDCAPS dwMiscCaps member)
 */

#define DDMISCCAPS_READSCANLINE         0x00000001
#define DDMISCCAPS_READMONITORFREQ      0x00000002
#define DDMISCCAPS_READVBLANKSTATUS     0x00000004
#define DDMISCCAPS_FLIPINTERVAL         0x00000008
#define DDMISCCAPS_FLIPODDEVEN          0x00000010
#define DDMISCCAPS_FLIPVSYNCWITHVBI     0x00000020
#define DDMISCCAPS_COLORCONTROLOVERLAY  0x00000040
#define DDMISCCAPS_COLORCONTROLPRIMARY  0x00000080
#define DDMISCCAPS_GAMMACONTROLOVERLAY  0x00000100
#define DDMISCCAPS_GAMMACONTROLPRIMARY  0x00000200

#define DDMISCCAPS_VALID                (DDMISCCAPS_READSCANLINE        | \
                                         DDMISCCAPS_READMONITORFREQ     | \
                                         DDMISCCAPS_READVBLANKSTATUS    | \
                                         DDMISCCAPS_FLIPINTERVAL        | \
                                         DDMISCCAPS_FLIPODDEVEN         | \
                                         DDMISCCAPS_FLIPVSYNCWITHVBI    | \
                                         DDMISCCAPS_COLORCONTROLOVERLAY | \
                                         DDMISCCAPS_COLORCONTROLPRIMARY | \
                                         DDMISCCAPS_GAMMACONTROLOVERLAY | \
                                         DDMISCCAPS_GAMMACONTROLPRIMARY)

/*
 * DirectDraw method flags
 */

/*
 * Flags for IDirectDraw::EnumSurfaces
 */

#define DDENUMSURFACES_ALL			    0x00000001
#define DDENUMSURFACES_MATCH			0x00000002
#define DDENUMSURFACES_NOMATCH			0x00000004
#define DDENUMSURFACES_CANBECREATED		0x00000008
#define DDENUMSURFACES_DOESEXIST		0x00000010

#define DDENUMSURFACES_VALID			(DDENUMSURFACES_ALL			 | \
                                         DDENUMSURFACES_MATCH		 | \
                                         DDENUMSURFACES_NOMATCH		 | \
                                         DDENUMSURFACES_CANBECREATED | \
                                         DDENUMSURFACES_DOESEXIST)

/*
 * Flags for IDirectDraw::SetCooperativeLevel
 */

#define DDSCL_NORMAL				0x00000000
#define DDSCL_FULLSCREEN			0x00000001

#define DDSCL_VALID                 (DDSCL_FULLSCREEN | \
                                     DDSCL_NORMAL)

/*
 * Flags for IDirectDraw::WaitForVerticalBlank
 */

#define DDWAITVB_BLOCKBEGIN			0x00000001
#define DDWAITVB_BLOCKEND			0x00000004

#define DDWAITVB_VALID              (DDWAITVB_BLOCKBEGIN | \
                                     DDWAITVB_BLOCKEND)

/*
 * Flags for IDirectDrawSurface::GetColorKey and IDirectDrawSurface::SetColorKey
 */

#define DDCKEY_COLORSPACE	0x00000001
#define DDCKEY_DESTBLT		0x00000002
#define DDCKEY_DESTOVERLAY	0x00000004
#define DDCKEY_SRCBLT		0x00000008
#define DDCKEY_SRCOVERLAY	0x00000010

#define DDCKEY_VALID		(DDCKEY_DESTBLT		| \
                             DDCKEY_DESTOVERLAY	| \
                             DDCKEY_SRCBLT		| \
                             DDCKEY_SRCOVERLAY)

/*
 * Flags for IDirectDrawSurface::Blt
 */

#define DDBLT_COLORFILL         0x00000400
#define DDBLT_KEYDEST			0x00002000
#define DDBLT_KEYDESTOVERRIDE	0x00004000
#define DDBLT_KEYSRC			0x00008000
#define DDBLT_KEYSRCOVERRIDE	0x00010000
#define DDBLT_ROP				0x00020000
#define DDBLT_WAITNOTBUSY       0x01000000
#define DDBLT_WAITVSYNC         0x00000001

#define DDBLT_VALID             (DDBLT_COLORFILL       | \
                                 DDBLT_KEYDEST		   | \
                                 DDBLT_KEYDESTOVERRIDE | \
                                 DDBLT_KEYSRC		   | \
                                 DDBLT_KEYSRCOVERRIDE  | \
                                 DDBLT_ROP			   | \
                                 DDBLT_WAITNOTBUSY     | \
                                 DDBLT_WAITVSYNC)

/*
 * Flags for IDirectDrawSurface::AlphaBlt
 */

#define DDABLT_NOBLEND 				0x02000000
#define DDABLT_COLORFILL			0x00100000
#define DDABLT_ALPHADESTNEG			0x00000004
#define DDABLT_ALPHASRCNEG			0x00000080
#define DDABLT_WAITNOTBUSY          0x01000000
#define DDABLT_WAITVSYNC            0x00000001

#define DDABLT_VALID			    (DDABLT_NOBLEND 		| \
                                     DDABLT_COLORFILL		| \
                                     DDABLT_ALPHADESTNEG	| \
                                     DDABLT_ALPHASRCNEG		| \
                                     DDABLT_WAITNOTBUSY     | \
                                     DDABLT_WAITVSYNC)

/*
 * Flags for IDirectDrawSurface::Flip
 */

#define DDFLIP_EVEN                          0x00000002
#define DDFLIP_ODD                           0x00000004
#define DDFLIP_INTERVAL1                     0x01000000
#define DDFLIP_INTERVAL2                     0x02000000
#define DDFLIP_INTERVAL4                     0x04000000
#define DDFLIP_WAITNOTBUSY                   0x00000008
#define DDFLIP_WAITVSYNC                     0x00000001

#define DDFLIP_VALID                         (DDFLIP_EVEN         | \
                                              DDFLIP_ODD          | \
                                              DDFLIP_INTERVAL1    | \
                                              DDFLIP_INTERVAL2    | \
                                              DDFLIP_INTERVAL4    | \
                                              DDFLIP_WAITNOTBUSY  | \
                                              DDFLIP_WAITVSYNC)

#define DDFLIP_INTERVAL3                     (DDFLIP_INTERVAL1 | \
                                              DDFLIP_INTERVAL2)

#define DDFLIP_INTERVALMASK                  (DDFLIP_INTERVAL1 | \
                                              DDFLIP_INTERVAL2 | \
                                              DDFLIP_INTERVAL4)

/*
 * Flag values for IDirectDrawSurface::UpdateOverlay
 */

#define DDOVER_ALPHADEST			    0x00000001
#define DDOVER_ALPHADESTNEG			    0x00000002
#define DDOVER_ALPHASRC				    0x00000004
#define DDOVER_ALPHASRCNEG			    0x00000008
#define DDOVER_ALPHACONSTOVERRIDE	    0x00000010
#define DDOVER_HIDE				        0x00000020
#define DDOVER_KEYDEST				    0x00000040
#define DDOVER_KEYDESTOVERRIDE			0x00000080
#define DDOVER_KEYSRC				    0x00000100
#define DDOVER_KEYSRCOVERRIDE			0x00000200
#define DDOVER_SHOW				        0x00000400
#define DDOVER_MIRRORLEFTRIGHT		    0x00001000
#define DDOVER_MIRRORUPDOWN			    0x00002000
#define DDOVER_WAITNOTBUSY              0x00004000
#define DDOVER_WAITVSYNC                0x00008000

#define DDOVER_VALID                    (DDOVER_ALPHADEST          | \
                                         DDOVER_ALPHADESTNEG       | \
                                         DDOVER_ALPHASRC           | \
                                         DDOVER_ALPHASRCNEG        | \
                                         DDOVER_ALPHACONSTOVERRIDE | \
                                         DDOVER_HIDE               | \
                                         DDOVER_KEYDEST            | \
                                         DDOVER_KEYDESTOVERRIDE    | \
                                         DDOVER_KEYSRC             | \
                                         DDOVER_KEYSRCOVERRIDE     | \
                                         DDOVER_SHOW               | \
                                         DDOVER_MIRRORLEFTRIGHT    | \
                                         DDOVER_MIRRORUPDOWN       | \
                                         DDOVER_WAITNOTBUSY        | \
                                         DDOVER_WAITVSYNC)

/*
 * Flags for IDirectDrawSurface::Lock
 */

#define DDLOCK_READONLY             0x00000001
#define DDLOCK_WRITEONLY            0x00000002
#define DDLOCK_DISCARD              0x00000004
#define DDLOCK_WAITNOTBUSY          0x00000008

#define DDLOCK_VALID				(DDLOCK_READONLY    | \
                                     DDLOCK_WRITEONLY   | \
                                     DDLOCK_DISCARD     | \
                                     DDLOCK_WAITNOTBUSY)

/*
 * Flags for IDirectDrawSurface::GetFlipStatus
 */

#define DDGFS_CANFLIP			0x00000001
#define DDGFS_ISFLIPDONE		0x00000002

#define DDGFS_VALID             (DDGFS_CANFLIP | \
                                 DDGFS_ISFLIPDONE)


/*
 * Flags for IDirectDrawSurface::GetBltStatus
 */

#define DDGBS_CANBLT			0x00000001
#define DDGBS_ISBLTDONE			0x00000002

#define DDGBS_VALID             (DDGBS_CANBLT | \
                                 DDGBS_ISBLTDONE)

/*
 * Flags for IDirectDrawSurface::EnumOverlayZOrders
 */

#define DDENUMOVERLAYZ_FRONTTOBACK	0x00000001

#define DDENUMOVERLAYZ_VALID        DDENUMOVERLAYZ_FRONTTOBACK

/*
 * Flags for IDirectDrawSurface::UpdateOverlayZOrder
 */

#define DDOVERZ_SENDTOFRONT		    0x00000000
#define DDOVERZ_SENDTOBACK		    0x00000001
#define DDOVERZ_MOVEFORWARD		    0x00000002
#define DDOVERZ_MOVEBACKWARD		0x00000003
#define DDOVERZ_INSERTINFRONTOF		0x00000004
#define DDOVERZ_INSERTINBACKOF		0x00000005

/*
 * DirectDraw Return Codes
 */

/*
 * Enumeration function return values.
 */

#define DDENUMRET_CANCEL			((HRESULT)(0))
#define DDENUMRET_OK				((HRESULT)(1))

/*
 * DirectDraw error codes.
 */

#define _FACDD	0x876
#define MAKE_DDHRESULT( code )	MAKE_HRESULT( 1, _FACDD, code )

#define DD_OK                               NOERROR

#define DDERR_CURRENTLYNOTAVAIL		        MAKE_DDHRESULT( 40 )
#define DDERR_GENERIC				        E_FAIL
#define DDERR_HEIGHTALIGN			        MAKE_DDHRESULT( 90 )
#define DDERR_INCOMPATIBLEPRIMARY		    MAKE_DDHRESULT( 95 )
#define DDERR_INVALIDCAPS			        MAKE_DDHRESULT( 100 )
#define DDERR_INVALIDCLIPLIST			    MAKE_DDHRESULT( 110 )
#define DDERR_INVALIDMODE			        MAKE_DDHRESULT( 120 )
#define DDERR_INVALIDOBJECT			        MAKE_DDHRESULT( 130 )
#define DDERR_INVALIDPARAMS			        E_INVALIDARG
#define DDERR_INVALIDPIXELFORMAT		    MAKE_DDHRESULT( 145 )
#define DDERR_INVALIDRECT			        MAKE_DDHRESULT( 150 )
#define DDERR_LOCKEDSURFACES			    MAKE_DDHRESULT( 160 )
#define DDERR_NOCLIPLIST			        MAKE_DDHRESULT( 205 )
#define DDERR_NOALPHAHW				        MAKE_DDHRESULT( 180 )
#define DDERR_NOCOLORCONVHW			        MAKE_DDHRESULT( 210 )
#define DDERR_NOCOOPERATIVELEVELSET		    MAKE_DDHRESULT( 212 )
#define DDERR_NOCOLORKEYHW			        MAKE_DDHRESULT( 215 )
#define DDERR_NOFLIPHW				        MAKE_DDHRESULT( 230 )
#define DDERR_NOTFOUND				        MAKE_DDHRESULT( 255 )
#define DDERR_NOOVERLAYHW			        MAKE_DDHRESULT( 260 )
#define DDERR_OVERLAPPINGRECTS			    MAKE_DDHRESULT( 270 )
#define DDERR_NORASTEROPHW			        MAKE_DDHRESULT( 280 )
#define DDERR_NOSTRETCHHW			        MAKE_DDHRESULT( 310 )
#define DDERR_NOVSYNCHW				        MAKE_DDHRESULT( 335 )
#define DDERR_NOZOVERLAYHW			        MAKE_DDHRESULT( 350 )
#define DDERR_OUTOFCAPS				        MAKE_DDHRESULT( 360 )
#define DDERR_OUTOFMEMORY			        E_OUTOFMEMORY
#define DDERR_OUTOFVIDEOMEMORY			    MAKE_DDHRESULT( 380 )
#define DDERR_PALETTEBUSY			        MAKE_DDHRESULT( 387 )
#define DDERR_COLORKEYNOTSET			    MAKE_DDHRESULT( 400 )
#define DDERR_SURFACEBUSY			        MAKE_DDHRESULT( 430 )
#define DDERR_CANTLOCKSURFACE               MAKE_DDHRESULT( 435 )
#define DDERR_SURFACELOST			        MAKE_DDHRESULT( 450 )
#define DDERR_TOOBIGHEIGHT			        MAKE_DDHRESULT( 470 )
#define DDERR_TOOBIGSIZE			        MAKE_DDHRESULT( 480 )
#define DDERR_TOOBIGWIDTH			        MAKE_DDHRESULT( 490 )
#define DDERR_UNSUPPORTED			        E_NOTIMPL
#define DDERR_UNSUPPORTEDFORMAT             MAKE_DDHRESULT( 536 )
#define DDERR_VERTICALBLANKINPROGRESS		MAKE_DDHRESULT( 537 )
#define DDERR_WASSTILLDRAWING			    MAKE_DDHRESULT( 540 )
#define DDERR_DIRECTDRAWALREADYCREATED		MAKE_DDHRESULT( 562 )
#define DDERR_PRIMARYSURFACEALREADYEXISTS	MAKE_DDHRESULT( 564 )
#define DDERR_REGIONTOOSMALL			    MAKE_DDHRESULT( 566 )
#define DDERR_CLIPPERISUSINGHWND		    MAKE_DDHRESULT( 567 )
#define DDERR_NOCLIPPERATTACHED			    MAKE_DDHRESULT( 568 )
#define DDERR_NOPALETTEATTACHED			    MAKE_DDHRESULT( 572 )
#define DDERR_NOPALETTEHW			        MAKE_DDHRESULT( 573 )
#define DDERR_NOBLTHW				        MAKE_DDHRESULT( 575 )
#define DDERR_OVERLAYNOTVISIBLE			    MAKE_DDHRESULT( 577 )
#define DDERR_NOOVERLAYDEST			        MAKE_DDHRESULT( 578 )
#define DDERR_INVALIDPOSITION			    MAKE_DDHRESULT( 579 )
#define DDERR_NOTAOVERLAYSURFACE		    MAKE_DDHRESULT( 580 )
#define DDERR_EXCLUSIVEMODEALREADYSET		MAKE_DDHRESULT( 581 )
#define DDERR_NOTFLIPPABLE			        MAKE_DDHRESULT( 582 )
#define DDERR_NOTLOCKED				        MAKE_DDHRESULT( 584 )
#define DDERR_CANTCREATEDC			        MAKE_DDHRESULT( 585 )
#define DDERR_NODC				            MAKE_DDHRESULT( 586 )
#define DDERR_WRONGMODE				        MAKE_DDHRESULT( 587 )
#define DDERR_IMPLICITLYCREATED			    MAKE_DDHRESULT( 588 )
#define DDERR_NOTPALETTIZED			        MAKE_DDHRESULT( 589 )
#define DDERR_DCALREADYCREATED			    MAKE_DDHRESULT( 620 )
#define DDERR_MOREDATA         			    MAKE_DDHRESULT( 690 )
#define DDERR_VIDEONOTACTIVE   		    	MAKE_DDHRESULT( 695 )
#define DDERR_DEVICEDOESNTOWNSURFACE   		MAKE_DDHRESULT( 699 )

#ifdef __cplusplus
};
#endif
