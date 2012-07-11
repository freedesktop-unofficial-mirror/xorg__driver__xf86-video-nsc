/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/nsc/gfx/gfx_init.c,v 1.1 2002/12/10 15:12:25 alanh Exp $ */
/*
 * $Workfile: gfx_init.c $
 *
 * This file contains routines typically used in driver initialization.
 *
 * Routines:
 * 
 *       gfx_pci_config_read
 *       gfx_cpu_config_read
 *       gfx_detect_cpu
 *       gfx_detect_video
 *       gfx_get_cpu_register_base
 *       gfx_get_frame_buffer_base
 *       gfx_get_frame_buffer_size
 *       gfx_get_vid_register_base
 *       gfx_get_vip_register_base
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

/* CONSTANTS USED BY THE INITIALIZATION CODE */

#define PCI_CONFIG_ADDR			         0x0CF8
#define PCI_CONFIG_DATA			         0x0CFC
#define PCI_VENDOR_DEVICE_GXM	         0x00011078
#define PCI_VENDOR_DEVICE_REDCLOUD       0x0028100B
#define REDCLOUD_VIDEO_PCI_VENDOR_DEVICE 0x0030100B

#define GXM_CONFIG_GCR			0xB8
#define GXM_CONFIG_CCR3			0xC3
#define GXM_CONFIG_DIR0			0xFE
#define GXM_CONFIG_DIR1			0xFF

/* STATIC VARIABLES FOR THIS FILE */

unsigned long gfx_cpu_version = 0;
unsigned long gfx_cpu_frequency = 0;
unsigned long gfx_vid_version = 0;
unsigned long gfx_gx1_scratch_base = 0;
unsigned long gfx_gx2_scratch_base = 0x7FC000;
ChipType gfx_chip_revision = CHIP_NOT_DETECTED;

/* INCLUDE SUPPORT FOR FIRST GENERATION, IF SPECIFIED. */

ChipType gfx_detect_chip(void);

#if GFX_INIT_GU1
#include "init_gu1.c"
#endif

/* INCLUDE SUPPORT FOR SECOND GENERATION, IF SPECIFIED. */

#if GFX_INIT_GU2
#include "init_gu2.c"
#endif

/* THE FOLLOWING ROUTINES ARE NEVER DYNAMIC              */
/* They are used to set the variables for future dynamic */
/* function calls.                                       */

/*-----------------------------------------------------------------------------
 * gfx_detect_chip
 *
 * This routine returns the name and revision of the chip. This function is only
 * relevant to the SC1200.
 *-----------------------------------------------------------------------------
 */
ChipType
gfx_detect_chip(void)
{
    unsigned char pid = INB(SC1200_CB_BASE_ADDR + SC1200_CB_PID);
    unsigned char rev = INB(SC1200_CB_BASE_ADDR + SC1200_CB_REV);

    gfx_chip_revision = CHIP_NOT_DETECTED;

    if (pid == 0x4) {
        switch (rev) {
        case 0:
            gfx_chip_revision = SC1200_REV_A;
            break;
        case 1:
            gfx_chip_revision = SC1200_REV_B1_B2;
            break;
        case 2:
            gfx_chip_revision = SC1200_REV_B3;
            break;
        case 3:
            gfx_chip_revision = SC1200_REV_C1;
            break;
        case 4:
            gfx_chip_revision = SC1200_REV_D1;
            break;
        case 5:
            gfx_chip_revision = SC1200_REV_D1_1;
            break;
        case 6:
            gfx_chip_revision = SC1200_REV_D2_MVD;
            break;
        }
        if (rev > 0x6)
            gfx_chip_revision = SC1200_FUTURE_REV;
    }
    else if (pid == 0x5) {
        if (rev == 0x6)
            gfx_chip_revision = SC1200_REV_D2_MVE;
        else if (rev > 0x6)
            gfx_chip_revision = SC1200_FUTURE_REV;
    }
    return (gfx_chip_revision);
}

/*-----------------------------------------------------------------------------
 * gfx_detect_cpu
 * 
 * This routine returns the type and revison of the CPU.  If a Geode 
 * processor is not present, the routine returns zero.
 *
 * The return value is as follows:
 *     bits[24:16] = minor version
 *     bits[15:8] = major version 
 *     bits[7:0] = type (1 = GXm, 2 = SC1200, 3 = Redcloud)
 *
 * A return value of 0x00020501, for example, indicates GXm version 5.2.
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_detect_cpu(void)
{

    unsigned long value = 0;
    unsigned long version = 0;

    /* initialize core freq. to 0 */
    gfx_cpu_frequency = 0;

#if	GFX_INIT_GU1

    value = gfx_pci_config_read(0x80000000);

    if (value == PCI_VENDOR_DEVICE_GXM) {
        unsigned char dir0 = gfx_gxm_config_read(GXM_CONFIG_DIR0) & 0xF0;
        unsigned char dir1 = gfx_gxm_config_read(GXM_CONFIG_DIR1);

        if (dir0 == 0x40) {
            /* CHECK FOR GXLV (and GXm) (DIR1 = 0x30 THROUGH 0x82) */

            if ((dir1 >= 0x30) && (dir1 <= 0x82)) {
                /* Major version is one less than what appears in DIR1 */
                if ((dir1 & 0xF0) < 0x70) {

                    version = GFX_CPU_GXLV | (((((unsigned long) dir1 >> 4) - 1) << 8)) |       /* major - 1 */
                        ((((unsigned long) dir1 & 0x0F)) << 16);        /* minor */
                }
                else {
                    version = GFX_CPU_GXLV | ((((unsigned long) dir1 >> 4)) << 8) |     /* major */
                        ((((unsigned long) dir1 & 0x0F)) << 16);        /* minor */

                }
                /* Currently always CS5530 for video overlay. */

#				if GFX_VIDEO_DYNAMIC
                gfx_video_type = GFX_VIDEO_TYPE_CS5530;
#				endif

                /* Currently always CS5530 GPIOs for I2C access. */

#				if GFX_I2C_DYNAMIC
                gfx_i2c_type = GFX_I2C_TYPE_GPIO;
#				endif

#				if GFX_TV_DYNAMIC
                gfx_tv_type = GFX_TV_TYPE_FS451;
#				endif
            }
        }
        else if (dir0 == 0xB0) {
            /* CHECK FOR SC1200 */

            if ((dir1 == 0x70) || (dir1 == 0x81)) {
                version = GFX_CPU_SC1200 | ((((unsigned long) dir1 >> 4)) << 8) |       /* major */
                    ((((unsigned long) dir1 & 0x0F)) << 16);    /* minor */

                /* Detect SC1200 revision */

                gfx_detect_chip();

                /* SC1200 for video overlay and VIP. */

#				if GFX_VIDEO_DYNAMIC
                gfx_video_type = GFX_VIDEO_TYPE_SC1200;
#				endif

#				if GFX_VIP_DYNAMIC
                gfx_vip_type = GFX_VIP_TYPE_SC1200;
#				endif

                /* Currently always SAA7114 decoder. */

#				if GFX_DECODER_DYNAMIC
                gfx_decoder_type = GFX_DECODER_TYPE_SAA7114;
#				endif

                /* SC1200 for TV encoder */

#				if GFX_TV_DYNAMIC
                gfx_tv_type = GFX_TV_TYPE_SC1200;
#				endif

                /* Currently always ACCESS.bus for I2C access. */

#				if GFX_I2C_DYNAMIC
                gfx_i2c_type = GFX_I2C_TYPE_ACCESS;
#				endif
            }
        }

        if (version) {
            /* ALWAYS FIRST GENERATION GRAPHICS UNIT */

#			if GFX_DISPLAY_DYNAMIC
            gfx_display_type = GFX_DISPLAY_TYPE_GU1;
#			endif
#			if GFX_2DACCEL_DYNAMIC
            gfx_2daccel_type = GFX_2DACCEL_TYPE_GU1;
#			endif
#			if GFX_INIT_DYNAMIC
            gfx_init_type = GFX_INIT_TYPE_GU1;
#			endif

            /* READ THE CORE FREQUENCY  */

            gfx_cpu_frequency = gfx_get_core_freq();
        }
    }
#endif

#if GFX_INIT_GU2

    value = gfx_pci_config_read(0x80000800);

    if (value == PCI_VENDOR_DEVICE_REDCLOUD) {
        Q_WORD msr_value;
        int valid, i;

        /* CHECK FOR SOFT VG */
        /* If SoftVG is not present, the base addresses for all devices */
        /* will not be allocated.  Essentially, it is as if no Redcloud */
        /* video hardware is present.                                   */

        value = gfx_pci_config_read(0x80000900);

        if (value == REDCLOUD_VIDEO_PCI_VENDOR_DEVICE) {
            valid = 1;

            /* BAR0 - BAR3 HOLD THE PERIPHERAL BASE ADDRESSES */

            for (i = 0; i < 4; i++) {
                value = gfx_pci_config_read(0x80000910 + (i << 2));
                if (value == 0x00000000 || value == 0xFFFFFFFF) {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                /* REDCLOUD INTEGRATED VIDEO             */

#				if GFX_VIDEO_DYNAMIC
                gfx_video_type = GFX_VIDEO_TYPE_REDCLOUD;
#				endif

                /* CURRENTLY, ALWAYS GPIO FOR I2C ACCESS */

#				if GFX_I2C_DYNAMIC
                gfx_i2c_type = GFX_I2C_TYPE_GPIO;
#				endif

                /* SECOND-GENERATION DISPLAY CONTROLLER  */

#				if GFX_DISPLAY_DYNAMIC
                gfx_display_type = GFX_DISPLAY_TYPE_GU2;
#				endif

                /* SECOND-GENERATION GRAPHICS UNIT       */

#				if GFX_2DACCEL_DYNAMIC
                gfx_2daccel_type = GFX_2DACCEL_TYPE_GU2;
#				endif

                /* SECOND-GENERATION INITIALIZATION      */

#				if GFX_INIT_DYNAMIC
                gfx_init_type = GFX_INIT_TYPE_GU2;
#				endif

                /* MBUS MSR ACCESSES                     */

#				if GFX_MSR_DYNAMIC
                gfx_msr_type = GFX_MSR_TYPE_REDCLOUD;
#				endif

                /* CS5530 GPIO I2C */

#				if GFX_I2C_DYNAMIC
                gfx_i2c_type = GFX_I2C_TYPE_GPIO;
#				endif

                /* READ VERSION */

                gfx_msr_init();

                gfx_msr_read(RC_ID_MCP, MCP_RC_REVID, &msr_value);

                /* SUBTRACT 1 FROM REV ID                                */
                /* REDCLOUD 1.X rev id is 1 less than the reported value */

                if ((msr_value.low & 0xF0) == 0x10)
                    msr_value.low--;

                version = GFX_CPU_REDCLOUD | ((msr_value.low & 0xF0) << 4) |    /* MAJOR */
                    ((msr_value.low & 0x0F) << 16);     /* MINOR */

                /* READ THE CORE FREQUENCY  */

                gfx_cpu_frequency = gfx_get_core_freq();

                /* SET THE GP SCRATCH AREA */
                /* Color bitmap BLTs use the last 16K of frame buffer space */

                gfx_gx2_scratch_base = gfx_get_frame_buffer_size() - 0x4000;
            }
        }
    }
#endif

    if (!version) {
        /* ALWAYS SECOND GENERATION IF SIMULATING */
        /* For now, that is.  This could change.  */

#		if GFX_DISPLAY_DYNAMIC
        gfx_display_type = GFX_DISPLAY_TYPE_GU2;
#		endif
#		if GFX_2DACCEL_DYNAMIC
        gfx_2daccel_type = GFX_2DACCEL_TYPE_GU2;
#		endif
#		if GFX_INIT_DYNAMIC
        gfx_init_type = GFX_INIT_TYPE_GU2;
#		endif
#		if GFX_MSR_DYNAMIC
        gfx_msr_type = GFX_MSR_TYPE_REDCLOUD;
#		endif
#		if GFX_VIDEO_DYNAMIC
        gfx_video_type = GFX_VIDEO_TYPE_REDCLOUD;
#		endif
#		if GFX_I2C_DYNAMIC
        gfx_i2c_type = GFX_I2C_TYPE_GPIO;
#		endif
    }
    gfx_cpu_version = version;

    return (version);
}

/*-----------------------------------------------------------------------------
 * gfx_detect_video
 * 
 * This routine returns the type of the video hardware.
 *
 * The return value is as follows:
 *     bits[7:0] = type (1 = CS5530, 2 = SC1200, 3 = Redcloud)
 *
 * Currently this routine does not actually detect any hardware, and bases
 * the video hardware entirely on the detected CPU.
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_detect_video(void)
{
    unsigned long version = 0;

    if ((gfx_cpu_version & 0xFF) == GFX_CPU_GXLV)
        version = GFX_VID_CS5530;
    else if ((gfx_cpu_version & 0xFF) == GFX_CPU_SC1200)
        version = GFX_VID_SC1200;
    else if ((gfx_cpu_version & 0xFF) == GFX_CPU_REDCLOUD)
        version = GFX_VID_REDCLOUD;
    gfx_vid_version = version;
    return (version);
}

/*-----------------------------------------------------------------------------
 * gfx_pci_config_read
 * 
 * This routine reads a 32-bit value from the specified location in PCI
 * configuration space.
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_pci_config_read(unsigned long address)
{
    unsigned long value = 0xFFFFFFFF;

    OUTD(PCI_CONFIG_ADDR, address);
    value = IND(PCI_CONFIG_DATA);
    return (value);
}

/*-----------------------------------------------------------------------------
 * gfx_pci_config_write
 * 
 * This routine writes a 32-bit value to the specified location in PCI
 * configuration space.
 *-----------------------------------------------------------------------------
 */
void
gfx_pci_config_write(unsigned long address, unsigned long data)
{
    OUTD(PCI_CONFIG_ADDR, address);
    OUTD(PCI_CONFIG_DATA, data);
    return;
}

/* WRAPPERS IF DYNAMIC SELECTION */
/* Extra layer to call either first or second generation routines. */

#if GFX_INIT_DYNAMIC

/*-----------------------------------------------------------------------------
 * gfx_get_core_freq
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_core_freq(void)
{
    unsigned long freq = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        freq = gu1_get_core_freq();
#	endif
#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        freq = gu2_get_core_freq();
#	endif
    return freq;
}

/*-----------------------------------------------------------------------------
 * gfx_get_cpu_register_base
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_cpu_register_base(void)
{
    unsigned long base = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        base = gu1_get_cpu_register_base();
#	endif
#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        base = gu2_get_cpu_register_base();
#	endif

    return (base);
}

/*-----------------------------------------------------------------------------
 * gfx_get_graphics_register_base
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_graphics_register_base(void)
{
    unsigned long base = 0;

#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        base = gu2_get_graphics_register_base();
#	endif

    return (base);
}

/*-----------------------------------------------------------------------------
 * gfx_get_frame_buffer_base
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_frame_buffer_base(void)
{
    unsigned long base = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        base = gu1_get_frame_buffer_base();
#	endif
#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        base = gu2_get_frame_buffer_base();
#	endif

    return (base);
}

/*-----------------------------------------------------------------------------
 * gfx_get_frame_buffer_size
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_frame_buffer_size(void)
{
    unsigned long size = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        size = gu1_get_frame_buffer_size();
#	endif
#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        size = gu2_get_frame_buffer_size();
#	endif

    return size;
}

/*-----------------------------------------------------------------------------
 * gfx_get_vid_register_base
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_vid_register_base(void)
{
    unsigned long base = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        base = gu1_get_vid_register_base();
#	endif
#	if GFX_INIT_GU2
    if (gfx_init_type & GFX_INIT_TYPE_GU2)
        base = gu2_get_vid_register_base();
#	endif

    return (base);
}

/*-----------------------------------------------------------------------------
 * gfx_get_vip_register_base
 *-----------------------------------------------------------------------------
 */
unsigned long
gfx_get_vip_register_base(void)
{
    unsigned long base = 0;

#	if GFX_INIT_GU1
    if (gfx_init_type & GFX_INIT_TYPE_GU1)
        base = gu1_get_vip_register_base();
#	endif

    return (base);
}

#endif

/* END OF FILE */
