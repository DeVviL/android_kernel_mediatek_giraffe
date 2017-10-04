
include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Module project related part

ifeq ($(CONFIG_MTK_SENSOR_SUPPORT),y)
ifeq ($(CONFIG_MTK_TMD2772_AUTO),y)
obj-y	+=  alsps/tmd2772_auto/
endif

ifeq ($(CONFIG_MTK_KXCJK_1013_AUTO),y)
obj-y   +=  accelerometer/kxcjk_1013_auto/
endif

ifeq ($(CONFIG_MTK_LIS3DH_AUTO),y)
obj-y   +=  accelerometer/lis3dh_auto/
endif

#ifeq ($(CONFIG_MTK_MPU3000),y)
#obj-y   +=  gyroscope/mpu3000/
#endif

#ifeq ($(CONFIG_MTK_MPU3000_NEW),y)
#obj-y   +=  gyroscope/mpu3000-new/
#endif

ifeq ($(CONFIG_MTK_ST480_AUTO),y)
obj-y	+=  magnetometer/st480_auto/
endif

ifeq ($(CONFIG_MTK_MMC3516X_AUTO),y)
obj-y	+=  magnetometer/mmc3516x_auto/
endif


endif

ifneq ($(CONFIG_ARCH_MTK_PROJECT),)
ARCH_MTK_PROJECT := $(shell echo $(CONFIG_ARCH_MTK_PROJECT))
obj-y	+= core/
obj-y	+= dct/dct/
endif

obj-y   +=  lcm/
#obj-y	+=  hdmi/
obj-y	+=  camera/camera/
obj-y	+=  imgsensor/t4k04_mipi_raw/
obj-y	+=  imgsensor/ov12830_mipi_raw/
obj-y	+=  imgsensor/ov5648_mipi_raw/
obj-y	+=  imgsensor/ov5645_mipi_yuv/
obj-y	+=  imgsensor/gc0311_yuv/
obj-y	+=  imgsensor/hi708_yuv/
obj-y   +=  flashlight/constant_flashlight/
obj-y	+=  vibrator/
obj-y	+=  leds/
obj-y	+=  keypad/
obj-y	+=  sound/
obj-y	+=  accdet/
obj-y	+=  partition/

