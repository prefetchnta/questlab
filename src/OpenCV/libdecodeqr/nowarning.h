#ifndef __QR_NO_WARNING__
#define __QR_NO_WARNING__
#pragma warning (disable: 4100)
#pragma warning (disable: 4189)
#pragma warning (disable: 4238)
#pragma warning (disable: 4244)
#define _CRT_SECURE_NO_WARNINGS
#define __BEGIN__ {
#define __END__ goto exit; exit: ; }
#define cvWarpPerspectiveQMatrix cvGetPerspectiveTransform
#endif
