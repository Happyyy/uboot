/*
 * U-boot - main board file
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/portmux.h>
#include <asm/arch/sc57x.h>
#include <asm/arch-sc57x/dwmmc.h>
#include <watchdog.h>
#include "soft_switch.h"

DECLARE_GLOBAL_DATA_PTR;
int board_early_init_f(void)
{
#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_init();
#endif

	return 0;
}

void set_spu_securep_msec(int n, bool msec)
{
	void __iomem *p = (void __iomem *)(REG_SPU0_CTL + 0xA00 + 4 * n);
	u32 securep = readl(p);

	if (msec)
		writel(securep | 0x2, p);
	else
		writel(securep & ~0x2, p);
}

/* miscellaneous platform dependent initialisations */
int misc_init_r(void)
{
	printf("other init\n");
	set_spu_securep_msec(41, 1);
	set_spu_securep_msec(45, 1);
	set_spu_securep_msec(140, 1);
	return setup_board_switches();
}

unsigned long flash_init(void)
{

#if 0
	/*  Enable bank 1 of smc because AMS1 is connected to enable of SRAM */
	*pREG_SMC0_GCTL = 0x10;
	*pREG_SMC0_B1CTL = 0x00000001;
	*pREG_SMC0_B1TIM = 0x05220522;
#endif
	return 0;
}

int dram_init()
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

void s_init(void)
{
}

#ifdef CONFIG_DESIGNWARE_ETH
int board_eth_init(bd_t *bis)
{
	int ret = 0;

	if (CONFIG_DW_PORTS & 1) {
		gpio_request(GPIO_PA5, "emac0_phy_reset");
		gpio_direction_output(GPIO_PA5, 0);
		mdelay(1);
		gpio_direction_output(GPIO_PA5, 1);

		mdelay(1);
		writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);

		static const unsigned short pins[] = P_RGMII0;
		if (!peripheral_request_list(pins, "emac0"))
			ret += designware_initialize(REG_EMAC0_MACCFG,
					PHY_INTERFACE_MODE_RGMII);
	}

	return ret;
}

int board_phy_config(struct phy_device *phydev)
{
	int  phy_data = 0;

	if (CONFIG_DW_PORTS & 1) {
		phy_data = phy_read(phydev, MDIO_DEVAD_NONE, 0x12);

		/* enable 3com mode for RGMII */
		phy_write(phydev, MDIO_DEVAD_NONE, 0x12, (3 << 12) | phy_data);
	}

	if (CONFIG_DW_PORTS & 2)
		phy_write(phydev, MDIO_DEVAD_NONE, 0x11, 3);

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	int ret;
#ifdef CONFIG_DWMMC
	ret = sc5xx_dwmmc_init(bis);
	if (ret)
		printf("dwmmc init failed\n");
#endif
	return ret;
}
#endif

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_SC573_EZKIT;
	/* boot param addr */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + (0x100);

	return 0;
}

