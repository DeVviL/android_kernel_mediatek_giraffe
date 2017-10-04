/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   mt6583.c
 *
 * Project:
 * --------
 *   MT6583  Audio Driver Kernel Function
 *
 * Description:
 * ------------
 *   Audio register
 *
 * Author:
 * -------
 * Chipeng Chang
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/


/*****************************************************************************
 *                     C O M P I L E R   F L A G S
 *****************************************************************************/


/*****************************************************************************
 *                E X T E R N A L   R E F E R E N C E S
 *****************************************************************************/

#include "AudDrv_Common.h"
#include "AudDrv_Def.h"
#include "AudDrv_Afe.h"
#include "AudDrv_Ana.h"
#include "AudDrv_Clk.h"
#include "mt_soc_afe_control.h"
#include "mt_soc_pcm_common.h"

/*
 *    function implementation
 */

static int mtk_afe_routing_probe(struct platform_device *pdev);
static int mtk_routing_pcm_close(struct snd_pcm_substream *substream);
static int mtk_asoc_routing_pcm_new(struct snd_soc_pcm_runtime *rtd);
static int mtk_afe_routing_platform_probe(struct snd_soc_platform *platform);

static int mDac_Sinegen = 27;
static const char *DAC_DL_SIDEGEN[] =
    { "I0I1", "I2", "I3I4", "I5I6", "I7I8", "I9", "I10I11", "I12I13", "I14", "I15I16", "I17I18",
"I19I20", "I21I22",
	"O0O1", "O2", "O3O4", "O5O6", "O7O8", "O9O10", "O11", "O12", "O13O14", "O15O16", "O17O18",
	    "O19O20", "O21O22", "O23O24", "OFF"
};

static int mDac_SampleRate = 7;
static const char *DAC_DL_SIDEGEN_SAMEPLRATE[] =
    { "8K", "11K", "12K", "16K", "24K", "32K", "44K", "48K" };

static int mDac_Sidegen_Amplitude = 6;
static const char *DAC_DL_SIDEGEN_AMPLITUE[] =
    { "1/128", "1/64", "1/32", "1/16", "1/8", "1/4", "1/2", "1" };

static bool mEnableSidetone;
static const char *ENABLESIDETONE[] = { "Off", "On" };

static bool AudDrvSuspendStatus;

static int Audio_SideGen_Get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("Audio_AmpR_Get = %d\n", mDac_Sinegen);
	ucontrol->value.integer.value[0] = mDac_Sinegen;
	return 0;
}

static int Audio_SideGen_Set(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	int index = 0;
	pr_debug("%s()\n", __func__);
	if (ucontrol->value.enumerated.item[0] > ARRAY_SIZE(DAC_DL_SIDEGEN)) {
		pr_debug("return -EINVAL\n");
		return -EINVAL;
	}
	index = ucontrol->value.integer.value[0];
	switch (index) {
	case 0:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I00,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 1:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I02,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 2:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I03,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 3:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I05,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 4:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I07,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 5:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I09,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 6:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I11,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 7:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I12,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 8:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I14,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 9:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I15,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 10:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I17,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 11:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I19,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 12:
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I21,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, true);
		break;
	case 13:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O01,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 14:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O02,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 15:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O03,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 16:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O05,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 17:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O07,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 18:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O09,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 19:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O11,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 20:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O12,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 21:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O13,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 22:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O15,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 23:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O17,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 24:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O19,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 25:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O21,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 26:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O23,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, true);
		break;
	case 27:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O11,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, false);
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I00,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, false);
		break;
	default:
		EnableSideGenHw(Soc_Aud_InterConnectionOutput_O11,
				Soc_Aud_MemIF_Direction_DIRECTION_OUTPUT, false);
		EnableSideGenHw(Soc_Aud_InterConnectionInput_I00,
				Soc_Aud_MemIF_Direction_DIRECTION_INPUT, false);
		break;
	}
	mDac_Sinegen = index;
	return 0;
}

static int Audio_SideGen_SampleRate_Get(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s\n", __func__);
	ucontrol->value.integer.value[0] = mDac_SampleRate;
	return 0;
}

static int Audio_SideGen_SampleRate_Set(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	int index = 0;
	pr_debug("%s()\n", __func__);

	if (ucontrol->value.enumerated.item[0] > ARRAY_SIZE(DAC_DL_SIDEGEN_SAMEPLRATE)) {
		pr_debug("return -EINVAL\n");
		return -EINVAL;
	}
	index = ucontrol->value.integer.value[0];
	mDac_SampleRate = index;
	return 0;
}

static int Audio_SideGen_Amplitude_Get(struct snd_kcontrol *kcontrol,
				       struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("Audio_AmpR_Get = %d\n", mDac_Sidegen_Amplitude);
	return 0;
}

static int Audio_SideGen_Amplitude_Set(struct snd_kcontrol *kcontrol,
				       struct snd_ctl_elem_value *ucontrol)
{
	int index = 0;
	pr_debug("%s()\n", __func__);

	if (ucontrol->value.enumerated.item[0] > ARRAY_SIZE(DAC_DL_SIDEGEN_AMPLITUE)) {
		pr_debug("return -EINVAL\n");
		return -EINVAL;
	}
	index = ucontrol->value.integer.value[0];
	mDac_Sidegen_Amplitude = index;
	return 0;
}

static int Audio_SideTone_Get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("Audio_SideTone_Get = %d\n", mEnableSidetone);
	ucontrol->value.integer.value[0] = mEnableSidetone;
	return 0;
}

static int Audio_SideTone_Set(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	int index = 0;
	pr_debug("%s()\n", __func__);

	if (ucontrol->value.enumerated.item[0] > ARRAY_SIZE(ENABLESIDETONE)) {
		pr_debug("return -EINVAL\n");
		return -EINVAL;
	}
	index = ucontrol->value.integer.value[0];
	if (mEnableSidetone != index) {
		mEnableSidetone = index;
		EnableSideToneFilter(mEnableSidetone);
	}
	return 0;
}

static const struct soc_enum Audio_Routing_Enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(DAC_DL_SIDEGEN), DAC_DL_SIDEGEN),
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(DAC_DL_SIDEGEN_SAMEPLRATE), DAC_DL_SIDEGEN_SAMEPLRATE),
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(DAC_DL_SIDEGEN_AMPLITUE), DAC_DL_SIDEGEN_AMPLITUE),
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(ENABLESIDETONE), ENABLESIDETONE),
};

static const struct snd_kcontrol_new Audio_snd_routing_controls[] = {
	SOC_ENUM_EXT("Audio_SideGen_Switch", Audio_Routing_Enum[0], Audio_SideGen_Get,
		     Audio_SideGen_Set),
	SOC_ENUM_EXT("Audio_SideGen_SampleRate", Audio_Routing_Enum[1],
		     Audio_SideGen_SampleRate_Get, Audio_SideGen_SampleRate_Set),
	SOC_ENUM_EXT("Audio_SideGen_Amplitude", Audio_Routing_Enum[2], Audio_SideGen_Amplitude_Get,
		     Audio_SideGen_Amplitude_Set),
	SOC_ENUM_EXT("Audio_Sidetone_Switch", Audio_Routing_Enum[3], Audio_SideTone_Get,
		     Audio_SideTone_Set),
};


void EnAble_Anc_Path(int state)
{
	pr_debug("%s state = %d\n ", __func__, state);
	switch (state) {
	case AUDIO_ANC_ON:
		AudDrv_Clk_On();
		AudDrv_ADC_Clk_On();
		AudDrv_ADC2_Clk_On();
		AudDrv_ADC3_Clk_On();

		Afe_Set_Reg(AFE_ADDA2_TOP_CON0, 0x00000008, 0x00000008);
		Afe_Set_Reg(AFE_ADDA2_NEWIF_CFG0, 0x03F87220, 0xffffffff);
		Afe_Set_Reg(AFE_ADDA2_NEWIF_CFG1, 0x03317980, 0xffffffff);
		Afe_Set_Reg(AFE_ADDA2_UL_SRC_CON0, 0x001e0001, 0xffffffff);
		Afe_Set_Reg(AFE_ADDA3_UL_SRC_CON0, 0x00000001, 0xffffffff);
		Afe_Set_Reg(AFE_ADDA3_UL_SRC_CON0, 0x1 << 17, 0x3 << 17);	/* downsample to 16k */
		Afe_Set_Reg(AFE_ADDA3_UL_SRC_CON0, 0x1 << 19, 0x3 << 19);	/* downsample to 16k */
		break;
	case AUDIO_ANC_OFF:
		Afe_Set_Reg(AFE_ADDA2_UL_SRC_CON0, 0x0, 0x1);
		Afe_Set_Reg(AFE_ADDA3_UL_SRC_CON0, 0x0, 0x1);
		Afe_Set_Reg(AFE_ADDA2_TOP_CON0, 0x00000000, 0x00000008);
		Afe_Set_Reg(AFE_ADDA2_NEWIF_CFG0, 0x03F87220, 0xffffffff);
		Afe_Set_Reg(AFE_ADDA2_NEWIF_CFG1, 0x03317980, 0xffffffff);
		AudDrv_ADC_Clk_Off();
		AudDrv_ADC2_Clk_Off();
		AudDrv_ADC3_Clk_Off();
		AudDrv_Clk_Off();
		break;
	default:
		break;
	}
}

static int m_Anc_State = AUDIO_ANC_ON;
static int Afe_Anc_Get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s()\n", __func__);
	ucontrol->value.integer.value[0] = m_Anc_State;
	return 0;
}

static int Afe_Anc_Set(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s()\n", __func__);
	EnAble_Anc_Path(ucontrol->value.integer.value[0]);
	m_Anc_State = ucontrol->value.integer.value[0];
	return 0;
}

/* here start uplink power function */
static const char *Afe_Anc_function[] = { "ANCON", "ANCOFF" };

static const struct soc_enum Afe_Anc_Enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(Afe_Anc_function), Afe_Anc_function),
};

static const struct snd_kcontrol_new Afe_Anc_controls[] = {
	SOC_ENUM_EXT("Pmic_Anc_Switch", Afe_Anc_Enum[0], Afe_Anc_Get, Afe_Anc_Set),
};

static struct snd_soc_pcm_runtime *pruntimepcm;

/* Conventional and unconventional sample rate supported */
static unsigned int soc_high_supported_sample_rates[] = {
	8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 88200, 96000, 176400, 192000
};

static struct snd_pcm_hw_constraint_list constraints_sample_rates = {
	.count = ARRAY_SIZE(soc_high_supported_sample_rates),
	.list = soc_high_supported_sample_rates,
	.mask = 0,
};

static int mtk_routing_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	int err = 0;
	int ret = 0;
	pr_debug("mtk_routing_pcm_open\n");

	ret = snd_pcm_hw_constraint_list(runtime, 0, SNDRV_PCM_HW_PARAM_RATE,
					 &constraints_sample_rates);
	if (ret < 0) {
		pr_debug("snd_pcm_hw_constraint_integer failed\n");
	}
	/* print for hw pcm information */
	pr_debug("mtk_routing_pcm_open runtime rate = %d channels = %d\n", runtime->rate,
		 runtime->channels);
	if (substream->pcm->device & 1) {
		runtime->hw.info &= ~SNDRV_PCM_INFO_INTERLEAVED;
		runtime->hw.info |= SNDRV_PCM_INFO_NONINTERLEAVED;
	}
	if (substream->pcm->device & 2)
		runtime->hw.info &= ~(SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		pr_debug("SNDRV_PCM_STREAM_PLAYBACK mtkalsa_playback_constraints\n");
	} else {

	}

	if (err < 0) {
		pr_debug("mtk_routing_pcm_close\n");
		mtk_routing_pcm_close(substream);
		return err;
	}
	pr_debug("mtk_routing_pcm_open return\n");
	return 0;
}

static int mtk_routing_pcm_close(struct snd_pcm_substream *substream)
{
	return 0;
}

static int mtk_routing_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	pr_debug("%s cmd = %d\n", __func__, cmd);
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		break;
	}
	return -EINVAL;
}

static int mtk_routing_pcm_copy(struct snd_pcm_substream *substream,
				int channel, snd_pcm_uframes_t pos,
				void __user *dst, snd_pcm_uframes_t count)
{
	count = count << 2;
	return 0;
}

static int mtk_routing_pcm_silence(struct snd_pcm_substream *substream,
				   int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t count)
{
	pr_debug("mtk_routing_pcm_silence\n");
	return 0;		/* do nothing */
}


static void *dummy_page[2];

static struct page *mtk_routing_pcm_page(struct snd_pcm_substream *substream, unsigned long offset)
{
	pr_debug("dummy_pcm_page\n");
	return virt_to_page(dummy_page[substream->stream]);	/* the same page */
}

static int mtk_routing_pcm_prepare(struct snd_pcm_substream *substream)
{
	pr_debug("mtk_alsa_prepare\n");
	return 0;
}

static int mtk_routing_pcm_hw_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *hw_params)
{
	int ret = 0;
	PRINTK_AUDDRV("mtk_routing_pcm_hw_params\n");
	return ret;
}

static int mtk_routing_pcm_hw_free(struct snd_pcm_substream *substream)
{
	PRINTK_AUDDRV("mtk_routing_pcm_hw_free\n");
	return snd_pcm_lib_free_pages(substream);
}

static struct snd_pcm_ops mtk_afe_ops = {
	.open = mtk_routing_pcm_open,
	.close = mtk_routing_pcm_close,
	.ioctl = snd_pcm_lib_ioctl,
	.hw_params = mtk_routing_pcm_hw_params,
	.hw_free = mtk_routing_pcm_hw_free,
	.prepare = mtk_routing_pcm_prepare,
	.trigger = mtk_routing_pcm_trigger,
	.copy = mtk_routing_pcm_copy,
	.silence = mtk_routing_pcm_silence,
	.page = mtk_routing_pcm_page,
};

static struct snd_soc_platform_driver mtk_soc_routing_platform = {
	.ops = &mtk_afe_ops,
	.pcm_new = mtk_asoc_routing_pcm_new,
	.probe = mtk_afe_routing_platform_probe,
};

static int mtk_afe_routing_probe(struct platform_device *pdev)
{
	pr_debug("mtk_afe_routing_probe\n");
	if (pdev->dev.of_node) {
		dev_set_name(&pdev->dev, "%s", MT_SOC_ROUTING_PCM);
	}

	pr_debug("%s: dev name %s\n", __func__, dev_name(&pdev->dev));
	return snd_soc_register_platform(&pdev->dev, &mtk_soc_routing_platform);
}

static int mtk_asoc_routing_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	int ret = 0;
	pruntimepcm = rtd;
	pr_debug("%s\n", __func__);
	return ret;
}

static int mtk_afe_routing_platform_probe(struct snd_soc_platform *platform)
{
	pr_debug("mtk_afe_routing_platform_probe\n");

	/* add  controls */
	snd_soc_add_platform_controls(platform, Audio_snd_routing_controls,
				      ARRAY_SIZE(Audio_snd_routing_controls));
	snd_soc_add_platform_controls(platform, Afe_Anc_controls, ARRAY_SIZE(Afe_Anc_controls));
	return 0;
}


static int mtk_afe_routing_remove(struct platform_device *pdev)
{
	pr_debug("%s\n", __func__);
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

/* supend and resume function */
static int mtk_routing_pm_ops_suspend(struct device *device)
{
	if (get_voice_status() == true) {
		return 0;
	}

	if (AudDrvSuspendStatus == false) {
		AudDrv_Clk_Power_On();
		BackUp_Audio_Register();
		AudDrv_Suspend_Clk_Off();
		AudDrvSuspendStatus = true;
	}
	return 0;
}

static int mtk_routing_pm_ops_resume(struct device *device)
{
	if (AudDrvSuspendStatus == true) {
		AudDrv_Suspend_Clk_On();
		Restore_Audio_Register();
		AudDrvSuspendStatus = false;
	}
	return 0;
}

struct dev_pm_ops mtk_routing_pm_ops = {
	.suspend = mtk_routing_pm_ops_suspend,
	.resume = mtk_routing_pm_ops_resume,
	.freeze = NULL,
	.thaw = NULL,
	.poweroff = NULL,
	.restore = NULL,
	.restore_noirq = NULL,
};

static struct platform_driver mtk_afe_routing_driver = {
	.driver = {
		   .name = MT_SOC_ROUTING_PCM,
		   .owner = THIS_MODULE,
#ifdef CONFIG_PM
		   .pm = &mtk_routing_pm_ops,
#endif
		   },
	.probe = mtk_afe_routing_probe,
	.remove = mtk_afe_routing_remove,
};

static struct platform_device *soc_mtkafe_routing_dev;

static int __init mtk_soc_routing_platform_init(void)
{
	int ret = 0;
	pr_debug("%s\n", __func__);

	soc_mtkafe_routing_dev = platform_device_alloc(MT_SOC_ROUTING_PCM, -1);
	if (!soc_mtkafe_routing_dev) {
		return -ENOMEM;
	}

	ret = platform_device_add(soc_mtkafe_routing_dev);
	if (ret != 0) {
		platform_device_put(soc_mtkafe_routing_dev);
		return ret;
	}

	ret = platform_driver_register(&mtk_afe_routing_driver);

	return ret;

}
module_init(mtk_soc_routing_platform_init);

static void __exit mtk_soc_routing_platform_exit(void)
{

	pr_debug("%s\n", __func__);
	platform_driver_unregister(&mtk_afe_routing_driver);
}
module_exit(mtk_soc_routing_platform_exit);

MODULE_DESCRIPTION("afe eouting driver");
MODULE_LICENSE("GPL");
