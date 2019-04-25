/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#if defined(BUILD_LK)
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#else
//#include <mach/mt_gpio.h>
//#include <mach/mt_pm_ldo.h>
#endif

#if !defined(BUILD_LK)
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#if defined(BUILD_LK)
    #define LCM_DEBUG  printf
    #define LCM_FUNC_TRACE() printf("huyl [uboot] %s\n",__func__)
#else
    #define LCM_DEBUG  printk
    #define LCM_FUNC_TRACE() printk("huyl [kernel] %s\n",__func__)
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH                                          (720)
#define FRAME_HEIGHT                                         (1280)
#define LCM_ID												(0x9361)

#define REGFLAG_DELAY                                       0xFE
#define REGFLAG_END_OF_TABLE                                0xFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE                                    0

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

//static kal_bool IsFirstBoot = KAL_TRUE;

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)            lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                           lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)                   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   

#define   LCM_DSI_CMD_MODE                          0

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

#if 0
static struct LCM_setting_table lcm_initialization_setting[] = {
#if 0	
{0xBF,3,{0x91,0x61,0xF2}},
{0xB3,2,{0x00,0xA1}},
{0xB4,2,{0x00,0xA1}},
{0xB8,6,{0x00,0x6F,0x01,0x00,0x6F,0x01}},
{0xBA,3,{0x34,0x23,0x00}},
{0xC3,1,{0x04}},
{0xC4,2,{0x30,0x6A}},
{0xC7,9,{0x00,0x01,0x32,0x05,0x65,0x2A,0x12,0xA5,0xA5}},
{0xC8,38,{0x7F,0x68,0x57,0x4A,0x44,0x33,0x34,0x1A,0x2E,0x29,0x25,0x3E,0x2F,0x3B,0x2E,0x2B,0x1E,0x0F,0x00,
	  0x7F,0x68,0x57,0x4A,0x44,0x33,0x34,0x1A,0x2E,0x29,0x25,0x3E,0x2F,0x3B,0x2E,0x2B,0x1E,0x0F,0x00}},
 
{0xD4,16,{0x1E,0x1F,0x1F,0x1F,0x06,0x04,0x0A,0x08,0x00,0x02,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},
{0xD5,16,{0x1E,0x1F,0x1F,0x1F,0x07,0x05,0x0B,0x09,0x01,0x03,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},
{0xD6,16,{0x1F,0x1E,0x1F,0x1F,0x07,0x09,0x0B,0x05,0x03,0x01,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},
{0xD7,16,{0x1F,0x1E,0x1F,0x1F,0x06,0x08,0x0A,0x04,0x02,0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}},
{0xD8,20,{0x20,0x00,0x00,0x30,0x08,0x20,0x01,0x02,0x00,0x01,0x02,0x06,0x7B,0x00,0x00,0x72,0x0A,0x06,0x7B,0x08}},
{0xD9,19,{0x00,0x0A,0x0A,0x89,0x00,0x00,0x06,0x7B,0x00,0x00,0x00,0x3B,0x33,0x1F,0x00,0x00,0x00,0x03,0x7B}},
{0xBE,1,{0x01}},
{0xCC,10,{0x34,0x20,0x38,0x60,0x11,0x91,0x00,0x40,0x00,0x00}},
{0xBE,1,{0x00}},
{0xC1, 0, {0x08}},
     	{0x35,	1,	{0x00}},
	{0x11,	1,	{0x00}},
	{REGFLAG_DELAY,150,{}},
	{0x29,	1,	{0x00}},
	{REGFLAG_DELAY,50,{}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
	
#else
{0xBF, 3, {0x91,0x61,0xF2}},
{REGFLAG_DELAY,1,{}},

{0xB3, 2, {0x00,0x96}},
{REGFLAG_DELAY,1,{}},

{0xB4, 2, {0x00,0x96}},
{REGFLAG_DELAY,1,{}},

{0xB8, 6, {0x00,0xBF,0x01,0x00,0xBF,0x01}},



{REGFLAG_DELAY,1,{}},

{0xBA, 3, {0x3E,0x23,0x00}},
{REGFLAG_DELAY,1,{}},

{0xC3, 1, {0x04}},
{REGFLAG_DELAY,1,{}},

{0xC4, 2, {0x30,0x6A}},
{REGFLAG_DELAY,1,{}},

{0xC7, 9, {0x00,0x01,0x31,0x0A,0x6A,0x2A,0x13,0xA5,0xA5}},
{REGFLAG_DELAY,1,{}},

{0xC8, 38, {0x7F,0x3B,0x33,0x28,0x1A,0x0B,0x12,0x00,0x1E,0x21,0x26,0x4B,0x41,0x53,0x4E,0x57,0x53,0x4E,0x48,0x7F,0x3B,0x33,0x28,0x1A,0x0B,0x12,0x00,0x1E,0x21,0x26,0x4B,0x41,0x53,0x4E,0x57,0x53,0x4E,0x48}},
{REGFLAG_DELAY,1,{}},

{0xD4, 16, {0x1F,0x1F,0x1F,0x03,0x01,0x05,0x07,0x09,0x0B,0x11,0x13,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY,1,{}},

{0xD5, 16, {0x1F,0x1F,0x1F,0x02,0x00,0x04,0x06,0x08,0x0A,0x10,0x12,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY,1,{}},

{0xD6, 16, {0x1F,0x1F,0x1F,0x10,0x12,0x04,0x06,0x08,0x0A,0x02,0x00,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY,1,{}},

{0xD7, 16, {0x1F,0x1F,0x1F,0x11,0x13,0x0B,0x09,0x07,0x05,0x03,0x01,0x1F,0x1F,0x1F,0x1F,0x1F}},
{REGFLAG_DELAY,1,{}},

{0xD8, 20, {0x20,0x00,0x00,0x30,0x03,0x30,0x01,0x02,0x30,0x01,0x02,0x06,0x70,0x73,0x5D,0x73,0x06,0x5F,0x5F,0x08}},
{REGFLAG_DELAY,1,{}},

{0xD9, 19, {0x00,0x0A,0x0A,0x88,0x00,0x00,0x06,0x7B,0x00,0x80,0x00,0x3B,0x33,0x1F,0x00,0x00,0x00,0x06,0x70}},
{REGFLAG_DELAY,1,{}},

{0xBE, 1, {0x01}},

{0xCC, 10, {0x34,0x20,0x38,0x60,0x11,0x91,0x00,0x40,0x00,0x00}},

{0xBE, 1, {0x00}},

{0x11, 0, {0x00}},
{REGFLAG_DELAY,120,{}},

{0x29, 0, {0x00}},
{REGFLAG_DELAY,10,{}},

{REGFLAG_END_OF_TABLE, 0x00, {}}
#endif
};

static struct LCM_setting_table lcm_compare_id_setting[] = {
     // Display off sequence
    {0xBF,  3,  {0x93, 0x61, 0xF4}},
    {REGFLAG_DELAY, 10, {}},

     //   {0x00,  1,  {0x80}},
    //{REGFLAG_DELAY, 10, {}},

      //  {0xFF,  2,  {0x80,0x09}},
    //{REGFLAG_DELAY, 10, {}},

      //  {0x00,  1,  {0x03}},
    //{REGFLAG_DELAY, 10, {}},

     //   {0xFF,  1,  {0x01}},
    //{REGFLAG_DELAY, 10, {}},

        // Sleep Mode On
        // {0xC3, 1, {0xFF}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
    {0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
    {0x29, 0, {0x00}},
    {REGFLAG_DELAY, 10, {}},
    
    //{REGFLAG_END_OF_TABLE, 0x00, {}}
};
#endif
static struct LCM_setting_table lcm_sleep_mode_in_setting[] = {
    // Display off sequence
    {0x28, 0, {0x00}},
    {REGFLAG_DELAY, 20, {}},

    // Sleep Mode On
	{0x10, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	//{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
            
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
                
            //case REGFLAG_END_OF_TABLE :
            //    break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
                MDELAY(2);
        }
    }
    
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		//params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		//params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

#if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
#else
		params->dsi.mode   = BURST_VDO_MODE;//SYNC_PULSE_VDO_MODE;
#endif
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

		params->dsi.vertical_sync_active				= 4;// 4;2
		params->dsi.vertical_backporch					= 11;// 8;2
		params->dsi.vertical_frontporch					= 8;// 8;2
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 20;//6
		params->dsi.horizontal_backporch				= 20;//37
		params->dsi.horizontal_frontporch				= 120;//37
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

#if 0    		
		params->dsi.pll_div1=0;//1;		// div1=0,1,2,3;div1_real=1,2,4,4
		params->dsi.pll_div2=2;//1;		// div2=0,1,2,3;div2_real=1,2,4,4
		//params->dsi.fbk_sel=1;		 // fbk_sel=0,1,2,3;fbk_sel_real=1,2,4,4
		params->dsi.fbk_div =26;//38;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	26=>351MHz Frame check=57
#else
		params->dsi.PLL_CLOCK=208;
#endif
		/* ESD or noise interference recovery For video mode LCM only. */ // Send TE packet to LCM in a period of n frames and check the response. 
		params->dsi.lcm_int_te_monitor = FALSE; 
		params->dsi.lcm_int_te_period = 1; // Unit : frames 
 
		// Need longer FP for more opportunity to do int. TE monitor applicably. 
		if(params->dsi.lcm_int_te_monitor) 
			params->dsi.vertical_frontporch *= 2; 
 
		// Monitor external TE (or named VSYNC) from LCM once per 2 sec. (LCM VSYNC must be wired to baseband TE pin.) 
		//params->dsi.lcm_ext_te_monitor = FALSE; 
		// Non-continuous clock 
	//	params->dsi.noncont_clock = TRUE; 
		//params->dsi.noncont_clock_period = 2; // Unit : frames
		
		
		    params->dsi.esd_check_enable = 1; //0; //chenyichong 0->1 temp.
		    params->dsi.customization_esd_check_enable      = 1;
		    params->dsi.clk_lp_per_line_enable              =1;
		    params->dsi.lcm_esd_check_table[0].cmd          = 0x0a;
		    params->dsi.lcm_esd_check_table[0].count        = 1;
		    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;//0x1C;
}

static void init_lcm_registers(void)
{

unsigned int data_array[16];

data_array[0]=0x00043902;
data_array[1]=0xF46193BF;
dsi_set_cmdq(data_array,2,1);
MDELAY(1);


data_array[0]=0x00063902;
data_array[1]=0x230100B0;
data_array[2]=0x00006645;
dsi_set_cmdq(data_array,3,1);
MDELAY(1);


data_array[0]=0x000C3902;
data_array[1]=0x690103C7;//ÉýÑ¹Ð¡ic 7721--690100C7, 7723--690103C7
data_array[2]=0x16260A0A;
data_array[3]=0x22324201;
dsi_set_cmdq(data_array,4,1);
MDELAY(1);


data_array[0]=0x00053902;
data_array[1]=0x2F2F31BA;
data_array[2]=0x0000007E;
dsi_set_cmdq(data_array,3,1);
MDELAY(1);




data_array[0]=0x01BE1500;
dsi_set_cmdq(data_array,1,1);
MDELAY(1);


data_array[0]=0x38C21500;
dsi_set_cmdq(data_array,1,1);
MDELAY(1);


data_array[0]=0x03C41500;
dsi_set_cmdq(data_array,1,1);
MDELAY(1);


data_array[0]=0x00093902;
data_array[1]=0x380034CC;
data_array[2]=0x02910007;
data_array[3]=0x000000D0;
dsi_set_cmdq(data_array,4,1);
MDELAY(1);


data_array[0]=0x00BE1500;
dsi_set_cmdq(data_array,1,1);
MDELAY(1);


//data_array[0]=0x08C11500;
//dsi_set_cmdq(data_array,1,1);
//MDELAY(1);


data_array[0]=0x00093902;
data_array[1]=0x0A0400C3;
data_array[2]=0xAD0C9115;
data_array[3]=0x000000AD;
dsi_set_cmdq(data_array,4,1);
MDELAY(1);


data_array[0]=0x000D3902;
data_array[1]=0xD0A004C4;
data_array[2]=0x00001104;
data_array[3]=0x000F0000;
data_array[4]=0x00000000;
dsi_set_cmdq(data_array,5,1);
MDELAY(1);


data_array[0]=0x00273902;
data_array[1]=0x425570C8;
data_array[2]=0x221C2D32;
data_array[3]=0x2D2B2A0D;
data_array[4]=0x2A3A374A;
data_array[5]=0x020E1A26;
data_array[6]=0x32425570;
data_array[7]=0x0D221C2D;
data_array[8]=0x4A2D2B2A;
data_array[9]=0x262A3A37;
data_array[10]=0x00020E1A;
dsi_set_cmdq(data_array,11,1);
MDELAY(1);


data_array[0]=0x00173902;
data_array[1]=0x0B1113D5;
data_array[2]=0x1F050709;
data_array[3]=0x1F1F1F1F;
data_array[4]=0x1F1F1F1F;
data_array[5]=0x011F1F1F;
data_array[6]=0x001F1F03;
dsi_set_cmdq(data_array,7,1);
MDELAY(1);


data_array[0]=0x00173902;
data_array[1]=0x0A1012D6;
data_array[2]=0x1F040608;
data_array[3]=0x1F1F1F1F;
data_array[4]=0x1F1F1F1F;
data_array[5]=0x001F1F1F;
data_array[6]=0x001F1F02;
dsi_set_cmdq(data_array,7,1);
MDELAY(1);


data_array[0]=0x00173902;
data_array[1]=0x080200D7;
data_array[2]=0x1F06040A;
data_array[3]=0x1F1F1F1F;
data_array[4]=0x1F1F1F1F;
data_array[5]=0x121F1F1F;
data_array[6]=0x001F1F10;
dsi_set_cmdq(data_array,7,1);
MDELAY(1);

data_array[0]=0x00173902;
data_array[1]=0x090301D8;
data_array[2]=0x1F07050B;
data_array[3]=0x1F1F1F1F;
data_array[4]=0x1F1F1F1F;
data_array[5]=0x131F1F1F;
data_array[6]=0x001F1F11;
dsi_set_cmdq(data_array,7,1);
MDELAY(1);

data_array[0]=0x002C3902;
data_array[1]=0x000000D9;
data_array[2]=0x12300930;
data_array[3]=0x0A738C03;
data_array[4]=0x00088C06;
data_array[5]=0x00860600;
data_array[6]=0x12300D75;
data_array[7]=0x00078C03;
data_array[8]=0x0500195D;
data_array[9]=0x86030005;
data_array[10]=0x00000000;
data_array[11]=0x00000000;
dsi_set_cmdq(data_array,12,1);
MDELAY(1);

data_array[0]=0x00053902;
data_array[1]=0xD1F800BB;
data_array[2]=0x00000065;
dsi_set_cmdq(data_array,3,1);
MDELAY(1);

data_array[0]=0x00073902;
data_array[1]=0x01BF00B8;
data_array[2]=0x0001BF00;
dsi_set_cmdq(data_array,3,1);
MDELAY(1);

data_array[0]=0x00033902;
data_array[1]=0x006700D0;
dsi_set_cmdq(data_array,2,1);
MDELAY(1);

data_array[0]=0x00033902;
data_array[1]=0x006600D1;
dsi_set_cmdq(data_array,2,1);
MDELAY(1);

data_array[0]=0x46C01500;
dsi_set_cmdq(data_array,1,1);
MDELAY(1);


data_array[0]=0x00110500;
dsi_set_cmdq(data_array,1,1);
MDELAY(120);


data_array[0]=0x00290500;
dsi_set_cmdq(data_array,1,1);
MDELAY(20);

 };

//add by hyde for debug
static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(10); 
    SET_RESET_PIN(0);
    MDELAY(20); 
    
    SET_RESET_PIN(1);
    MDELAY(120); 

  //  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
  init_lcm_registers();
  
}



static void lcm_suspend(void)
{
/*
    SET_RESET_PIN(1);
    SET_RESET_PIN(0);
    MDELAY(20);//Must > 10ms
    SET_RESET_PIN(1);
    MDELAY(150);//Must > 120ms
*/    
	push_table(lcm_sleep_mode_in_setting, sizeof(lcm_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
	SET_RESET_PIN(0);
	MDELAY(20); 
/*
#if defined(BUILD_LK)
    upmu_set_rg_vgp5_en(0);
#else
    hwPowerDown(MT65XX_POWER_LDO_VGP5, "Lance_LCM");
#endif
*/
}


static void lcm_resume(void)
{
	lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}
         
#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
    unsigned int x0 = x;
    unsigned int y0 = y;
    unsigned int x1 = x0 + width - 1;
    unsigned int y1 = y0 + height - 1;

    unsigned char x0_MSB = ((x0>>8)&0xFF);
    unsigned char x0_LSB = (x0&0xFF);
    unsigned char x1_MSB = ((x1>>8)&0xFF);
    unsigned char x1_LSB = (x1&0xFF);
    unsigned char y0_MSB = ((y0>>8)&0xFF);
    unsigned char y0_LSB = (y0&0xFF);
    unsigned char y1_MSB = ((y1>>8)&0xFF);
    unsigned char y1_LSB = (y1&0xFF);

    unsigned int data_array[16];

    data_array[0]= 0x00053902;
    data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
    data_array[2]= (x1_LSB);
    dsi_set_cmdq(data_array, 3, 1);
    
    data_array[0]= 0x00053902;
    data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
    data_array[2]= (y1_LSB);
    dsi_set_cmdq(data_array, 3, 1);

    data_array[0]= 0x00290508; //HW bug, so need send one HS packet
    dsi_set_cmdq(data_array, 1, 1);
    
    data_array[0]= 0x002c3909;
    dsi_set_cmdq(data_array, 1, 0);

}
#endif


static unsigned int lcm_compare_id(void)
{
	unsigned int id1 = 0, id2 = 0, id3 = 0, id4 = 0,id;
	unsigned char buffer[4];
	unsigned int data_array[16];
	 
	SET_RESET_PIN(1);	//NOTE:should reset LCM firstly
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(100);

	data_array[0] = 0x00023700;
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(10);

	read_reg_v2(0x04, buffer, 2);
	id1= buffer[0]; //should be 
	id2= buffer[1]; //should be 0x98
	
	id=(id1 << 8) | id2;

#if defined(BUILD_LK)
	printf("ili9806e id=%x, %x %x %x %x\n", id, id1, id2, id3, id4);
#else
	printk("ili9806e id=%x, %x %x %x %x\n", id, id1, id2, id3, id4);
#endif
	return (LCM_ID == id) ? 1 : 0;
}
/*
static unsigned int lcm_esd_check(void)
{
  #ifndef BUILD_LK
    char  buffer[3];
    int   array[4];

    if(lcm_esd_test)
    {
        lcm_esd_test = FALSE;
        return TRUE;
    }

    array[0] = 0x00013700;
    dsi_set_cmdq(array, 1, 1);

    read_reg_v2(0x36, buffer, 1);
    if(buffer[0]==0x90)
    {
        return FALSE;
    }
    else
    {            
        return TRUE;
    }
 #endif

}
*/
#if 0
static unsigned int lcm_esd_recover(void)
{
    lcm_init();
    lcm_resume();

    return TRUE;
}
#endif

LCM_DRIVER hct_jd9361_dsi_vdo_hd_ivo_50_bx = 
{
.name = "hct_jd9361_dsi_vdo_hd_ivo_50_bx",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
    .compare_id     = lcm_compare_id,
    //.esd_check = lcm_esd_check,
    //.esd_recover = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
    };
