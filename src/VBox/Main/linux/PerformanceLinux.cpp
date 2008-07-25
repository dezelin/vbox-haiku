/* $Id$ */

/** @file
 *
 * VBox Linux-specific Performance Classes implementation.
 */

/*
 * Copyright (C) 2008 Sun Microsystems, Inc.
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 USA or visit http://www.sun.com if you need
 * additional information or have any questions.
 */

#include <unistd.h>
#include <stdio.h>
#include <iprt/alloc.h>
#include <iprt/err.h>
#include <iprt/string.h>
#include "Performance.h"

namespace pm {

class CollectorLinux : public CollectorHAL
{
public:
    virtual int getHostCpuMHz(unsigned long *mhz);
    virtual int getHostMemoryUsage(unsigned long *total, unsigned long *used, unsigned long *available);
    virtual int getProcessMemoryUsage(RTPROCESS process, unsigned long *used);

    virtual int getRawHostCpuLoad(uint64_t *user, uint64_t *kernel, uint64_t *idle);
    virtual int getRawProcessCpuLoad(RTPROCESS process, uint64_t *user, uint64_t *kernel, uint64_t *total);
private:
    int getRawProcessStats(RTPROCESS process, uint64_t *cpuUser, uint64_t *cpuKernel, unsigned long *memPagesUsed);
    long getPageSize() { return getpagesize(); };
};

// Linux Metric factory

MetricFactoryLinux::MetricFactoryLinux()
{
    mHAL = new CollectorLinux();
    Assert(mHAL);
}

// Collector HAL for Linux

int CollectorLinux::getRawHostCpuLoad(uint64_t *user, uint64_t *kernel, uint64_t *idle)
{
    int rc = VINF_SUCCESS;
    unsigned long u32user, u32nice, u32kernel, u32idle;
    FILE *f = fopen("/proc/stat", "r");

    if (f)
    {
        if (fscanf(f, "cpu %lu %lu %lu %lu", &u32user, &u32nice, &u32kernel, &u32idle) == 4)
        {
            *user   = (uint64_t)u32user + u32nice;
            *kernel = u32kernel;
            *idle   = u32idle;
        }
        else
            rc = VERR_FILE_IO_ERROR;
        fclose(f);
    }
    else
        rc = VERR_ACCESS_DENIED;

    return rc;
}

int CollectorLinux::getRawProcessCpuLoad(RTPROCESS process, uint64_t *user, uint64_t *kernel, uint64_t *total)
{
    int rc = VINF_SUCCESS;
    uint64_t uHostUser, uHostKernel, uHostIdle;

    rc = getRawHostCpuLoad(&uHostUser, &uHostKernel, &uHostIdle);
    if (RT_SUCCESS(rc))
    {
        unsigned long ulTmp;
        *total = (uint64_t)uHostUser + uHostKernel + uHostIdle;
        rc = getRawProcessStats(process, user, kernel, &ulTmp);
    }

    return rc;
}

int CollectorLinux::getHostCpuMHz(unsigned long *mhz)
{
    return E_NOTIMPL;
}

int CollectorLinux::getHostMemoryUsage(unsigned long *total, unsigned long *used, unsigned long *available)
{
    int rc = VINF_SUCCESS;
    unsigned long buffers, cached;
    FILE *f = fopen("/proc/meminfo", "r");

    if (f)
    {
        int processed = fscanf(f, "MemTotal: %lu kB", total);
        processed    += fscanf(f, "MemFree: %lu kB", available);
        processed    += fscanf(f, "Buffers: %lu kB", &buffers);
        processed    += fscanf(f, "Cached: %lu kB", &cached);
        if (processed == 4)
            *available += buffers + cached;
        else
            rc = VERR_FILE_IO_ERROR;
        fclose(f);
    }
    else
        rc = VERR_ACCESS_DENIED;

    return rc;
}

int CollectorLinux::getProcessMemoryUsage(RTPROCESS process, unsigned long *used)
{
    uint64_t u64Tmp;
    unsigned long nPagesUsed;
    int rc = getRawProcessStats(process, &u64Tmp, &u64Tmp, &nPagesUsed);
    if (RT_SUCCESS(rc))
    {
        Assert(getPageSize() >= 1024);
        *used = nPagesUsed * (getPageSize() / 1024);
    }
    return rc;
}

int CollectorLinux::getRawProcessStats(RTPROCESS process, uint64_t *cpuUser, uint64_t *cpuKernel, unsigned long *memPagesUsed)
{
    int rc = VINF_SUCCESS;
    char *pszName;
    pid_t pid2;
    char c;
    int iTmp;
    uint64_t u64Tmp;
    unsigned uTmp;
    unsigned long ulTmp, u32user, u32kernel;
    char buf[80]; /* @todo: this should be tied to max allowed proc name. */

    RTStrAPrintf(&pszName, "/proc/%d/stat", process);
    //printf("Opening %s...\n", pszName);
    FILE *f = fopen(pszName, "r");
    RTMemFree(pszName);

    if (f)
    {
        if (fscanf(f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu "
                      "%ld %ld %ld %ld %ld %ld %llu %lu %ld",
                   &pid2, buf, &c, &iTmp, &iTmp, &iTmp, &iTmp, &iTmp, &uTmp,
                   &ulTmp, &ulTmp, &ulTmp, &ulTmp, &u32user, &u32kernel,
                   &ulTmp, &ulTmp, &ulTmp, &ulTmp, &ulTmp, &ulTmp, &u64Tmp,
                   &ulTmp, memPagesUsed) == 24)
        {
            Assert((pid_t)process == pid2);
            *cpuUser   = u32user;
            *cpuKernel = u32kernel;
        }
        else
            rc = VERR_FILE_IO_ERROR;
        fclose(f);
    }
    else
        rc = VERR_ACCESS_DENIED;

    return rc;
}

}

