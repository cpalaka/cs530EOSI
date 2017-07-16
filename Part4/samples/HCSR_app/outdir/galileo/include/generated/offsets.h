/* include/generated/offsets.h.tmp - structure member offsets definition header */

/*
 * Copyright (c) 2010-2014 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* THIS FILE IS AUTO GENERATED.  PLEASE DO NOT EDIT */

/*
 * This header file provides macros for the offsets of various structure
 * members.  These offset macros are primarily intended to be used in
 * assembly code.
 */

/*
 * Auto-generated header guard.
 */
#ifndef _HGUARD_650ca0d8
#define _HGUARD_650ca0d8
 
#ifdef __cplusplus
extern "C" {
#endif

/* defines */

#define	___kernel_t_current_OFFSET	0x8
#define	___kernel_t_nested_OFFSET	0x0
#define	___kernel_t_irq_stack_OFFSET	0x4
#define	___thread_base_t_flags_OFFSET	0x8
#define	___thread_base_t_prio_OFFSET	0xC
#define	___thread_base_t_sched_locked_OFFSET	0x10
#define	___thread_base_t_swap_data_OFFSET	0x14
#define	___thread_t_base_OFFSET	0x0
#define	___thread_t_caller_saved_OFFSET	0x30
#define	___thread_t_callee_saved_OFFSET	0x30
#define	___thread_t_arch_OFFSET	0x40
#define	K_THREAD_SIZEOF	0x40
#define	___thread_arch_t_coopFloatReg_OFFSET	0x0
#define	___thread_arch_t_preempFloatReg_OFFSET	0x0
#define	_K_THREAD_NO_FLOAT_SIZEOF	0x40
#define	___callee_saved_t_esp_OFFSET	0x0
#define	__tSwapStk_eax_OFFSET	0x0
#define	__tSwapStk_ebp_OFFSET	0x4
#define	__tSwapStk_ebx_OFFSET	0x8
#define	__tSwapStk_esi_OFFSET	0xC
#define	__tSwapStk_edi_OFFSET	0x10
#define	__tSwapStk_retAddr_OFFSET	0x14
#define	__tSwapStk_param_OFFSET	0x18
#define	__tSwapStk_SIZEOF	0x1C
#define	__NANO_ESF_esp_OFFSET	0x0
#define	__NANO_ESF_ebp_OFFSET	0x4
#define	__NANO_ESF_ebx_OFFSET	0x8
#define	__NANO_ESF_esi_OFFSET	0xC
#define	__NANO_ESF_edi_OFFSET	0x10
#define	__NANO_ESF_edx_OFFSET	0x14
#define	__NANO_ESF_ecx_OFFSET	0x1C
#define	__NANO_ESF_eax_OFFSET	0x18
#define	__NANO_ESF_errorCode_OFFSET	0x20
#define	__NANO_ESF_eip_OFFSET	0x24
#define	__NANO_ESF_cs_OFFSET	0x28
#define	__NANO_ESF_eflags_OFFSET	0x2C
#define	__ISR_LIST_SIZEOF	0x14

#ifdef __cplusplus
}
#endif

#endif /* _HGUARD_ */
