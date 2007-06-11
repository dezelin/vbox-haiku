/** @file
 * innotek Portable Runtime - alloca().
 */

/*
 * Copyright (C) 2006-2007 innotek GmbH
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation,
 * in version 2 as it comes in the "COPYING" file of the VirtualBox OSE
 * distribution. VirtualBox OSE is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * If you received this file as part of a commercial VirtualBox
 * distribution, then only the terms of your commercial VirtualBox
 * license agreement apply instead of the previous paragraph.
 */

#ifndef __iprt_alloca_h__
#define __iprt_alloca_h__


/*
 * If there are more difficult platforms out there, we'll do OS
 * specific #ifdefs. But for now we'll just include the headers
 * which normally contains the alloca() prototype.
 * When we're in kernel territory it starts getting a bit more
 * interesting of course...
 */
#if defined(IN_RING0) && defined(__LINUX__)
/* ASSUMES GNU C */
# define alloca(cb) __builtin_alloca(cb)
#else
# include <stdlib.h>
# if !defined(__DARWIN__) && !defined(__FREEBSD__)
#  include <malloc.h>
# endif
# ifdef __SOLARIS__
#  include <alloca.h>
# endif
#endif

#endif

