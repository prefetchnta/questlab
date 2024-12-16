/*
***************************************
*   Asylum Project @ 2013-12-09
***************************************
*/

#ifndef __ASYLUM_HPP__
#define __ASYLUM_HPP__

#include "crhack.h"
#include "util/egui.h"

/* System Filter */
#if !defined(_CR_SYS32_) && \
    !defined(_CR_SYS64_)
    #error "asylum.hpp: system not supported!"
#endif
#if !defined(_CR_CC_MSC_) && \
    !defined(_CR_CC_GCC_)
    #error "asylum.hpp: compiler not supported!"
#endif
#if !defined(_CR_OS_UNIX_) && \
    !defined(_CR_OS_WIN32_) && \
    !defined(_CR_OS_WIN64_)
    #error "asylum.hpp: os type not supported!"
#endif

/* Import CrHack Library */
#ifndef _CR_NO_PRAGMA_LIB_
    #ifndef ASY_USE_STATIC
        #pragma comment (lib, "CrH_CORE.lib")
        #pragma comment (lib, "CrH_UTIL.lib")
    #else
        #pragma comment (lib, "COREs.lib")
        #pragma comment (lib, "UTILs.lib")
    #endif
#endif

/* Include */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(_CR_CC_MSC_)
    #include <intrin.h>
    typedef dist_t  ssize_t;
#endif
#if defined(_CR_OS_MSWIN_)
    #include <process.h>
    #include <windows.h>
#else
    #include <time.h>
    #include <errno.h>
    #include <sched.h>
    #include <pthread.h>
#endif

/* BASIC STUFF */
#include "basic/basic.hpp"
#include "basic/defer.hpp"
#include "basic/safer.hpp"

/* CPU STUFF */
#include "cpu/thread.hpp"
#include "cpu/atomic.hpp"
#include "cpu/crisec.hpp"
#include "cpu/events.hpp"
#include "cpu/exlock.hpp"
#include "cpu/mtlock.hpp"
#include "cpu/nolock.hpp"
#include "cpu/rwlock.hpp"
#include "cpu/splock.hpp"
#include "cpu/doonce.hpp"
#include "cpu/mtpool.hpp"

/* CrHack C++ Wrapper */
#include "crhack/ascall.hpp"
#include "crhack/iports.hpp"
#if defined(ASY_USE_GDI)
    #include "gfx2/gdiwin.h"
    #include "crhack/gfx2_gdi.hpp"
#endif
#if defined(ASY_USE_DX8)
    #include "gfx2/dx8win.h"
    #include "crhack/gfx2_dx8.hpp"
#endif
#if defined(ASY_USE_DX9)
    #include "gfx2/dx9win.h"
    #include "crhack/gfx2_dx9.hpp"
#endif

/* Yet Another Wheel */
#include "yaw/array.hpp"
#include "yaw/bring.hpp"
#include "yaw/compr.hpp"
#include "yaw/list2.hpp"
#include "yaw/map2d.hpp"
#include "yaw/map_a.hpp"
#include "yaw/map_l.hpp"
#include "yaw/oring.hpp"
#include "yaw/table.hpp"
#include "yaw/tree_a.hpp"
#include "yaw/tree_l.hpp"
#include "yaw/map_acs.hpp"
#include "yaw/msg_pack.hpp"
#include "yaw/mt_queue.hpp"
#include "yaw/mt_stack.hpp"

/* Trade System */
#include "trade/kdays.hpp"

#endif  /* __ASYLUM_HPP__ */
