/*
 * pmc.c
 *
 *  Created on: Mar 15, 2023
 *      Author: ludo
 */

#include "pmc.h"

#include "pmc_reg.h"
#include "nmpa_reg.h"
#include "types.h"

/*** PMC local macros ***/

#define PMC_DCDC_POWER_PROFILE_LOW_MAX_FREQUENCY_HZ		100000000
#define PMC_DCDC_POWER_PROFILE_MEDIUM_MAX_FREQUENCY_HZ	130000000
#define PMC_DCDC_POWER_PROFILE_HIGH_MAX_FREQUENCY_HZ	150000000

#define PMC_PROCESS_NNN_AVG_HZ		19300000
#define PMC_PROCESS_NNN_STD_HZ		400000
#define PMC_PROCESS_NNN_LIMITS		6
#define PMC_PROCESS_NNN_MIN_HZ		(PMC_PROCESS_NNN_AVG_HZ - (PMC_PROCESS_NNN_LIMITS * PMC_PROCESS_NNN_STD_HZ))
#define PMC_PROCESS_NNN_MAX_HZ		(PMC_PROCESS_NNN_AVG_HZ + (PMC_PROCESS_NNN_LIMITS * PMC_PROCESS_NNN_STD_HZ))

#define PMC_VOLTAGE_SSS_LOW_MV		1075
#define PMC_VOLTAGE_SSS_MEDIUM_MV	1150
#define PMC_VOLTAGE_SSS_HIGH_MV		1200
#define PMC_VOLTAGE_NNN_LOW_MV		1000
#define PMC_VOLTAGE_NNN_MEDIUM_MV	1100
#define PMC_VOLTAGE_NNN_HIGH_MV		1150
#define PMC_VOLTAGE_FFF_LOW_MV		1000
#define PMC_VOLTAGE_FFF_MEDIUM_MV	1025
#define PMC_VOLTAGE_FFF_HIGH_MV		1050

/*** PMC local structures ***/

typedef enum {
	PMC_DCDC_POWER_PROFILE_LOW = 0,
	PMC_DCDC_POWER_PROFILE_MEDIUM,
	PMC_DCDC_POWER_PROFILE_HIGH,
	PMC_DCDC_POWER_PROFILE_LAST
} PMC_dc_dc_profile_t;

typedef enum {
    PMC_PROCESS_CORNER_SSS = 0,
    PMC_PROCESS_CORNER_NNN,
    PMC_PROCESS_CORNER_FFF,
    PMC_PROCESS_CORNER_OTHERS,
} PMC_process_corner_t;

typedef enum {
    PMC_V_DCDC_0P950 = 0,
    PMC_V_DCDC_0P975 = 1,
    PMC_V_DCDC_1P000 = 2,
    PMC_V_DCDC_1P025 = 3,
    PMC_V_DCDC_1P050 = 4,
    PMC_V_DCDC_1P075 = 5,
    PMC_V_DCDC_1P100 = 6,
    PMC_V_DCDC_1P125 = 7,
    PMC_V_DCDC_1P150 = 8,
    PMC_V_DCDC_1P175 = 9,
    PMC_V_DCDC_1P200 = 10
} PMC_v_dcdc_t;

typedef enum {
	PMC_V_AO_0P700 = 1,
	PMC_V_AO_0P725 = 2,
	PMC_V_AO_0P750 = 3,
	PMC_V_AO_0P775 = 4,
	PMC_V_AO_0P800 = 5,
	PMC_V_AO_0P825 = 6,
	PMC_V_AO_0P850 = 7,
	PMC_V_AO_0P875 = 8,
	PMC_V_AO_0P900 = 9,
	PMC_V_AO_0P960 = 10,
	PMC_V_AO_0P970 = 11,
	PMC_V_AO_0P980 = 12,
	PMC_V_AO_0P990 = 13,
	PMC_V_AO_1P000 = 14,
	PMC_V_AO_1P010 = 15,
	PMC_V_AO_1P020 = 16,
	PMC_V_AO_1P030 = 17,
	PMC_V_AO_1P040 = 18,
	PMC_V_AO_1P050 = 19,
	PMC_V_AO_1P060 = 20,
	PMC_V_AO_1P070 = 21,
	PMC_V_AO_1P080 = 22,
	PMC_V_AO_1P090 = 23,
	PMC_V_AO_1P100 = 24,
	PMC_V_AO_1P110 = 25,
	PMC_V_AO_1P120 = 26,
	PMC_V_AO_1P130 = 27,
	PMC_V_AO_1P140 = 28,
	PMC_V_AO_1P150 = 29,
	PMC_V_AO_1P160 = 30,
	PMC_V_AO_1P220 = 31
} PMC_v_ao_t;

/*** PMC local global variables ***/

static const uint8_t PMC_PDEN_BIT_INDEX[PMC_BLOCK_LAST] = {3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 18, 19, 20, 22, 23};

/*** PMC local functions ***/

/* READ PROCESS CORNER.
 * @param:					None.
 * @return process_corner:	Process corner.
 */
PMC_process_corner_t _PMC_get_process_corner(void) {
	// Local variables.
	PMC_process_corner_t part_process_corner = PMC_PROCESS_CORNER_SSS;
	uint32_t pvt_ringo_hz = 0;
	uint32_t pvt_ringo_0 = NMPA_PVT_MONITOR_0_RINGO;
	uint32_t pvt_ringo_1 = NMPA_PVT_MONITOR_1_RINGO;
	// Read corner.
	if ((pvt_ringo_0 & (0b1 << 0)) != 0) {
		// PVT Trimmings in Flash are valid.
		pvt_ringo_0 = pvt_ringo_0 >> 1;
	}
	else {
		// PVT Trimmings in Flash are NOT valid (average value assumed).
		pvt_ringo_0 = PMC_PROCESS_NNN_AVG_HZ;
	}
	if ((pvt_ringo_1 & (0b1 << 0)) != 0) {
		// PVT Trimmings in Flash are valid */
		pvt_ringo_1 = pvt_ringo_1 >> 1;
	}
	else {
		// PVT Trimmings in Flash are NOT valid (average value assumed).
		pvt_ringo_1 = PMC_PROCESS_NNN_AVG_HZ;
	}
	if (pvt_ringo_1 <= pvt_ringo_0) {
		pvt_ringo_hz = pvt_ringo_1;
	}
	else {
		pvt_ringo_hz = pvt_ringo_0;
	}
	// Determine the process corner based on the value of the Ring Oscillator frequency.
	if (pvt_ringo_hz <= PMC_PROCESS_NNN_MIN_HZ) {
		part_process_corner = PMC_PROCESS_CORNER_SSS;
	}
	else {
		if (pvt_ringo_hz <= PMC_PROCESS_NNN_MAX_HZ) {
			part_process_corner = PMC_PROCESS_CORNER_NNN;
		}
		else {
			part_process_corner = PMC_PROCESS_CORNER_FFF;
		}
	}
	return part_process_corner;
}

/* SET SYSTEM VOLTAGE.
 * @param system_voltage_mv:	System voltage in mV.
 * @return:						None.
 */
void _PMC_set_system_voltage(uint32_t system_voltage_mv) {
	// Local variables.
	uint32_t lv_ldo_ao = (uint32_t) PMC_V_AO_1P100;
	uint32_t lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P150;
	uint32_t lv_dcdc = (uint32_t) PMC_V_DCDC_1P100;
	uint32_t reg_value = 0;
	// Check system voltage
	if (system_voltage_mv <= 950) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_0P950;
		lv_ldo_ao = (uint32_t) PMC_V_AO_0P960;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P010;
	}
	else if (system_voltage_mv <= 975) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_0P975;
		lv_ldo_ao = (uint32_t) PMC_V_AO_0P980;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P030;
	}
	else if (system_voltage_mv <= 1000) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P000;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P000;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P050;
	}
	else if (system_voltage_mv <= 1025) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P025;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P030;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P080;
	}
	else if (system_voltage_mv <= 1050) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P050;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P060;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P110;
	}
	else if (system_voltage_mv <= 1075) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P075;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P080;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P130;
	}
	else if (system_voltage_mv <= 1100) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P100;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P100;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P150;
	}
	else if (system_voltage_mv <= 1125) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P125;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P130;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P160;
	}
	else if (system_voltage_mv <= 1150) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P150;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P160;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P220;
	}
	else if (system_voltage_mv <= 1175) {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P175;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P160;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P220;
	}
	else {
		lv_dcdc = (uint32_t) PMC_V_DCDC_1P200;
		lv_ldo_ao = (uint32_t) PMC_V_AO_1P160;
		lv_ldo_ao_boost = (uint32_t) PMC_V_AO_1P220;
	}
	// Update LDOPMU register.
	reg_value = (PMC -> LDOPMU);
	reg_value &= 0xFFFFF83E0;
	reg_value |= ((lv_ldo_ao & 0x1F) << 0) | ((lv_ldo_ao_boost & 0x1F) << 10);
	PMC -> LDOPMU = reg_value;
	// Update DCDC0 register.
	reg_value = (PMC -> DCDC0);
	reg_value &=0xFFE1FFFF;
	reg_value |= ((lv_dcdc & 0x0F) << 17);
	PMC -> DCDC0 = reg_value;
}

/* SET DC-DC TRIM VALUES.
 * @param profile:	DC-DC profile.
 * @return status:	Function execution status.
 */
PMC_status_t _PMC_set_dc_dc_trim(PMC_dc_dc_profile_t profile) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	uint32_t dc_dc_trim_value0 = 0;
	uint32_t dc_dc_trim_value1 = 0;
	// Check profile.
	switch (profile) {
	case PMC_DCDC_POWER_PROFILE_LOW:
		dc_dc_trim_value0 = NMPA_DCDC_POWER_PROFILE_LOW_0;
		dc_dc_trim_value1 = NMPA_DCDC_POWER_PROFILE_LOW_1;
		break;
	case PMC_DCDC_POWER_PROFILE_MEDIUM:
		dc_dc_trim_value0 = NMPA_DCDC_POWER_PROFILE_MEDIUM_0;
		dc_dc_trim_value1 = NMPA_DCDC_POWER_PROFILE_MEDIUM_1;
		break;
	case PMC_DCDC_POWER_PROFILE_HIGH:
		dc_dc_trim_value0 = NMPA_DCDC_POWER_PROFILE_HIGH_0;
		dc_dc_trim_value1 = NMPA_DCDC_POWER_PROFILE_HIGH_1;
		break;
	default:
		status = PMC_ERROR_DCDC_POWER_PROFILE;
		goto errors;
	}
	// Update registers.
	if ((dc_dc_trim_value0 & (0b1 << 0)) != 0) {
		dc_dc_trim_value0 = (dc_dc_trim_value0 >> 1);
		PMC -> DCDC0 = dc_dc_trim_value0;
		PMC -> DCDC1 = dc_dc_trim_value1;
	}
errors:
	return status;
}

/* SET DC-DC VOLTAGE.
 * @param profile:	DC-DC profile.
 * @return status:	Function execution status.
 */
PMC_status_t _PMC_set_dc_dc_voltage(PMC_dc_dc_profile_t profile) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	PMC_process_corner_t part_process_corner = PMC_PROCESS_CORNER_SSS;
	// Read process corner.
	part_process_corner = _PMC_get_process_corner();
    switch (part_process_corner) {
	case PMC_PROCESS_CORNER_SSS:
		switch (profile) {
		case PMC_DCDC_POWER_PROFILE_MEDIUM:
			_PMC_set_system_voltage(PMC_VOLTAGE_SSS_MEDIUM_MV);
			break;
		case PMC_DCDC_POWER_PROFILE_HIGH:
			_PMC_set_system_voltage(PMC_VOLTAGE_SSS_HIGH_MV);
			break;
		default:
			_PMC_set_system_voltage(PMC_VOLTAGE_SSS_LOW_MV);
			break;
		}
		break;

	case PMC_PROCESS_CORNER_FFF:
		switch (profile) {
		case PMC_DCDC_POWER_PROFILE_MEDIUM:
			_PMC_set_system_voltage(PMC_VOLTAGE_FFF_MEDIUM_MV);
			break;
		case PMC_DCDC_POWER_PROFILE_HIGH:
			_PMC_set_system_voltage(PMC_VOLTAGE_FFF_HIGH_MV);
			break;
		default:
			_PMC_set_system_voltage(PMC_VOLTAGE_FFF_LOW_MV);
			break;
		}
		break;
	default:
		switch (profile) {
		case PMC_DCDC_POWER_PROFILE_MEDIUM:
			_PMC_set_system_voltage(PMC_VOLTAGE_NNN_MEDIUM_MV);
			break;
		case PMC_DCDC_POWER_PROFILE_HIGH:
			_PMC_set_system_voltage(PMC_VOLTAGE_NNN_HIGH_MV);
			break;
		default:
			_PMC_set_system_voltage(PMC_VOLTAGE_NNN_LOW_MV);
			break;
		}
		break;
	}
errors:
	return status;
}

/*** PMC functions ***/

/* CONFIGURE INTERNAL DC-DC REGULATOR ACCORDING TO SYSTEM CLOCK FREQUENCY.
 * @param system_clock_frequency_hz:	System clock frequency in Hz.
 * @return status:						Function execution status.
 */
PMC_status_t PMC_configure_dc_dc(uint32_t system_clock_frequency_hz) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	if (system_clock_frequency_hz <= PMC_DCDC_POWER_PROFILE_LOW_MAX_FREQUENCY_HZ) {
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_LOW);
		if (status != PMC_SUCCESS) goto errors;
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_LOW);
	}
	else if (system_clock_frequency_hz <= PMC_DCDC_POWER_PROFILE_MEDIUM_MAX_FREQUENCY_HZ) {
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_MEDIUM);
		if (status != PMC_SUCCESS) goto errors;
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_MEDIUM);
	}
	else {
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_HIGH);
		if (status != PMC_SUCCESS) goto errors;
		status = _PMC_set_dc_dc_trim(PMC_DCDC_POWER_PROFILE_HIGH);
	}
errors:
	return status;
}

/* TURN BLOCK ON.
 * @param block:	Bloc to power up.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_power_on(PMC_block_t block) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check parameter.
	if (block >= PMC_BLOCK_LAST) {
		status = PMC_ERROR_BLOCK;
		goto errors;
	}
	// Clear power down bit.
	PMC -> PDRUNCFGCLR0 = (0b1 << PMC_PDEN_BIT_INDEX[block]);
errors:
	return status;
}

/* TURN BLOCK OFF.
 * @param block:	Bloc to power down.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_power_off(PMC_block_t block) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check parameter.
	if (block >= PMC_BLOCK_LAST) {
		status = PMC_ERROR_BLOCK;
		goto errors;
	}
	// Set power down bit.
	PMC -> PDRUNCFGSET0 = (0b1 << PMC_PDEN_BIT_INDEX[block]);
errors:
	return status;
}

/* SELECT 32K-OSC CLOCK SOURCE.
 * @param source:	Clock source.
 * @return status:	Function execution status.
 */
PMC_status_t PMC_set_32k_osc_source(PMC_32k_osc_source_t source) {
	// Local variables.
	PMC_status_t status = PMC_SUCCESS;
	// Check source.
	switch (source) {
	case PMC_32K_OSC_SOURCE_FRO32K:
		PMC -> RTCOSC32K &= ~(0b1 << 0);
		break;
	case PMC_32K_OSC_SOURCE_XTAL32K:
		PMC -> RTCOSC32K |= (0b1 << 0);
		break;
	default:
		status = PMC_ERROR_32K_OSC_SOURCE;
		goto errors;
	}
errors:
	return status;
}

/* READ XTAL32K STATUS.
 * @param:	None.
 * @return:	'1' if the XTAL32K is running, '0' otherwise.
 */
uint8_t PMC_xtal32k_is_ready(void) {
	// Read XTAL32KOK bit.
	return ((PMC -> STATUSCLK) & (0b1 << 0));
}
