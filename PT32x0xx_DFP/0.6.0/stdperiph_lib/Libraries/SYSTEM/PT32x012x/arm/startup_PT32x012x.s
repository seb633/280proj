;  File Name        : startup_PT32x0xx.s
;  Version          : V1.00
;  Date             : 01/03/2023
;  Description      : Startup code.
;-----------------------------------------------------------------------------------------------------------*/
;/* <<< Use Configuration Wizard in Context Menu >>>                                                        */

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;//   <o> Stack Size (in Bytes, must 8 byte aligned) <0-16384:8>
Stack_Size          EQU     512

                    AREA    STACK, NOINIT, READWRITE, ALIGN = 3
Stack_Mem           SPACE   Stack_Size
__initial_sp

;//   <o>  Heap Size (in Bytes) <0-16384:8>
Heap_Size           EQU     0

                    AREA    HEAP, NOINIT, READWRITE, ALIGN = 3
__heap_base
Heap_Mem            SPACE   Heap_Size
__heap_limit

                    PRESERVE8
                    THUMB

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
                    AREA    RESET, CODE, READONLY
                    EXPORT  __Vectors

__Vectors           DCD  __initial_sp                       ; Top address of Stack
                    DCD  Reset_Handler                      ; Reset Handler
                    DCD  NMI_Handler                        ; NMI Handler
                    DCD  HardFault_Handler                  ; Hard Fault Handler
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved						
                    DCD  SVC_Handler            			; SVC Handler
                    DCD  0                                  ; Reserved
                    DCD  0                                  ; Reserved
                    DCD  PendSV_Handler                     ; PendSV Handler
                    DCD  SysTick_Handler                    ; SysTick Handler

                    ; External Interrupt Handler
                    DCD  PVD_Handler						;  16+ 0:  PVD_Handler
                    DCD  0 									;  16+ 1:  Not Used
                    DCD  IFMC_Handler						;  16+ 2:  IFMC_Handler
                    DCD  EXTIA_Handler						;  16+ 3:  EXTIA_Handler
                    DCD  EXTIB_Handler           			;  16+ 4:  EXTIB_Handler
                    DCD  ADC0_Handler			     		;  16+ 5:  ADC0_Handler
                    DCD  CMP0_Handler						;  16+ 6:  CMP0_Handler
                    DCD  I2C0_Handler						;  16+ 7:  I2C0_Handler
                    DCD  LPUART0_Handler					;  16+ 8:  LPUART0_Handler
                    DCD  USART0_Handler						;  16+ 9:  USART0_Handler
                    DCD  LEDPWM_Handler						;  16+ 10: LEDPWM_Handler
                    DCD  TIM4_Handler					  	;  16+ 11: TIM4_Handler
                    DCD  TIM20_Handler			            ;  16+ 12: TIM20_Handler
                    DCD  TIM21_Handler		                ;  16+ 13: TIM21_Handler
                    DCD  TIM22_Handler		                ;  16+ 14: TIM22_Handler
                    DCD  0		                			;  16+ 15: Not Used
                    DCD  0                       			;  16+ 16: Not Used
                    DCD  0		                			;  16+ 17: Not Used
                    DCD  0           			   			;  16+ 18: Not Used
                    DCD  0           						;  16+ 19: Not Used
                    DCD  0		            				;  16+ 20: Not Used
                    DCD  0									;  16+ 21: Not Used
				    DCD  0                       			;  16+ 22: Not Used
                    DCD  0									;  16+ 23: Not Used
                    DCD  0          						;  16+ 24: Not Used
                    DCD  0									;  16+ 25: Not Used
                    DCD  0									;  16+ 26: Not Used		
                    DCD  0									;  16+ 27: Not Used		
                    DCD  0									;  16+ 28: Not Used
                    DCD  0									;  16+ 29: Not Used	
                    DCD  0                       			;  16+ 30: Not Used
                    DCD  0                       			;  16+ 31: Not Used
						

; Reset handler routine
Reset_Handler       PROC
                    EXPORT  Reset_Handler                   [WEAK]
                    IMPORT  __main
                    IMPORT  SystemInit
					LDR     R0, =SystemInit
                    BLX     R0
                    LDR     R0, =__main
                    BX      R0
                    ENDP

; Dummy Exception Handlers
NMI_Handler         PROC
                    EXPORT  NMI_Handler                     [WEAK]
                    B       .
                    ENDP

HardFault_Handler   PROC
                    EXPORT  HardFault_Handler               [WEAK]
                    B       .
                    ENDP

SVC_Handler         PROC
                    EXPORT  SVC_Handler                     [WEAK]
                    B       .
                    ENDP

PendSV_Handler      PROC
                    EXPORT  PendSV_Handler                  [WEAK]
                    B       .
                    ENDP

SysTick_Handler     PROC
                    EXPORT  SysTick_Handler                 [WEAK]
                    B       .
                    ENDP


Default_Handler     PROC
                    EXPORT  PVD_Handler                    	[WEAK]	
					EXPORT  IFMC_Handler                    [WEAK]
                    EXPORT  EXTIA_Handler                   [WEAK]
                    EXPORT  EXTIB_Handler                   [WEAK]
					EXPORT  ADC0_Handler	               	[WEAK]
                    EXPORT  CMP0_Handler                   	[WEAK]	
					EXPORT  I2C0_Handler                    [WEAK]						
                    EXPORT  LPUART0_Handler	               	[WEAK]
                    EXPORT  USART0_Handler                	[WEAK]						
                    EXPORT  LEDPWM_Handler                 	[WEAK]
                    EXPORT  TIM4_Handler                    [WEAK]
                    EXPORT  TIM20_Handler                   [WEAK]
                    EXPORT  TIM21_Handler                   [WEAK]				
					EXPORT  TIM22_Handler                  	[WEAK]




PVD_Handler
IFMC_Handler
EXTIA_Handler
EXTIB_Handler
ADC0_Handler
CMP0_Handler
I2C0_Handler
LPUART0_Handler
USART0_Handler
LEDPWM_Handler
TIM4_Handler
TIM20_Handler
TIM21_Handler
TIM22_Handler




                    B       .
                    ENDP

                    ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                    IF      :DEF:__MICROLIB

                    EXPORT  __initial_sp
                    EXPORT  __heap_base
                    EXPORT  __heap_limit

                    ELSE

                    IMPORT  __use_two_region_memory
                    EXPORT  __user_initial_stackheap
__user_initial_stackheap

                    LDR     R0, =  Heap_Mem
                    LDR     R1, = (Stack_Mem + Stack_Size)
                    LDR     R2, = (Heap_Mem + Heap_Size)
                    LDR     R3, = Stack_Mem
                    BX      LR

                    ALIGN

                    ENDIF

                    END
