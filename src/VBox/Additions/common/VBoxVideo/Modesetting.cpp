/* $Id$ */
/** @file
 * VirtualBox Video driver, common code - HGSMI initialisation and helper
 * functions.
 */

/*
 * Copyright (C) 2006-2010 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#include <VBox/VBoxVideoGuest.h>
#include <VBox/VBoxVideo.h>
#include <VBox/VBoxGuest.h>
#include <VBox/Hardware/VBoxVideoVBE.h>
#include <VBox/VMMDev.h>

#include <iprt/asm.h>
#include <iprt/log.h>

/**
 * Gets the count of virtual monitors attached to the guest via an HGSMI
 * command
 *
 * @returns the right count on success or 1 on failure.
 * @param  pCtx  the context containing the heap to use
 */
RTDECL(uint32_t) VBoxHGSMIGetMonitorCount(PHGSMIGUESTCOMMANDCONTEXT pCtx)
{
    /* Query the configured number of displays. */
    uint32_t cDisplays = 0;
    VBoxQueryConfHGSMI(pCtx, VBOX_VBVA_CONF32_MONITOR_COUNT, &cDisplays);
    LogFunc(("cDisplays = %d\n", cDisplays));
    if (cDisplays == 0 || cDisplays > VBOX_VIDEO_MAX_SCREENS)
        /* Host reported some bad value. Continue in the 1 screen mode. */
        cDisplays = 1;
    return cDisplays;
}


/**
 * Tell the host about how VRAM is divided up between each screen via an HGSMI
 * command.  It is acceptable to specifiy identical data for each screen if
 * they share a single framebuffer.
 *
 * @returns iprt status code, either VERR_NO_MEMORY or the status returned by
 *          @a pfnFill
 * @param  pCtx      the context containing the heap to use
 * @param  u32Count  the number of screens we are activating
 * @param  pfnFill   a callback which initialises the VBVAINFOVIEW structures
 *                   for all screens
 * @param  pvData    context data for @a pfnFill
 */
RTDECL(int) VBoxHGSMISendViewInfo(PHGSMIGUESTCOMMANDCONTEXT pCtx,
                                  uint32_t u32Count,
                                  PFNHGSMIFILLVIEWINFO pfnFill,
                                  void *pvData)
{
    int rc;
    /* Issue the screen info command. */
    void *p = VBoxHGSMIBufferAlloc(pCtx, sizeof(VBVAINFOVIEW) * u32Count,
                                   HGSMI_CH_VBVA, VBVA_INFO_VIEW);
    if (p)
    {
        VBVAINFOVIEW *pInfo = (VBVAINFOVIEW *)p;
        rc = pfnFill(pvData, pInfo, u32Count);
        if (RT_SUCCESS(rc))
            VBoxHGSMIBufferSubmit (pCtx, p);
        VBoxHGSMIBufferFree(pCtx, p);
    }
    else
        rc = VERR_NO_MEMORY;
    return rc;
}


/**
 * Set a video mode using port registers.  This must be done for the first
 * screen before every HGSMI modeset and also works when HGSM is not enabled.
 * @param  cWidth    the mode width
 * @param  cHeight   the mode height
 * @param  cBPP      the colour depth of the mode
 * @param  cx        the horizontal panning offset
 * @param  cy        the vertical panning offset
 */
RTDECL(void) VBoxVideoSetModeRegisters(uint16_t cWidth, uint16_t cHeight,
                                       uint16_t cVirtWidth, uint16_t cBPP,
                                       uint16_t cx, uint16_t cy)
{
    /* set the mode characteristics */
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_XRES);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cWidth);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_YRES);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cHeight);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_VIRT_WIDTH);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cVirtWidth);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_BPP);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cBPP);
    /* enable the mode */
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_ENABLE);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
    /* Panning registers */
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_X_OFFSET);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cx);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_INDEX, VBE_DISPI_INDEX_Y_OFFSET);
    VBoxVideoCmnPortWriteUshort(VBE_DISPI_IOPORT_DATA, cy);
    /** @todo read from the port to see if the mode switch was successful */
}


/**
 * Set a video mode via an HGSMI request.  The views must have been
 * initialised first using @a VBoxHGSMISendViewInfo and if the mode is being
 * set on the first display then it must be set first using registers.
 * @param  cDisplay  the screen number
 * @param  cOriginX  the horizontal displacement relative to the first screen
 * @param  cOriginY  the vertical displacement relative to the first screen
 * @param  offStart  the offset of the visible area of the framebuffer
 *                   relative to the framebuffer start
 * @param  cbPitch   the offset in bytes between the starts of two adjecent
 *                   scan lines in video RAM
 * @param  cWidth    the mode width
 * @param  cHeight   the mode height
 * @param  cBPP      the colour depth of the mode
 */
RTDECL(void) VBoxHGSMIProcessDisplayInfo(PHGSMIGUESTCOMMANDCONTEXT pCtx,
                                         uint32_t cDisplay,
                                         int32_t  cOriginX,
                                         int32_t  cOriginY,
                                         uint32_t offStart,
                                         uint32_t cbPitch,
                                         uint32_t cWidth,
                                         uint32_t cHeight,
                                         uint16_t cBPP)
{
    /* Issue the screen info command. */
    void *p = VBoxHGSMIBufferAlloc(pCtx,
                                   sizeof (VBVAINFOSCREEN),
                                   HGSMI_CH_VBVA,
                                   VBVA_INFO_SCREEN);
    if (!p)
    {
        LogFunc(("HGSMIHeapAlloc failed\n"));
    }
    else
    {
        VBVAINFOSCREEN *pScreen = (VBVAINFOSCREEN *)p;

        pScreen->u32ViewIndex    = cDisplay;
        pScreen->i32OriginX      = cOriginX;
        pScreen->i32OriginY      = cOriginY;
        pScreen->u32StartOffset  = offStart;
        pScreen->u32LineSize     = cbPitch;
        pScreen->u32Width        = cWidth;
        pScreen->u32Height       = cHeight;
        pScreen->u16BitsPerPixel = cBPP;
        pScreen->u16Flags        = VBVA_SCREEN_F_ACTIVE;

        VBoxHGSMIBufferSubmit(pCtx, p);

        VBoxHGSMIBufferFree(pCtx, p);
    }
}