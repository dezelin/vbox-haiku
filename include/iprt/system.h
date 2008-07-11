/** @file
 * IPRT - System.
 */

/*
 * Copyright (C) 2006-2007 Sun Microsystems, Inc.
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 USA or visit http://www.sun.com if you need
 * additional information or have any questions.
 */

#ifndef ___iprt_system_h
#define ___iprt_system_h

#include <iprt/cdefs.h>
#include <iprt/types.h>


__BEGIN_DECLS

/** @defgroup grp_rt_system RTSystem - System Information
 * @ingroup grp_rt
 * @{
 */

/**
 * Gets the number of logical (not physical) processors in the system.
 *
 * @returns Number of logical processors in the system.
 *
 * @todo Replaced by RTMpGetOnlineCount / RTMpGetCount, retire this guy.
 */
RTDECL(unsigned) RTSystemProcessorGetCount(void);

/**
 * Gets the active logical processor mask.
 *
 * @returns Active logical processor mask. (bit 0 == logical cpu 0)
 *
 * @todo Replaced by RTMpGetOnlineSet, retire this guy.
 */
RTDECL(uint64_t) RTSystemProcessorGetActiveMask(void);

/** @} */

__END_DECLS

#endif

