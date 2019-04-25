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

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <string.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
//	#include <mach/mt_gpio.h>
#endif


// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH                                         (1080)
#define FRAME_HEIGHT                                        (1920)
#define LCM_ID                       (0x1322)

#define REGFLAG_DELAY               (0XFE)
#define REGFLAG_END_OF_TABLE        (0x100) // END OF REGISTERS MARKER


#define LCM_DSI_CMD_MODE                                    0

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)                                    (lcm_util.set_reset_pin((v)))

#define UDELAY(n)                                           (lcm_util.udelay(n))
#define MDELAY(n)                                           (lcm_util.mdelay(n))

//static kal_bool IsFirstBoot = KAL_TRUE;

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)    lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)       lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                                      lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)                  lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg                                            lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)               lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};


static struct LCM_setting_table lcm_initialization_setting[] = {
//R61322A_H546DAN08
//VS=2, VBP=10, VFP=12 //HS=8, HBP=80, HFP=80
{0xB0,1,{0x04}},
{0xD6,1,{0x01}},
{0xB3,6,{0x14,0x40,0x00,0x00,0x00,0x00}},
{0xB4,2,{0x0C,0x00}},//
{0xB6,2,{0x3A,0xD3}},
{0xC1,42,{0x8C,0x60,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x10,0x02,
0x0F,0x0A,0x00,0x88,0x00,0x00,0x01,0x00,0x00,0x00,0x62,0x30,0x40,0xA5,0x0F,0x04,0x20}},
{0xC2,9,{0x20,0xF0,0x07,0x80,0x0C,0x0C,0x00,0x00,0x00}},
{0xC4,21,{0x70,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x01,0x00,0x00,0x01,0x02}},
{0xC6,11,{0xC8,0x28,0x4B,0x1E,0x41,0x00,0x00,0x00,0x00,0x00,0x00}},
//GAMMA2.2

{0xC7,30,{0x08,0x1F,0x26,0x30,0x3D,0x49,0x53,0x61,0x45,0x4C,0x57,0x63,0x6B,0x72,0x7D,0x08,0x1F,0x27,0x2F,0x3D,0x49,0x52,0x60,0x44,0x4C,0x57,0x63,0x6B,0x72,0x7D}},
//GAMMA2.5
//{0xC7,30,0x09,0x21,0x29,0x33,0x40,0x4C,0x56,0x64,0x46,0x4F,0x5A,0x65,0x6D,0x73,0x7D,0x07,0x1F,0x27,0x30,0x3F,0x4C,0x55,0x63,0x49,0x50,0x5C,0x69,0x6F,0x73,0x7D}},
{0xCB,13,{0xFF,0xFF,0x0F,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xCC,1,{0x23}},
{0xCD,41,{0x01,0x04,0x04,0x04,0x04,0x01,0x03,0x02,0x10,0x0F,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x00,0x00,0x12,0x04,0x04,0x04,0x04,0x12,0x00,0x00,
0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x02,0x03,0x01,0x04,0x04,0x04,0x04}},
{0xD0,13,{0x11,0x59,0xBB,0x68,0x99,0x4C,0x99,0x11,0x19,0x04,0xDD,0x14,0x00}},

{0xD2,2,{0x88,0x6F}},
{0xD3,29,{0xA3,0x33,0xBB,0xBB,0xB3,0x33,0x33,0x33,0x00,0x00,0x0A,0x8B,0x82,0x20,0x20,0x36,0x36,0x33,0x33,0x37,0x60,0xFD,0xFE,0x07,0x10,0x00,0x00,0x00,0x52}},
{0xD5,13,{0x06,0x00,0x00,0x00,0x97,0x00,0x94,0x01,0x00,0x00,0x00,0x00,0x00}},

{REGFLAG_DELAY, 10, {}},
{0x29,   1,  {0X00}},
{REGFLAG_DELAY, 30, {}},
{0x11,   1,  {0X00}},
{REGFLAG_DELAY, 150, {}},
};




static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {

        // Display off sequence
    {0x28, 1, {0x00}},
    {REGFLAG_DELAY, 20, {}},
    
 //   {0xD3,25,{0x13,0x33,0xBB,0xB3,0xB3,0x33,0x33,0x33,0x00,0x01,0x00,0xA0,0xE8,0xA0,0x0D,0x46,0x46,0x33,0x3B,0x37,0x72,0x57,0x3D,0xBF,0x33}},
  //  {REGFLAG_DELAY, 120, {}},
    // Sleep Mode On
    {0x10, 1, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    {REGFLAG_END_OF_TABLE, 0x00, {}}
    

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
                
            case REGFLAG_END_OF_TABLE :
                break;
                
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
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

	params->physical_width = 68;
	params->physical_height = 122;

    // enable tearing-free
    params->dbi.te_mode             = LCM_DBI_TE_MODE_DISABLED;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;

    params->dsi.mode   = BURST_VDO_MODE;

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
    params->dsi.intermediat_buffer_num = 0;
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    params->dsi.vertical_sync_active                = 2;//2;//
    params->dsi.vertical_backporch                  = 6;//38;//
    params->dsi.vertical_frontporch                 = 8;//40;//
    params->dsi.vertical_active_line                = FRAME_HEIGHT; 
    params->dsi.horizontal_sync_active              = 12;//24;//
    params->dsi.horizontal_backporch                =80;//75;//
    params->dsi.horizontal_frontporch               = 80;//75;//
 //   params->dsi.horizontal_blanking_pixel              = 60;
    params->dsi.horizontal_active_pixel            = FRAME_WIDTH;
    // Bit rate calculation

    params->dsi.PLL_CLOCK=430;//230 430
   // params->dsi.ssc_disable=1;

	params->dsi.clk_lp_per_line_enable = 1; // 1 clk line by line return LP11 0 clk frame by frame return LP11
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 0;
    params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
	params->dsi.lcm_esd_check_table[0].count = 1;
    params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
    params->dsi.noncont_clock = 1; // 1 clk noncontinue 非连续 0 continue 连续
    params->dsi.noncont_clock_period = 2;
}

static void lcm_init(void)
{
    SET_RESET_PIN(1);  //shm
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ONE);  //shm
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 1);
#endif
    MDELAY(10);

#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ONE);
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 1);
#endif

    MDELAY(20);

    SET_RESET_PIN(1);
    MDELAY(10);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
  
    push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);   //wqtao. enable

    SET_RESET_PIN(1);  //shm
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(20);
    //SET_RESET_PIN(1);
   // MDELAY(20);

#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ZERO);
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 0);    // 2V8 off
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ZERO);  //shm
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 0);
#endif
    MDELAY(5);
}


static void lcm_resume(void)
{
#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ZERO);
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 0);    // 2V8 off
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ZERO);  //shm
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 0);
#endif
    MDELAY(5);

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
    unsigned int id = 0;
    unsigned char buffer[5];
    unsigned int array[16];

#ifdef GPIO_LCM_LDO_1V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_1V8_EN_PIN, GPIO_OUT_ONE);  //shm
#else   

    mt_dsi_pinctrl_set(LCM_POWER_DM_NO, 1);
#endif
    MDELAY(5);

#ifdef GPIO_LCM_LDO_2V8_EN_PIN
    lcm_util.set_gpio_mode(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_MODE_00);
    lcm_util.set_gpio_dir(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_DIR_OUT);
    lcm_util.set_gpio_out(GPIO_LCM_LDO_2V8_EN_PIN, GPIO_OUT_ONE);
#else   
    mt_dsi_pinctrl_set(LCM_POWER_DP_NO, 1);
#endif
    MDELAY(5);

    SET_RESET_PIN(1);  //shm
    MDELAY(10);
    SET_RESET_PIN(0);
    MDELAY(10);
    SET_RESET_PIN(1);
    MDELAY(20);

    array[0] = 0x00053700;  // read id return two byte,version and id
    dsi_set_cmdq(array, 1, 1);
    read_reg_v2(0xbf, buffer, 5);
     MDELAY(20);
    id = ((buffer[2] << 8) | buffer[3]);    //we only need ID

#ifdef BUILD_LK
	printf("%s,  otm1287a+tm id = 0x%08x\n", __func__, id);
#else
	printk("%s,  otm1287a+tm id = 0x%08x\n", __func__, id);
#endif
    return (LCM_ID == id)?1:0;
   //return 1;
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER hct_r61322a_dsi_vdo_fhd_55_ot = 
{
    .name           = "hct_r61322a_dsi_vdo_fhd_55_ot",
    .set_util_funcs = lcm_set_util_funcs,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,   
    .compare_id    = lcm_compare_id,    
#if (LCM_DSI_CMD_MODE)
    .update         = lcm_update,
#endif
};

