/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */


/*
 * How this all works...
 *
 * This directory implements three different interfaces to Chromium:
 *
 * 1. the Chromium interface - this is defined by the functions that start
 *    with the "cr" prefix and are defined in chromium.h and implemented in
 *    stub.c.  Typically, this is used by parallel apps (like psubmit).
 *
 * 2. GLX emulation interface - the glX*() functions are emulated here.
 *    When glXCreateContext() is called we may either create a real, native
 *    GLX context or a Chromium context (depending on match_window_title and
 *    minimum_window_size).
 *
 * 3. WGL emulation interface - the wgl*() functions are emulated here.
 *    When wglCreateContext() is called we may either create a real, native
 *    WGL context or a Chromium context (depending on match_window_title and
 *    minimum_window_size).
 *
 *
 */


#ifndef CR_STUB_H
#define CR_STUB_H

#include "chromium.h"
#include "cr_version.h"
#include "cr_hash.h"
#include "cr_process.h"
#include "cr_spu.h"
#include "cr_threads.h"
#include "spu_dispatch_table.h"

#ifdef GLX
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xfixes.h>
#endif

#if defined(WINDOWS) || defined(Linux) || defined(SunOS)
# define CR_NEWWINTRACK
#endif

#if !defined(CHROMIUM_THREADSAFE) && defined(CR_NEWWINTRACK)
# error CHROMIUM_THREADSAFE have to be defined
#endif

/*#define VBOX_TEST_MEGOO*/

#if 0 && defined(CR_NEWWINTRACK) && !defined(WINDOWS)
#define XLOCK(dpy) XLockDisplay(dpy)
#define XUNLOCK(dpy) XUnlockDisplay(dpy)
#else
#define XLOCK(dpy)
#define XUNLOCK(dpy)
#endif

/* When we first create a rendering context we can't be sure whether
 * it'll be handled by Chromium or as a native GLX/WGL context.  So in
 * CreateContext() we'll mark the ContextInfo object as UNDECIDED then
 * switch it to either NATIVE or CHROMIUM the first time MakeCurrent()
 * is called.  In MakeCurrent() we can use a criteria like window size
 * or window title to decide between CHROMIUM and NATIVE.
 */
typedef enum
{
    UNDECIDED,
    CHROMIUM,
    NATIVE
} ContextType;

#define MAX_DPY_NAME 1000

typedef struct context_info_t ContextInfo;
typedef struct window_info_t WindowInfo;

#ifdef GLX
typedef struct glxpixmap_info_t GLX_Pixmap_t;

struct glxpixmap_info_t
{
    int x, y;
    unsigned int w, h, border, depth;
    GLenum format;
    Window root;
    GLenum target;
    GC gc;
    Pixmap hShmPixmap; /* Shared memory pixmap object, if it's supported*/
    Damage hDamage;    /* damage xserver handle*/
    Bool   bPixmapImageDirty;
    Region pDamageRegion;
};
#endif

struct context_info_t
{
    char dpyName[MAX_DPY_NAME];
    GLint spuContext;  /* returned by head SPU's CreateContext() */
    ContextType type;  /* CHROMIUM, NATIVE or UNDECIDED */
    unsigned long id;          /* the client-visible handle */
    GLint visBits;
    WindowInfo *currentDrawable;

#ifdef WINDOWS
    HGLRC hglrc;
#elif defined(DARWIN)
    ContextInfo *share;
    CGLContextObj cglc;

    /* CGLContextEnable (CGLEnable, CGLDisable, and CGLIsEnabled) */
    unsigned int options;

    /* CGLContextParameter (CGLSetParameter and CGLGetParameter) */
    GLint parambits;
    long swap_rect[4], swap_interval;
    unsigned long client_storage;
    long surf_order, surf_opacy;

    long disp_mask;
#elif defined(GLX)
    Display *dpy;
    ContextInfo *share;
    XVisualInfo *visual;
    Bool direct;
    GLXContext glxContext;
    CRHashTable *pGLXPixmapsHash;
    Bool     damageInitFailed;
    Display *damageDpy; /* second display connection to read xdamage extension data */
    int      damageEventsBase;
#endif
};

#ifdef DARWIN
enum {
    VISBIT_SWAP_RECT,
    VISBIT_SWAP_INTERVAL,
    VISBIT_CLIENT_STORAGE
};
#endif

struct window_info_t
{
    char dpyName[MAX_DPY_NAME];
    int x, y;
    unsigned int width, height;
    ContextType type;
    GLint spuWindow;       /* returned by head SPU's WindowCreate() */
    ContextInfo *pOwner;     /* ctx which created this window */
    GLboolean mapped;
#ifdef WINDOWS
    HDC      drawable;
    HRGN     hVisibleRegion;
    DWORD    dmPelsWidth;
    DWORD    dmPelsHeight;
    HWND     hWnd;
#elif defined(DARWIN)
    CGSConnectionID connection;
    CGSWindowID  drawable;
    CGSSurfaceID surface;
#elif defined(GLX)
    Display *dpy;
# ifdef CR_NEWWINTRACK
    Display *syncDpy;
# endif
    GLXDrawable drawable;
    XRectangle *pVisibleRegions;
    GLint cVisibleRegions;
#endif
#ifdef CR_NEWWINTRACK
    uint32_t u32ClientID;
#endif
};

/* "Global" variables for the stub library */
typedef struct {
    /* the first SPU in the SPU chain on this app node */
    SPU *spu;

    /* OpenGL/SPU dispatch tables */
    crOpenGLInterface wsInterface;
    SPUDispatchTable spuDispatch;
    SPUDispatchTable nativeDispatch;
    GLboolean haveNativeOpenGL;

    /* config options */
    int appDrawCursor;
    GLuint minChromiumWindowWidth;
    GLuint minChromiumWindowHeight;
    GLuint maxChromiumWindowWidth;
    GLuint maxChromiumWindowHeight;
    GLuint matchChromiumWindowCount;
    GLuint matchChromiumWindowCounter;
    GLuint *matchChromiumWindowID;
    GLuint numIgnoreWindowID;
    char *matchWindowTitle;
    int ignoreFreeglutMenus;
    int trackWindowSize;
    int trackWindowPos;
    int trackWindowVisibility;
    int trackWindowVisibleRgn;
    char *spu_dir;
    int force_pbuffers;
    int viewportHack;

    /* thread safety stuff */
    GLboolean threadSafe;
#ifdef CHROMIUM_THREADSAFE
    CRtsd dispatchTSD;
    CRmutex mutex;
#endif

    CRpid mothershipPID;

    /* contexts */
    int freeContextNumber;
    CRHashTable *contextTable;
    ContextInfo *currentContext; /* may be NULL */

    /* windows */
    CRHashTable *windowTable;

#ifdef GLX
    /* Shared memory, used to transfer XServer pixmaps data into client memory */
    XShmSegmentInfo xshmSI;
    GLboolean       bShmInitFailed;

    CRHashTable     *pGLXPixmapsHash;

    GLboolean       bXExtensionsChecked;
    GLboolean       bHaveXComposite;
    GLboolean       bHaveXFixes;
#endif

#ifdef WINDOWS
# ifndef CR_NEWWINTRACK
    HHOOK           hMessageHook;
# endif
# ifdef VBOX_WITH_WDDM
    bool            bRunningUnderWDDM;
# endif
#endif

#ifdef CR_NEWWINTRACK
    RTTHREAD        hSyncThread;
    bool volatile   bShutdownSyncThread;
#endif

} Stub;


extern Stub stub;
extern DECLEXPORT(SPUDispatchTable) glim;
extern SPUDispatchTable stubThreadsafeDispatch;
extern DECLEXPORT(SPUDispatchTable) stubNULLDispatch;

#if defined(GLX) || defined (WINDOWS)
extern GLboolean stubUpdateWindowVisibileRegions(WindowInfo *pWindow);
#endif

#ifdef WINDOWS

/* WGL versions */
extern WindowInfo *stubGetWindowInfo( HDC drawable );

extern void stubInstallWindowMessageHook();
extern void stubUninstallWindowMessageHook();

#elif defined(DARWIN)

extern CGSConnectionID    _CGSDefaultConnection(void);
extern OSStatus CGSGetWindowLevel( CGSConnectionID cid, CGSWindowID wid, CGWindowLevel *level );
extern OSStatus CGSSetWindowAlpha( const CGSConnectionID cid, CGSWindowID wid, float alpha );

/* These don't seem to be included in the OSX glext.h ... */
extern void glPointParameteri( GLenum pname, GLint param );
extern void glPointParameteriv( GLenum pname, const GLint * param );

extern WindowInfo *stubGetWindowInfo( CGSWindowID drawable );

#elif defined(GLX)

/* GLX versions */
extern WindowInfo *stubGetWindowInfo( Display *dpy, GLXDrawable drawable );
extern void stubUseXFont( Display *dpy, Font font, int first, int count, int listbase );
extern Display* stubGetWindowDisplay(WindowInfo *pWindow);

extern void stubCheckXExtensions(WindowInfo *pWindow);
#endif


extern ContextInfo *stubNewContext( const char *dpyName, GLint visBits, ContextType type, unsigned long shareCtx );
extern void stubDestroyContext( unsigned long contextId );
extern GLboolean stubMakeCurrent( WindowInfo *window, ContextInfo *context );
extern GLint stubNewWindow( const char *dpyName, GLint visBits );
extern void stubSwapBuffers(WindowInfo *window, GLint flags);
extern void stubGetWindowGeometry(WindowInfo *win, int *x, int *y, unsigned int *w, unsigned int *h);
extern GLboolean stubUpdateWindowGeometry(WindowInfo *pWindow, GLboolean bForceUpdate);
extern GLboolean stubIsWindowVisible(WindowInfo *win);
extern bool stubInit(void);

extern void APIENTRY stub_GetChromiumParametervCR( GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values );

extern void APIENTRY glBoundsInfoCR(const CRrecti *, const GLbyte *, GLint, GLint);

#endif /* CR_STUB_H */
