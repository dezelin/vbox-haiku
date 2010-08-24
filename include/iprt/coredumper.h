/** @file
 * IPRT - Core Dumper.
 */

/*
 * Copyright (C) 2010 Oracle Corporation
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
 */

#ifndef ___iprt_coredumper_h
#define ___iprt_coredumper_h

#include <iprt/cdefs.h>
#include <iprt/types.h>

RT_C_DECLS_BEGIN

/** @defgroup grp_rt_coredumper     RTCoreDumper - Core Dumper.
 * @ingroup grp_rt
 * @{
 */

/**
 * Take a core dump of the current process without terminating it.
 *
 * @returns IPRT status code.
 * @param   pszOutputFile       Name of the core file.  If NULL use the
 *                              default naming scheme.
 */
RTDECL(int) RTCoreDumperTakeDump(const char *pszOutputFile);

/**
 * Sets up and enables the core dumper.
 *
 * Installs signal / unhandled exception handlers for catching fatal errors
 * that should result in a core dump.  If you wish to install your own handlers
 * you should do that after calling this function and make sure you pass on
 * events you don't handle.
 *
 * This can be called multiple times to change the settings without needing to
 * call RTCoreDumperDisable in between.
 *
 * @param   pszOutputDir        The directory to store the cores in.  If NULL
 *                              the current directory will be used.
 * @param   pszBaseName         Base file name, no directory.  If NULL the
 *                              dumper will generate an appropriate name.
 * @param   fFlags              Reserved for later, MBZ.
 */
RTDECL(int) RTCoreDumperSetup(const char *pszOutputDir, uint32_t fFlags);

/**
 * Disables the core dumper, i.e. undoes what RTCoreDumperSetup did.
 *
 * @returns IPRT status code.
 */
RTDECL(int) RTCoreDumperDisable(void);

/** @} */

RT_C_DECLS_END

#endif
