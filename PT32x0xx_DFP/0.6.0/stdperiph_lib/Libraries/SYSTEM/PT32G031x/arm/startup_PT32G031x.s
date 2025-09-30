;*******************************************************************************
;* File Name          : startup_PT32G031x.s
;* Author             : Application development team
;* Description        : PT32G031x devices vector table for MDK-ARM toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;********************************************************************************
;* @attention
;*
;* Copyright (c) 2024 Pai-IC Microelectronics.
;* All rights reserved.
;*
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;//   <o> Stack Size (in Bytes, must 8 byte aligned) <0-16384:8>
Stack_Size		EQU				1024

			AREA	STACK, NOINIT, READWRITE, ALIGN = 3
Stack_Mem		SPACE	Stack_Size
__initial_sp

;//   <o>  Heap Size (in Bytes) <0-16384:8>
Heap_Size		EQU				0

			AREA	HEAP, NOINIT, READWRITE, ALIGN = 3
__heap_base
Heap_Mem		SPACE	Heap_Size
__heap_limit

			PRESERVE8
			THUMB

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
; Vector Table Mapped to Address 0 at Reset
			AREA	RESET, CODE, READONLY
			EXPORT	__Vectors
			EXPORT	__Vectors_End
			EXPORT	__Vectors_Size

__Vectors		DCD	__initial_sp 			; Top address of Stack
			DCD	Reset_Handler				; Reset Handler
			DCD	NMI_Handler					; NMI Handler
			DCD	HardFault_Handler			; Hard Fault Handler
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	0							; Reserved						
			DCD	SVC_Handler					; SVC Handler
			DCD	0							; Reserved
			DCD	0							; Reserved
			DCD	PendSV_Handler				; PendSV Handler
			DCD	SysTick_Handler				; SysTick Handler

			; External Interrupt Handler
			DCD	PVD_Handler					;  16+ 0:  PVD_Handler
			DCD	0							;  16+ 1:  Reserved
			DCD	0							;  16+ 2:  Reserved
			DCD	IFMC_Handler				;  16+ 3:  IFMC_Handler
			DCD	DMA_Handler					;  16+ 4:  DMA_Handler
			DCD	EXTIA_Handler				;  16+ 5:  EXTIA_Handler
			DCD	EXTIB_Handler				;  16+ 6:  EXTIB_Handler
			DCD	0							;  16+ 7:  Reserved
			DCD	0							;  16+ 8:  Reserved
			DCD	CMP1_Handler				;  16+ 9:  CMP1_Handler
			DCD	0							;  16+ 10: Reserved
			DCD	0							;  16+ 11: Reserved
			DCD	ADC0_Handler				;  16+ 12: ADC0_Handler
			DCD	TIM1_Handler				;  16+ 13: TIM1_Handler
			DCD	0							;  16+ 14: Reserved
			DCD	TIM4_Handler				;  16+ 15: TIM4_Handler
			DCD	TIM3_Handler				;  16+ 16: TIM3_Handler
			DCD	TIM2_Handler				;  16+ 17: TIM2_Handler
			DCD	ALU_Handler					;  16+ 18: ALU_Handler
			DCD	0							;  16+ 19: Reserved
			DCD	0							;  16+ 20: Reserved
			DCD	CMP0_Handler				;  16+ 21: CMP0_Handler
			DCD	TIM8_Handler				;  16+ 22: TIM8_Handler
			DCD	I2C0_Handler				;  16+ 23: I2C0_Handler
			DCD	0							;  16+ 24: Reserved
			DCD	SPI0_Handler				;  16+ 25: SPI0_Handler
			DCD	0							;  16+ 26: Reserved		
			DCD	UART0_Handler				;  16+ 27: UART0_Handler					
			DCD	0							;  16+ 28: Reserved
			DCD	0							;  16+ 29: Reserved	
			DCD	0							;  16+ 30: Reserved
			DCD	0							;  16+ 31: Reserved
__Vectors_End

__Vectors_Size		EQU	__Vectors_End - __Vectors
					
			AREA	|.text|, CODE, READONLY

; Reset handler routine
Reset_Handler		PROC
			EXPORT	Reset_Handler			[WEAK]
			IMPORT	__main
			IMPORT	SystemInit
					
			LDR	R0, =SystemInit
			BLX	R0
			LDR	R0, =__main
			BX	R0
			ENDP

; Dummy Exception Handlers
NMI_Handler		PROC
			EXPORT  NMI_Handler			[WEAK]
			B	.
			ENDP

HardFault_Handler	PROC
			EXPORT	HardFault_Handler		[WEAK]
			B	.
			ENDP

SVC_Handler		PROC
			EXPORT	SVC_Handler			[WEAK]
			B	.
			ENDP

PendSV_Handler		PROC
			EXPORT	PendSV_Handler			[WEAK]
			B	.
			ENDP

SysTick_Handler		PROC
			EXPORT	SysTick_Handler			[WEAK]
			B	.
			ENDP

Default_Handler		PROC
			EXPORT	PVD_Handler			[WEAK]
			EXPORT	IFMC_Handler			[WEAK]
			EXPORT	DMA_Handler			[WEAK]
			EXPORT	EXTIA_Handler			[WEAK]
			EXPORT	EXTIB_Handler			[WEAK]
			EXPORT	CMP1_Handler			[WEAK]					
			EXPORT	ADC0_Handler			[WEAK]
			EXPORT	TIM1_Handler			[WEAK]						
			EXPORT	TIM4_Handler			[WEAK]
			EXPORT	TIM3_Handler			[WEAK]
			EXPORT	TIM2_Handler			[WEAK]
			EXPORT	ALU_Handler			[WEAK]				
			EXPORT	CMP0_Handler			[WEAK]
			EXPORT	TIM8_Handler			[WEAK]
			EXPORT	I2C0_Handler			[WEAK]
			EXPORT	SPI0_Handler			[WEAK]
			EXPORT	UART0_Handler			[WEAK]

PVD_Handler
IFMC_Handler
DMA_Handler
EXTIA_Handler
EXTIB_Handler
CMP1_Handler
ADC0_Handler
TIM1_Handler
TIM4_Handler
TIM3_Handler
TIM2_Handler
ALU_Handler
CMP0_Handler
TIM8_Handler
I2C0_Handler
SPI0_Handler
UART0_Handler

			B	.
			ENDP

			ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
			IF	:DEF:__MICROLIB

			EXPORT	__initial_sp
			EXPORT	__heap_base
			EXPORT	__heap_limit

			ELSE

			IMPORT	__use_two_region_memory
			EXPORT	__user_initial_stackheap
					
__user_initial_stackheap
			LDR	R0, = Heap_Mem
			LDR	R1, = (Stack_Mem + Stack_Size)
			LDR	R2, = (Heap_Mem + Heap_Size)
			LDR	R3, = Stack_Mem
			BX	LR

			ALIGN

			ENDIF

			END
