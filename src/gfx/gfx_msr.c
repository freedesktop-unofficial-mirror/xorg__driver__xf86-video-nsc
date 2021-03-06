/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/nsc/gfx/gfx_msr.c,v 1.1 2002/12/10 15:12:25 alanh Exp $ */
/*
 * $Workfile: gfx_msr.c $
 *
 * This file contains routines to read machine-specific registers (MSRs)
 *
 * Routines:
 * 
 *       gfx_msr_init
 *       gfx_id_msr_device
 *       gfx_get_msr_dev_address
 *       gfx_get_glink_id_at_address
 *       gfx_msr_read
 *       gfx_msr_write
 *
 * NSC_LIC_ALTERNATIVE_PREAMBLE
 *
 * Revision 1.0
 *
 * National Semiconductor Alternative GPL-BSD License
 *
 * National Semiconductor Corporation licenses this software 
 * ("Software"):
 *
 *      Durango
 *
 * under one of the two following licenses, depending on how the 
 * Software is received by the Licensee.
 * 
 * If this Software is received as part of the Linux Framebuffer or
 * other GPL licensed software, then the GPL license designated 
 * NSC_LIC_GPL applies to this Software; in all other circumstances 
 * then the BSD-style license designated NSC_LIC_BSD shall apply.
 *
 * END_NSC_LIC_ALTERNATIVE_PREAMBLE */

/* NSC_LIC_BSD
 *
 * National Semiconductor Corporation Open Source License for Durango
 *
 * (BSD License with Export Notice)
 *
 * Copyright (c) 1999-2001
 * National Semiconductor Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 *   * Redistributions of source code must retain the above copyright 
 *     notice, this list of conditions and the following disclaimer. 
 *
 *   * Redistributions in binary form must reproduce the above 
 *     copyright notice, this list of conditions and the following 
 *     disclaimer in the documentation and/or other materials provided 
 *     with the distribution. 
 *
 *   * Neither the name of the National Semiconductor Corporation nor 
 *     the names of its contributors may be used to endorse or promote 
 *     products derived from this software without specific prior 
 *     written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * NATIONAL SEMICONDUCTOR CORPORATION OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE,
 * INTELLECTUAL PROPERTY INFRINGEMENT, OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * EXPORT LAWS: THIS LICENSE ADDS NO RESTRICTIONS TO THE EXPORT LAWS OF 
 * YOUR JURISDICTION. It is licensee's responsibility to comply with 
 * any export regulations applicable in licensee's jurisdiction. Under 
 * CURRENT (2001) U.S. export regulations this software 
 * is eligible for export from the U.S. and can be downloaded by or 
 * otherwise exported or reexported worldwide EXCEPT to U.S. embargoed 
 * destinations which include Cuba, Iraq, Libya, North Korea, Iran, 
 * Syria, Sudan, Afghanistan and any other country to which the U.S. 
 * has embargoed goods and services. 
 *
 * END_NSC_LIC_BSD */

/* NSC_LIC_GPL
 *
 * National Semiconductor Corporation Gnu General Public License for Durango
 *
 * (GPL License with Export Notice)
 *
 * Copyright (c) 1999-2001
 * National Semiconductor Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted under the terms of the GNU General 
 * Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version  
 *
 * In addition to the terms of the GNU General Public License, neither 
 * the name of the National Semiconductor Corporation nor the names of 
 * its contributors may be used to endorse or promote products derived 
 * from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * NATIONAL SEMICONDUCTOR CORPORATION OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE, 
 * INTELLECTUAL PROPERTY INFRINGEMENT, OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE. See the GNU General Public License for more details. 
 *
 * EXPORT LAWS: THIS LICENSE ADDS NO RESTRICTIONS TO THE EXPORT LAWS OF 
 * YOUR JURISDICTION. It is licensee's responsibility to comply with 
 * any export regulations applicable in licensee's jurisdiction. Under 
 * CURRENT (2001) U.S. export regulations this software 
 * is eligible for export from the U.S. and can be downloaded by or 
 * otherwise exported or reexported worldwide EXCEPT to U.S. embargoed 
 * destinations which include Cuba, Iraq, Libya, North Korea, Iran, 
 * Syria, Sudan, Afghanistan and any other country to which the U.S. 
 * has embargoed goods and services. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with this file; if not, write to the Free Software Foundation, 
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 *
 * END_NSC_LIC_GPL */

/* INCLUDE SUPPORT FOR REDCLOUD, IF SPECIFIED */

#if GFX_MSR_REDCLOUD
#include "msr_rdcl.c"
#endif

/* EXTRA WRAPPERS FOR DYNAMIC SELECTION */

#if GFX_MSR_DYNAMIC

/*-----------------------------------------------------------------------------
 * gfx_msr_init
 *-----------------------------------------------------------------------------
 */
int
gfx_msr_init()
{
    int ret_value = 0;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_msr_init();
#	endif

    return ret_value;
}

/*-----------------------------------------------------------------------------
 * gfx_id_msr_device
 *-----------------------------------------------------------------------------
 */
DEV_STATUS
gfx_id_msr_device(MSR * pDev, unsigned long address)
{
    DEV_STATUS ret_value = NOT_KNOWN;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_id_msr_device(pDev, address);
#	endif

    return ret_value;
}

/*-----------------------------------------------------------------------------
 * gfx_get_msr_dev_address
 *-----------------------------------------------------------------------------
 */
DEV_STATUS
gfx_get_msr_dev_address(unsigned int device, unsigned long *address)
{
    DEV_STATUS ret_value = NOT_KNOWN;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_get_msr_dev_address(device, address);
#	endif

    return ret_value;
}

/*-----------------------------------------------------------------------------
 * gfx_get_glink_id_at_address
 *-----------------------------------------------------------------------------
 */
DEV_STATUS
gfx_get_glink_id_at_address(unsigned int *device, unsigned long address)
{
    DEV_STATUS ret_value = NOT_KNOWN;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_get_glink_id_at_address(device, address);
#	endif

    return ret_value;
}

/*-----------------------------------------------------------------------------
 * gfx_msr_read
 *-----------------------------------------------------------------------------
 */
DEV_STATUS
gfx_msr_read(unsigned int device, unsigned int msrRegister, Q_WORD * msrValue)
{
    DEV_STATUS ret_value = NOT_KNOWN;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_msr_read(device, msrRegister, msrValue);
#	endif

    return ret_value;
}

/*-----------------------------------------------------------------------------
 * gfx_msr_write
 *-----------------------------------------------------------------------------
 */
DEV_STATUS
gfx_msr_write(unsigned int device, unsigned int msrRegister, Q_WORD * msrValue)
{
    DEV_STATUS ret_value = NOT_KNOWN;

#	if GFX_MSR_REDCLOUD
    if (gfx_msr_type & GFX_MSR_TYPE_REDCLOUD)
        ret_value = redcloud_msr_write(device, msrRegister, msrValue);
#	endif

    return ret_value;
}

#endif
