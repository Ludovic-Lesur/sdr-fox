/*
 * nmpa_registers.h
 *
 *  Created on: 17 mar. 2023
 *      Author: Ludo
 */

#ifndef __NMPA_REGISTERS_H__
#define __NMPA_REGISTERS_H__

#include "types.h"

/*** NMPA REGISTERS macros ***/

#define NMPA_BASE                               0x0009FC00

#define NMPA_DCDC_POWER_PROFILE_LOW_0_ADDR      (NMPA_BASE + 0xE0)
#define NMPA_DCDC_POWER_PROFILE_LOW_0           (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_LOW_0_ADDR))

#define NMPA_DCDC_POWER_PROFILE_LOW_1_ADDR      (NMPA_BASE + 0xE4)
#define NMPA_DCDC_POWER_PROFILE_LOW_1           (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_LOW_1_ADDR))

#define NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDR   (NMPA_BASE + 0xE8)
#define NMPA_DCDC_POWER_PROFILE_MEDIUM_0        (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_MEDIUM_0_ADDR))

#define NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDR   (NMPA_BASE + 0xEC)
#define NMPA_DCDC_POWER_PROFILE_MEDIUM_1        (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_MEDIUM_1_ADDR))

#define NMPA_DCDC_POWER_PROFILE_HIGH_0_ADDR     (NMPA_BASE + 0xD8)
#define NMPA_DCDC_POWER_PROFILE_HIGH_0          (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_HIGH_0_ADDR))

#define NMPA_DCDC_POWER_PROFILE_HIGH_1_ADDR     (NMPA_BASE + 0xDC)
#define NMPA_DCDC_POWER_PROFILE_HIGH_1          (*((volatile uint32_t*) NMPA_DCDC_POWER_PROFILE_HIGH_1_ADDR))

#define NMPA_PVT_MONITOR_0_RINGO_ADDR           (NMPA_BASE + 0x130)
#define NMPA_PVT_MONITOR_0_RINGO                (*((volatile uint32_t*) NMPA_PVT_MONITOR_0_RINGO_ADDR))

#define NMPA_PVT_MONITOR_1_RINGO_ADDR           (NMPA_BASE + 0x140)
#define NMPA_PVT_MONITOR_1_RINGO                (*((volatile uint32_t*) NMPA_PVT_MONITOR_1_RINGO_ADDR))

#endif /* __NMPA_REGISTERS_H__ */
