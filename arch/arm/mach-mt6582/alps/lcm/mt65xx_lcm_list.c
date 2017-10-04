#include <lcm_drv.h>
#ifdef BUILD_LK
#include <platform/disp_drv_platform.h>
#else
#include <linux/delay.h>
#include <mach/mt_gpio.h>
#endif
#include <cust_gpio_usage.h>
//used to identify float ID PIN status
#define LCD_HW_ID_STATUS_LOW      0
#define LCD_HW_ID_STATUS_HIGH     1
#define LCD_HW_ID_STATUS_FLOAT 0x02
#define LCD_HW_ID_STATUS_ERROR  0x03

#ifdef BUILD_LK
#define LCD_DEBUG(fmt)  dprintf(CRITICAL,fmt)
#else
#define LCD_DEBUG(fmt)  printk(fmt)
#endif

extern LCM_DRIVER nt35512_wvga_dsi_vdo_txd_lcm_drv;
extern LCM_DRIVER otm8018b_wvga_dsi_vdo_dijing_lcm_drv;
extern LCM_DRIVER otm8019a_wvga_dsi_vdo_dijing_lcm_drv;



LCM_DRIVER* lcm_driver_list[] = 
{
#if defined(OTM8018B_WVGA_DSI_VDO_DIJING)
	&otm8018b_wvga_dsi_vdo_dijing_lcm_drv,
#endif

#if defined(OTM8019A_WVGA_DSI_VDO_DIJING)
	&otm8019a_wvga_dsi_vdo_dijing_lcm_drv,
#endif
  
#if defined(NT35512_WVGA_DSI_VDO_TXD)
	&nt35512_wvga_dsi_vdo_txd_lcm_drv,
#endif


};

#define LCM_COMPILE_ASSERT(condition) LCM_COMPILE_ASSERT_X(condition, __LINE__)
#define LCM_COMPILE_ASSERT_X(condition, line) LCM_COMPILE_ASSERT_XX(condition, line)
#define LCM_COMPILE_ASSERT_XX(condition, line) char assertion_failed_at_line_##line[(condition)?1:-1]

unsigned int lcm_count = sizeof(lcm_driver_list)/sizeof(LCM_DRIVER*);
LCM_COMPILE_ASSERT(0 != sizeof(lcm_driver_list)/sizeof(LCM_DRIVER*));

