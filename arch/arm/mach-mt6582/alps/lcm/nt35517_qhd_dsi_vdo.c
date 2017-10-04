#ifndef BUILD_LK
#include <linux/string.h>
#endif
 
#include "lcm_drv.h"
#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
    #include <string.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
  
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  										(540)
#define FRAME_HEIGHT 										(960)


#define LCM_DSI_CMD_MODE									0

#define LCM_ID_HX8389B 0x89

#define HX8389B (1)     // main source
#define NT35517 (0)     // second source

#ifndef TRUE
    #define TRUE  1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

//unsigned int lcm_esd_test = FALSE; // only for ESD test
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util;
#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))
#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))



// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(para_tbl,size,force_update)        lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                                    lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)  



static  LCM_setting_table_V3 lcm_initialization_setting[] = {
	/*
	Note:
	Data ID will depends on the following rule.
	    count of parameters > 1 ==> DataID=0x39
	    count of parameters = 1 ==> DataID=0x15
	    count of parameters = 0 ==> DataID=0x05
	Structure format:
	    {DataID, DCS command, count of parameters, {parameter list}},
	    {REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, milliseconds, {}},
	*/
	
	// Set extension command
	{0x39, 0xF0,  5, {0x55, 0xAA, 0x52, 0x08, 0x00}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
  {0x15, 0xB3,  1, {0x00}},
  //{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
  {0x15, 0xBC,  1, {0x04}},
  //{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
  {0x39, 0xB7,  2, {0x00,0x00}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xB8,  4, {0x01,0x04,0x04,0x04}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xC7,  1, {0x70}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xCA,  11, {0x01, 0xE4, 0xE4, 0xE4, 0xE4, 0xE4,
		                 0xE4, 0x08, 0x08, 0x00, 0x00}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xF0,  5, {0x55, 0xAA, 0x52, 0x08, 0x01}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB0,  1, {0x0A}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB6,  1, {0x43}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB1,  1, {0x0A}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB7,  1, {0x33}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB2,  1, {0x03}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB8,  1, {0x14}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB3,  1, {0x10}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB9,  1, {0x34}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xB4,  1, {0x07}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xBA,  1, {0x24}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xBC,  3, {0x00, 0x8C, 0x01}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xBD,  3, {0x00,0x8D, 0x01}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xBE,  1, {0x42}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x15, 0xCF,  1, {0x04}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD1,  16, {0x00,0x00,0x00,0x51,0x00,0x80,
		                 0x00,0xA4,0x00,0xC1,0x00,0xE9,
		                 0x01,0x09,0x01,0x38}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD2,  16, {0x01,0x5F,0x01,0x9E,0x01,0xD0,
		                 0x02,0x1D,0x02,0x5C,0x02,0x5E,
		                 0x02,0x98,0x02,0xD8}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD3,  16, {0x03,0x01,0x03,0x38,0x03,0x5A,
		                 0x03,0x8A,0x03,0xA9,0x03,0xD0,
		                 0x03,0xE2,0x03,0xF5}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD4,  4, {0x03,0xFE,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD5,  16, {0x00,0x1C,0x00,0x67,0x00,0xA1,
		                 0x00,0xC4,0x00,0xDF,0x01,0x08,
		                 0x01,0x27,0x01,0x59}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD6,  16, {0x01,0x82,0x01,0xBE,0x01,0xF0,
		                 0x02,0x3D,0x02,0x7D,0x02,0x7F,
		                 0x02,0xB6,0x02,0xF0}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD7,  16, {0x03,0x15,0x03,0x47,0x03,0x69,
		                 0x03,0x93,0x03,0xAB,0x03,0xCD,
		                 0x03,0xDF,0x03,0xF1}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD8,  4, {0x03,0xFC,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xD9,  16, {0x00,0x1C,0x00,0x67,0x00,0xA1,
		                 0x00,0xC4,0x00,0xDF,0x01,0x08,
		                 0x01,0x27,0x01,0x59}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xDD,  16, {0x01,0x82,0x01,0xBE,0x01,0xF0,
		                 0x02,0x3D,0x02,0x7D,0x02,0x7F,
		                 0x02,0xB6,0x02,0xF0}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xDE,  16, {0x03,0x15,0x03,0x47,0x03,0x69,
		                 0x03,0x93,0x03,0xAB,0x03,0xCD,
		                 0x03,0xDF,0x03,0xF1}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xDF,  4, {0x03,0xFC,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE0,  16, {0x00,0x1C,0x00,0x67,0x00,0xA1,
		                 0x00,0xC4,0x00,0xDF,0x01,0x08,
		                 0x01,0x27,0x01,0x59}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE1,  16, {0x01,0x82,0x01,0xBE,0x01,0xF0,
		                 0x02,0x3D,0x02,0x7D,0x02,0x7F,
		                 0x02,0xB6,0x02,0xF0}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE2,  16, {0x03,0x15,0x03,0x47,0x03,0x69,
		                 0x03,0x93,0x03,0xAB,0x03,0xCD,
		                 0x03,0xDF,0x03,0xF1}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE3,  4, {0x03,0xFC,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE4,  16, {0x00,0x1C,0x00,0x67,0x00,0xA1,
		                 0x00,0xC4,0x00,0xDF,0x01,0x08,
		                 0x01,0x27,0x01,0x59}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE5,  16, {0x01,0x82,0x01,0xBE,0x01,0xF0,
		                 0x02,0x3D,0x02,0x7D,0x02,0x7F,
		                 0x02,0xB6,0x02,0xF0}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE6,  16, {0x03,0x15,0x03,0x47,0x03,0x69,
		                 0x03,0x93,0x03,0xAB,0x03,0xCD,
		                 0x03,0xDF,0x03,0xF1}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE7,  4, {0x03,0xFC,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE8,  16, {0x00,0x1C,0x00,0x67,0x00,0xA1,
		                 0x00,0xC4,0x00,0xDF,0x01,0x08,
		                 0x01,0x27,0x01,0x59}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xE9,  16, {0x01,0x82,0x01,0xBE,0x01,0xF0,
		                 0x02,0x3D,0x02,0x7D,0x02,0x7F,
		                 0x02,0xB6,0x02,0xF0}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xEA,  16, {0x03,0x15,0x03,0x47,0x03,0x69,
		                 0x03,0x93,0x03,0xAB,0x03,0xCD,
		                 0x03,0xDF,0x03,0xF1}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},
	{0x39, 0xEB,  4, {0x03,0xFC,0x03,0xFF}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 1, {}},

	// --- Resume ------------------------------------
	// Do resume within initial is NOT recommended!
	/*
	// Sleep out
	{0x05, 0x11, 0, {}},
	{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 800, {}},


	// Display on
	{0x05, 0x29, 0, {}},
	{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 10, {}},
	*/
};


static LCM_setting_table_V3 lcm_suspend_setting[] = {
	// Display off
	//{0x05, 0x28, 0, {}},
	//{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 20, {}},
	// Note:
	// Remove the Display off command is a workaround
	// to reduce the idle currect of Himax HX8389-B driver IC.
		
	// Sleep in
	{0x05, 0x10, 0, {}},
	{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 120, {}},
};
		
			
static LCM_setting_table_V3 lcm_resume_setting[] = {
	// Sleep out
	{0x05, 0x11, 0, {}},
	{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 120, {}},
				
	// Display on
	{0x05, 0x29, 0, {}},
	{REGFLAG_ESCAPE_ID, REGFLAG_DELAY_MS_V3, 20, {}},
};

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static int getLCMID(){
  int lcm_id = 0;
	 mt_set_gpio_mode(GPIO_LCM_ID, GPIO_MODE_00);
   mt_set_gpio_dir(GPIO_LCM_ID, GPIO_DIR_IN);
   mt_set_gpio_pull_enable(GPIO_LCM_ID, GPIO_PULL_DISABLE);
   lcm_id = mt_get_gpio_in(GPIO_LCM_ID);
   return lcm_id;
}
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

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
#else // Video modes: SYNC_PULSE_VDO_MODE; SYNC_EVENT_VDO_MODE; BURST_VDO_MODE;
	params->dsi.mode = SYNC_PULSE_VDO_MODE;
#endif
	params->dsi.LANE_NUM				= LCM_TWO_LANE;
	params->dsi.PS                 = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.data_format.format		= LCM_DSI_FORMAT_RGB888;
	
	params->dsi.vertical_sync_active    = 20;//4;
	params->dsi.vertical_backporch      = 20;
	params->dsi.vertical_frontporch     = 20;//8;
	params->dsi.vertical_active_line	= FRAME_HEIGHT; 
	
	params->dsi.horizontal_sync_active  = 30;//24;
	params->dsi.horizontal_backporch    = 30;//24;
	params->dsi.horizontal_frontporch   = 30;//24;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;
	// Required BRPL (Bit Rate per Lane):
	// = (4+20+8+960)*(24+24+24+540)*RGB24bit*60fps / 2 lane
	// = 437,114,880 bit/s

	params->dsi.PLL_CLOCK = 250; // Range: 25..625 MHz
	// DRPL (Data Rate per Lane) = 2*PLL_CLOCK

	// Spread Spectrum Clocking setting (MT6582 new added)
	params->dsi.ssc_disable = 0; // 0:enable, 1:disable
	params->dsi.ssc_range   = 5; // Range: 1..8 (default: 5)

}

static void lcm_init(void)
{  
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(40);
	SET_RESET_PIN(1);
	MDELAY(120);

	dsi_set_cmdq_V3(lcm_initialization_setting, sizeof(lcm_initialization_setting)/sizeof(lcm_initialization_setting[0]), 1);
	dsi_set_cmdq_V3(lcm_resume_setting, sizeof(lcm_resume_setting)/sizeof(lcm_resume_setting[0]), 1);
}


static void lcm_suspend(void)
{
	dsi_set_cmdq_V3(lcm_suspend_setting, sizeof(lcm_suspend_setting)/sizeof(lcm_suspend_setting[0]), 1);
}


static void lcm_resume(void)
{
	dsi_set_cmdq_V3(lcm_resume_setting, sizeof(lcm_resume_setting)/sizeof(lcm_resume_setting[0]), 1);
}


#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width  - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = ( x0       & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = ( x1       & 0xFF);
	unsigned char y0_MSB = ((y0 >> 8) & 0xFF);
	unsigned char y0_LSB = ( y0       & 0xFF);
	unsigned char y1_MSB = ((y1 >> 8) & 0xFF);
	unsigned char y1_LSB = ( y1       & 0xFF);

	unsigned int data_array[16];


	
       data_array[0]=0x00053902;
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

        data_array[0]=0x00053902;
	data_array[1] = (y1_MSB << 24) | (y0_LSB << 16) | (y0_MSB << 8) | 0x2b;
	data_array[2] = (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);
}
#endif




static unsigned int lcm_compare_id(void)
{
  #ifdef BUILD_LK
        int LCM_ID = 0;
        LCM_ID = getLCMID();
        if(LCM_ID == NT35517){
            printf("%s, LK selected NT35517 \n", __func__);
            printf("[LK/LCM] lcm_compare_id selected NT35517 devices\n");
            return 1;
        }else {
	          printf("%s, LK selected HX8389B \n", __func__);
	          printf("[LK/LCM] lcm_compare_id selected HX8389B devices\n");
	          return 0;
        }
   #else
       return 0;
   #endif
}



static unsigned int lcm_esd_check(void)
{
	unsigned int ret = FALSE;
#ifndef BUILD_LK
	char buffer[6];
	int  array[4];
	
	//if (lcm_esd_test) {
	//	lcm_esd_test = FALSE;
	//	return TRUE;
	//}
	array[0] = 0x00083700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0x0A, buffer, 2);
	//printk("esd buffer0=%x, buffer1=%x\n", buffer[0], buffer[1]);
	//read_reg_v2(0x09, buffer, 5);
	//printk("esd buffer0=%x, buffer1=%x buffer2=%x, buffer3=%x, buffer4=%x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
	if (buffer[0] == 0x1C) {
		ret = FALSE;
	}else{
		ret = TRUE;
	}
#endif
	return ret;
}


static unsigned int lcm_esd_recover(void)
{
	lcm_init();

	#ifndef BUILD_LK
	printk("lcm_esd_recover: hx8389b_qhd_dsi_vdo_tianma\n");
	#endif
	return TRUE;
}

LCM_DRIVER nt35517_qhd_dsi_vdo_lcm_drv = 
{
	.name			= "nt35517_qhd_dsi_vdo",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,	
	//.esd_check      = lcm_esd_check,
	//.esd_recover    = lcm_esd_recover,
#if (LCM_DSI_CMD_MODE)
	.update         = lcm_update,
#endif
};

