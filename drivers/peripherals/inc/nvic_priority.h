/*
 * nvic_priority.h
 *
 *  Created on: 05 apr. 2025
 *      Author: Ludo
 */

#ifndef __NVIC_PRIORITY_H__
#define __NVIC_PRIORITY_H__

/*!******************************************************************
 * \enum NVIC_priority_list_t
 * \brief NVIC interrupt priorities list.
 *******************************************************************/
typedef enum {
    NVIC_PRIORITY_USB_HS_DEVICE = 0,
    NVIC_PRIORITY_SYSTICK,
} NVIC_priority_list_t;

#endif /* __NVIC_PRIORITY_H__ */
