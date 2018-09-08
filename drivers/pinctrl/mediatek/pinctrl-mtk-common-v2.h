/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2018 MediaTek Inc.
 *
 * Author: Sean Wang <sean.wang@mediatek.com>
 *
 */

#ifndef __PINCTRL_MTK_COMMON_V2_H
#define __PINCTRL_MTK_COMMON_V2_H

#define MTK_INPUT      0
#define MTK_OUTPUT     1
#define MTK_DISABLE    0
#define MTK_ENABLE     1
#define MTK_PULLDOWN   0
#define MTK_PULLUP     1

#define EINT_NA	-1

#define PIN_FIELD_CALC(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit,	\
			_x_bits, _sz_reg, _fixed) {			\
		.s_pin = _s_pin,					\
		.e_pin = _e_pin,					\
		.s_addr = _s_addr,					\
		.x_addrs = _x_addrs,					\
		.s_bit = _s_bit,					\
		.x_bits = _x_bits,					\
		.sz_reg = _sz_reg,					\
		.fixed = _fixed,					\
	}

#define PIN_FIELD(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit, _x_bits)	\
	PIN_FIELD_CALC(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit,	\
		       _x_bits, 32, 0)

#define PINS_FIELD(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit, _x_bits)	\
	PIN_FIELD_CALC(_s_pin, _e_pin, _s_addr, _x_addrs, _s_bit,	\
		       _x_bits, 32, 1)

/* List these attributes which could be modified for the pin */
enum {
	PINCTRL_PIN_REG_MODE,
	PINCTRL_PIN_REG_DIR,
	PINCTRL_PIN_REG_DI,
	PINCTRL_PIN_REG_DO,
	PINCTRL_PIN_REG_SR,
	PINCTRL_PIN_REG_SMT,
	PINCTRL_PIN_REG_PD,
	PINCTRL_PIN_REG_PU,
	PINCTRL_PIN_REG_E4,
	PINCTRL_PIN_REG_E8,
	PINCTRL_PIN_REG_TDSEL,
	PINCTRL_PIN_REG_RDSEL,
	PINCTRL_PIN_REG_DRV,
	PINCTRL_PIN_REG_PUPD,
	PINCTRL_PIN_REG_R0,
	PINCTRL_PIN_REG_R1,
	PINCTRL_PIN_REG_IES,
	PINCTRL_PIN_REG_PULLEN,
	PINCTRL_PIN_REG_PULLSEL,
	PINCTRL_PIN_REG_MAX,
};

/* Group the pins by the driving current */
enum {
	DRV_FIXED,
	DRV_GRP0,
	DRV_GRP1,
	DRV_GRP2,
	DRV_GRP3,
	DRV_GRP4,
	DRV_GRP_MAX,
};

/* struct mtk_pin_field - the structure that holds the information of the field
 *			  used to describe the attribute for the pin
 * @offset:		the register offset relative to the base address
 * @mask:		the mask used to filter out the field from the register
 * @bitpos:		the start bit relative to the register
 * @next:		the indication that the field would be extended to the
			next register
 */
struct mtk_pin_field {
	u32 offset;
	u32 mask;
	u8  bitpos;
	u8  next;
};

/* struct mtk_pin_field_calc - the structure that holds the range providing
 *			       the guide used to look up the relevant field
 * @s_pin:		the start pin within the range
 * @e_pin:		the end pin within the range
 * @s_addr:		the start address for the range
 * @x_addrs:		the address distance between two consecutive registers
 *			within the range
 * @s_bit:		the start bit for the first register within the range
 * @x_bits:		the bit distance between two consecutive pins within
 *			the range
 * @sz_reg:		the size of bits in a register
 * @fixed:		the consecutive pins share the same bits with the 1st
 *			pin
 */
struct mtk_pin_field_calc {
	u16 s_pin;
	u16 e_pin;
	u32 s_addr;
	u8  x_addrs;
	u8  s_bit;
	u8  x_bits;
	u8  sz_reg;
	u8  fixed;
};

/* struct mtk_pin_reg_calc - the structure that holds all ranges used to
 *			     determine which register the pin would make use of
 *			     for certain pin attribute.
 * @range:		     the start address for the range
 * @nranges:		     the number of items in the range
 */
struct mtk_pin_reg_calc {
	const struct mtk_pin_field_calc *range;
	unsigned int nranges;
};

/**
 * struct mtk_pin_desc - the structure that providing information
 *			       for each pin of chips
 * @number:		unique pin number from the global pin number space
 * @name:		name for this pin
 * @eint_n:		the eint number for this pin
 * @drv_n:		the index with the driving group
 */
struct mtk_pin_desc {
	unsigned int number;
	const char *name;
	u16 eint_n;
	u8 drv_n;
};

struct mtk_pinctrl;

/* struct mtk_pin_soc - the structure that holds SoC-specific data */
struct mtk_pin_soc {
	const struct mtk_pin_reg_calc	*reg_cal;
	const struct pinctrl_pin_desc	*pins;
	unsigned int			npins;
	const struct group_desc		*grps;
	unsigned int			ngrps;
	const struct function_desc	*funcs;
	unsigned int			nfuncs;
	const struct mtk_eint_regs	*eint_regs;
	const struct mtk_eint_hw	*eint_hw;

	/* Specific parameters per SoC */
	u8				gpio_m;
	u8				eint_m;
	bool				ies_present;

	/* Specific pinconfig operations */
	int (*bias_disable_set)(struct mtk_pinctrl *hw,
				const struct mtk_pin_desc *desc);
	int (*bias_disable_get)(struct mtk_pinctrl *hw,
				const struct mtk_pin_desc *desc, int *res);
	int (*bias_set)(struct mtk_pinctrl *hw,
			const struct mtk_pin_desc *desc, bool pullup);
	int (*bias_get)(struct mtk_pinctrl *hw,
			const struct mtk_pin_desc *desc, bool pullup, int *res);

	int (*drive_set)(struct mtk_pinctrl *hw,
			 const struct mtk_pin_desc *desc, u32 arg);
	int (*drive_get)(struct mtk_pinctrl *hw,
			 const struct mtk_pin_desc *desc, int *val);

	int (*adv_pull_set)(struct mtk_pinctrl *hw,
			    const struct mtk_pin_desc *desc, bool pullup,
			    u32 arg);
	int (*adv_pull_get)(struct mtk_pinctrl *hw,
			    const struct mtk_pin_desc *desc, bool pullup,
			    u32 *val);
};

struct mtk_pinctrl {
	struct pinctrl_dev		*pctrl;
	void __iomem			*base;
	struct device			*dev;
	struct gpio_chip		chip;
	const struct mtk_pin_soc        *soc;
	struct mtk_eint			*eint;
};

void mtk_rmw(struct mtk_pinctrl *pctl, u32 reg, u32 mask, u32 set);

int mtk_hw_set_value(struct mtk_pinctrl *hw, const struct mtk_pin_desc *desc,
		     int field, int value);
int mtk_hw_get_value(struct mtk_pinctrl *hw, const struct mtk_pin_desc *desc,
		     int field, int *value);

int mtk_pinconf_bias_disable_set(struct mtk_pinctrl *hw,
				 const struct mtk_pin_desc *desc);
int mtk_pinconf_bias_disable_get(struct mtk_pinctrl *hw,
				 const struct mtk_pin_desc *desc, int *res);
int mtk_pinconf_bias_set(struct mtk_pinctrl *hw,
			 const struct mtk_pin_desc *desc, bool pullup);
int mtk_pinconf_bias_get(struct mtk_pinctrl *hw,
			 const struct mtk_pin_desc *desc, bool pullup,
			 int *res);

int mtk_pinconf_bias_disable_set_rev1(struct mtk_pinctrl *hw,
				      const struct mtk_pin_desc *desc);
int mtk_pinconf_bias_disable_get_rev1(struct mtk_pinctrl *hw,
				      const struct mtk_pin_desc *desc,
				      int *res);
int mtk_pinconf_bias_set_rev1(struct mtk_pinctrl *hw,
			      const struct mtk_pin_desc *desc, bool pullup);
int mtk_pinconf_bias_get_rev1(struct mtk_pinctrl *hw,
			      const struct mtk_pin_desc *desc, bool pullup,
			      int *res);

int mtk_pinconf_drive_set(struct mtk_pinctrl *hw,
			  const struct mtk_pin_desc *desc, u32 arg);
int mtk_pinconf_drive_get(struct mtk_pinctrl *hw,
			  const struct mtk_pin_desc *desc, int *val);

int mtk_pinconf_drive_set_rev1(struct mtk_pinctrl *hw,
			       const struct mtk_pin_desc *desc, u32 arg);
int mtk_pinconf_drive_get_rev1(struct mtk_pinctrl *hw,
			       const struct mtk_pin_desc *desc, int *val);

int mtk_pinconf_adv_pull_set(struct mtk_pinctrl *hw,
			     const struct mtk_pin_desc *desc, bool pullup,
			     u32 arg);
int mtk_pinconf_adv_pull_get(struct mtk_pinctrl *hw,
			     const struct mtk_pin_desc *desc, bool pullup,
			     u32 *val);

#endif /* __PINCTRL_MTK_COMMON_V2_H */
