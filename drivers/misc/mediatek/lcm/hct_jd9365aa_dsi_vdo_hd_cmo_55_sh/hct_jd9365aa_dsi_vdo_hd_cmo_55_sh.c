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


#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"




// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)
#define LCM_ID 0x93

#define REGFLAG_DELAY               (0XFE)
#define REGFLAG_END_OF_TABLE        (0x100) // END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE									0



// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))



//static unsigned int lcm_esd_test = FALSE;      ///only for ESD test

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#define   LCM_DSI_CMD_MODE							0
 struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
//------------------JD9365 initial code------------------------//

//Page0
{0xE0,1,{0x00}},

//--- PASSWORD  ----//
{0xE1,1,{0x93}},
{0xE2,1,{0x65}},
{0xE3,1,{0xF8}},

//--- Sequence Ctrl ----//
//{0x70,1,{0x20}},
//{0x71,1,{0x13}},
//{0x72,1,{0x06}},
//{0x75,1,{0x03}},
//{0x80,1,{0x02}},

//--- Page1  ----//
{0xE0,1,{0x01}},

//Set VCOM
{0x00,1,{0x00}},
{0x01,1,{0x8C}},
{0x03,1,{0x00}},
{0x04,1,{0x91}},
//{0x0C,1,{0x74}},

//Set Gamma Power,1,{ VGMP,1,{VGMN,1,{VGSP,1,{VGSN
{0x17,1,{0x00}},
{0x18,1,{0xBF}},
{0x19,1,{0x01}},
{0x1A,1,{0x00}},
{0x1B,1,{0xBF}},
{0x1C,1,{0x01}},

//Set Gate Power
{0x1F,1,{0x7C}},
{0x20,1,{0x26}},
{0x21,1,{0x26}},
{0x22,1,{0x4E}},
//{0x24,1,{0xF8}},


//SET RGBCYC
{0x37,1,{0x09}},	//SS=1,1,{ BGR=1
{0x38,1,{0x04}},	//JDT=100 column inversion
{0x39,1,{0x08}},
{0x3A,1,{0x1F}},
{0x3B,1,{0x1F}},
{0x3C,1,{0x78}},
{0x3D,1,{0xFF}},
{0x3E,1,{0xFF}},
{0x3F,1,{0x00}},

//Set TCON
{0x40,1,{0x04}},	//RSO 04h=720,1,{ 05h=768,1,{ 06h=800
{0x41,1,{0xA0}},	//LN=640->1280 line

{0x43,1,{0x0F}},
{0x44,1,{0x0A}},
{0x45,1,{0x24}},
//--- power voltage  ----//
{0x55,1,{0x01}},
{0x56,1,{0x01}},
{0x57,1,{0x69}},//VGH/VGL charge pump
{0x58,1,{0x0A}},
{0x59,1,{0x4A}},
{0x5A,1,{0x2E}},//VGH
{0x5B,1,{0x1A}},//VGL
{0x5C,1,{0x19}},

//--- Gamma  ----//
{0x5D,1,{0x7C}},
{0x5E,1,{0x4C}},
{0x5F,1,{0x38}},
{0x60,1,{0x29}},
{0x61,1,{0x23}},
{0x62,1,{0x12}},
{0x63,1,{0x17}},
{0x64,1,{0x01}},
{0x65,1,{0x1A}},
{0x66,1,{0x1A}},
{0x67,1,{0x1A}},
{0x68,1,{0x3A}},
{0x69,1,{0x2D}},
{0x6A,1,{0x3D}},
{0x6B,1,{0x38}},
{0x6C,1,{0x43}},
{0x6D,1,{0x3A}},
{0x6E,1,{0x28}},
{0x6F,1,{0x08}},

{0x70,1,{0x7C}},
{0x71,1,{0x4C}},
{0x72,1,{0x38}},
{0x73,1,{0x29}},
{0x74,1,{0x23}},
{0x75,1,{0x12}},
{0x76,1,{0x17}},
{0x77,1,{0x01}},
{0x78,1,{0x1A}},
{0x79,1,{0x1A}},
{0x7A,1,{0x1A}},
{0x7B,1,{0x3A}},
{0x7C,1,{0x2D}},
{0x7D,1,{0x3D}},
{0x7E,1,{0x38}},
{0x7F,1,{0x43}},
{0x80,1,{0x3A}},
{0x81,1,{0x28}},
{0x82,1,{0x08}},


//Page2,1,{ for GIP
{0xE0,1,{0x02}},

//GIP_L Pin mapping
{0x00,1,{0x57}},
{0x01,1,{0x77}},
{0x02,1,{0x44}},
{0x03,1,{0x46}},
{0x04,1,{0x48}},
{0x05,1,{0x4A}},
{0x06,1,{0x4C}},
{0x07,1,{0x4E}},
{0x08,1,{0x50}},
{0x09,1,{0x55}},
{0x0A,1,{0x52}},
{0x0B,1,{0x55}},
{0x0C,1,{0x55}},
{0x0D,1,{0x55}},
{0x0E,1,{0x55}},
{0x0F,1,{0x55}},
{0x10,1,{0x55}},
{0x11,1,{0x55}},
{0x12,1,{0x55}},
{0x13,1,{0x40}},
{0x14,1,{0x55}},
{0x15,1,{0x55}},

//GIP_R Pin mapping
{0x16,1,{0x57}},
{0x17,1,{0x77}},
{0x18,1,{0x45}},
{0x19,1,{0x47}},
{0x1A,1,{0x49}},
{0x1B,1,{0x4B}},
{0x1C,1,{0x4D}},
{0x1D,1,{0x4F}},
{0x1E,1,{0x51}},
{0x1F,1,{0x55}},
{0x20,1,{0x53}},
{0x21,1,{0x55}},
{0x22,1,{0x55}},
{0x23,1,{0x55}},
{0x24,1,{0x55}},
{0x25,1,{0x55}},
{0x26,1,{0x55}},
{0x27,1,{0x55}},
{0x28,1,{0x55}},
{0x29,1,{0x41}},
{0x2A,1,{0x55}},
{0x2B,1,{0x55}},
                      
//GIP_L_GS Pin mapping
{0x2C,1,{0x57}},
{0x2D,1,{0x77}},   
{0x2E,1,{0x4F}}, 
{0x2F,1,{0x4D}}, 
{0x30,1,{0x4B}}, 
{0x31,1,{0x49}}, 
{0x32,1,{0x47}}, 
{0x33,1,{0x45}}, 
{0x34,1,{0x41}}, 
{0x35,1,{0x55}}, 
{0x36,1,{0x53}}, 
{0x37,1,{0x55}}, 
{0x38,1,{0x55}}, 
{0x39,1,{0x55}}, 
{0x3A,1,{0x55}}, 
{0x3B,1,{0x55}}, 
{0x3C,1,{0x55}}, 
{0x3D,1,{0x55}}, 
{0x3E,1,{0x55}}, 
{0x3F,1,{0x51}}, 
{0x40,1,{0x55}}, 
{0x41,1,{0x55}},
 
//GIP_R_GS Pin mapping
{0x42,1,{0x57}},
{0x43,1,{0x77}},   
{0x44,1,{0x4F}},
{0x45,1,{0x4D}},
{0x46,1,{0x4B}},
{0x47,1,{0x49}},
{0x48,1,{0x47}},
{0x49,1,{0x45}},
{0x4A,1,{0x41}},
{0x4B,1,{0x55}}, 
{0x4C,1,{0x53}},
{0x4D,1,{0x55}}, 
{0x4E,1,{0x55}}, 
{0x4F,1,{0x55}}, 
{0x50,1,{0x55}}, 
{0x51,1,{0x55}}, 
{0x52,1,{0x55}}, 
{0x53,1,{0x55}}, 
{0x54,1,{0x55}}, 
{0x55,1,{0x51}},
{0x56,1,{0x55}}, 
{0x57,1,{0x55}}, 

//GIP Timing  
{0x58,1,{0x40}}, 
{0x59,1,{0x00}}, 
{0x5A,1,{0x00}}, 
{0x5B,1,{0x10}}, 
{0x5C,1,{0x09}}, 
{0x5D,1,{0x30}}, 
{0x5E,1,{0x01}}, 
{0x5F,1,{0x02}}, 
{0x60,1,{0x30}}, 
{0x61,1,{0x03}}, 
{0x62,1,{0x04}}, 
{0x63,1,{0x06}}, 
{0x64,1,{0x6A}},
{0x65,1,{0x75}},
{0x66,1,{0x0F}},
{0x67,1,{0xB3}},
{0x68,1,{0x0B}},
{0x69,1,{0x06}},
{0x6A,1,{0x6A}},
{0x6B,1,{0x10}}, 
{0x6C,1,{0x00}}, 
{0x6D,1,{0x04}}, 
{0x6E,1,{0x04}}, 
{0x6F,1,{0x88}}, 
{0x70,1,{0x00}}, 
{0x71,1,{0x00}}, 
{0x72,1,{0x06}}, 
{0x73,1,{0x7B}}, 
{0x74,1,{0x00}}, 
{0x75,1,{0xBC}}, 
{0x76,1,{0x00}}, 
{0x77,1,{0x05}}, 
{0x78,1,{0x2E}}, 
{0x79,1,{0x00}}, 
{0x7A,1,{0x00}}, 
{0x7B,1,{0x00}}, 
{0x7C,1,{0x00}}, 
{0x7D,1,{0x03}}, 
{0x7E,1,{0x7B}}, 

{0xE0,1,{0x04}}, //ESD
{0x09,1,{0x10}},
{0x2B,1,{0x2B}}, 
{0x2E,1,{0x44}}, 
//Page0
{0xE0,1,{0x00}},
//open watch dog
{0xE6,1,{0x02}},
{0xE7,1,{0x02}},

{0x35,1,{0x00}},

{0x11,1,{0x00}},                 // Sleep-Out

{REGFLAG_DELAY, 120, {}},
//{REGFLAG_DELAY,100,{}},

{0x29,1,{0x00}},                 // Display On

{REGFLAG_DELAY, 20, {}},
//{REGFLAG_DELAY,120,{}},

//{REGFLAG_END_OF_TABLE,0x00,{}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
    unsigned int i;

    for(i = 0; i < count; i++) {
        
        unsigned cmd;
        cmd = table[i].cmd;
        
        switch (cmd) {
			/*case 0xd9:
			table[i].para_list[0]=vcom;
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
            vcom+=2;
			break;
			*/
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
                
            case REGFLAG_END_OF_TABLE :
                break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
                MDELAY(1);
        }
    }
    
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS * util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS * params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

    // enable tearing-free
    params->dbi.te_mode             = LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;



    params->dsi.mode   = SYNC_EVENT_VDO_MODE;


    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM                = LCM_FOUR_LANE;
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
	
	params->dsi.vertical_sync_active = 4;
	params->dsi.vertical_backporch = 12;
	params->dsi.vertical_frontporch =30;
	params->dsi.vertical_active_line				= FRAME_HEIGHT; 

	params->dsi.horizontal_sync_active = 24; 
	params->dsi.horizontal_backporch = 48;
	params->dsi.horizontal_frontporch = 40;
	params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

// Bit rate calculation
//1 Every lane speed
//params->dsi.pll_div1=0;		// div1=0,1,1,3;div1_real=1,1,4,4 ----0: 546Mbps  1:273Mbps
//params->dsi.pll_div2=0;		// div2=0,1,1,3;div1_real=1,1,4,4	
//params->dsi.fbk_div =0x12;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

// zhangxiaofei add for test
	params->dsi.PLL_CLOCK = 230;
	params->dsi.noncont_clock=1;
	params->dsi.noncont_clock_period=1;
	params->dsi.ssc_disable= 1;	
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd = 0x0a;
	params->dsi.lcm_esd_check_table[0].count = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
    MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(120);

	push_table(lcm_initialization_setting,sizeof(lcm_initialization_setting) /sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
    unsigned int data_array[16];
    data_array[0]=0x00280500;
    dsi_set_cmdq(data_array, 1, 1);
    MDELAY(10);
    data_array[0]=0x00100500;
    dsi_set_cmdq(data_array, 1, 1);
	MDELAY(100);
    SET_RESET_PIN(1);
    MDELAY(120);
    SET_RESET_PIN(0);
	MDELAY(1); // 1ms
	SET_RESET_PIN(1);
	MDELAY(120);
}


static void lcm_resume(void)
{
    //unsigned int id;
	//unsigned char buffer[5];
	//unsigned int array[5];
	//lcm_compare_id();
//	push_table(lcm_sleep_out_setting,sizeof(lcm_sleep_out_setting) /sizeof(struct LCM_setting_table), 1);
	lcm_init();
			
/*array[0] = 0x00063902;// read id return two byte,version and id
	array[1] = 0x52AA55F0;
	array[2] = 0x00000108;
	dsi_set_cmdq(array, 3, 1);
	MDELAY(10);
	
	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xc5, buffer, 2);
	id = ((buffer[0] << 8) | buffer[1]);
#if defined(BUILD_LK)
printf("%s, [rm68191_ctc50_jhzt]  buffer[0] = [0x%d] buffer[2] = [0x%d] ID = [0x%d]\n",__func__, buffer[0], buffer[1], id);
#else
printk("%s, [rm68191_ctc50_jhzt]  buffer[0] = [0x%d] buffer[2] = [0x%d] ID = [0x%d]\n",__func__, buffer[0], buffer[1], id);
#endif*/
}



static unsigned int lcm_compare_id(void)
{
    int array[4];
    char buffer[5];
	unsigned int id=0;
	unsigned int id1=0;
	unsigned int id2=0;
    
    SET_RESET_PIN(1);
	  SET_RESET_PIN(0);
		MDELAY(10);
	  SET_RESET_PIN(1);
		MDELAY(120);

    array[0] = 0x00023700;// read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1);
	MDELAY(10);
	read_reg_v2(0xda, buffer, 2);

	id = buffer[0];
	read_reg_v2(0xdb, buffer, 2);
	id1 = buffer[0];
	read_reg_v2(0xdc, buffer, 2);
	id2 = buffer[0];   

    #ifdef BUILD_LK
	printf("jd9163 id = 0x%08x\n",  id);
    #else

	printk("jd9163 id = 0x%08x\n",  id);
    #endif
	return (LCM_ID == id)?1:0;//id1

}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hct_jd9365aa_dsi_vdo_hd_cmo_55_sh = 
{
	.name			= "hct_jd9365aa_dsi_vdo_hd_cmo_55_sh",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,	
	.compare_id     = lcm_compare_id,	
    //.esd_check   	= lcm_esd_check,	
    //.esd_recover   	= lcm_esd_recover,	
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif	//wqtao
};

