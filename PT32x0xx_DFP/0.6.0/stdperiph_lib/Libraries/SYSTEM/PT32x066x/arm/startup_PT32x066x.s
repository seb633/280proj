;  File Name        : startup_PT32x066x.s
;  Version          : V1.00
;  Date             : 13/02/2025
;  Description      : Startup code.
;-----------------------------------------------------------------------------------------------------------*/
;/* <<< Use Configuration Wizard in Context Menu >>>                                                        */

; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs
;//   <o> Stack Size (in Bytes, must 8 byte aligned) <0-16384:8>
Stack_Size          EQU     1024

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
                    DCD  0   				    			;  16+ 1:  Not Used
                    DCD  0									;  16+ 2:  Not Used
                    DCD  IFMC_Handler						;  16+ 3:  IFMC_Handler
                    DCD  DMA_Handler           				;  16+ 4:  CAN_Handler
                    DCD  EXTIA_Handler			     		;  16+ 5:  EXTIA_Handler
                    DCD  EXTIB_Handler						;  16+ 6:  EXTIB_Handler
                    DCD  EXTIC_Handler						;  16+ 7:  EXTIC_Handler
                    DCD  EXTID_Handler						;  16+ 8:  EXTID_Handler
                    DCD  EXTIE_Handler						;  16+ 9:  EXTIE_Handler
                    DCD  0									;  16+ 10: Not Used
                    DCD  LPTIM_Handler					    ;  16+ 11: LPTIM_Handler
                    DCD  SDADC0_Handler			            ;  16+ 12: SDADC0_Handler
                    DCD  0	                				;  16+ 13: Not Used
                    DCD  TIM5_Handler		                ;  16+ 14: TIM5_Handler
                    DCD  TIM4_Handler		                ;  16+ 15: TIM4_Handler
                    DCD  TIM3_Handler                       ;  16+ 16: TIM3_Handler
                    DCD  TIM2_Handler		                ;  16+ 17: TIM2_Handler
                    DCD  0			           			   	;  16+ 18: Not Used
                    DCD  LPUART1_Handler          			;  16+ 19: LPUART1_Handler
                    DCD  LPUART0_Handler          			;  16+ 20: LPUART0_Handler
                    DCD  0			           			   	;  16+ 21: Not Used
                    DCD  0			           			   	;  16+ 22: Not Used
                    DCD  I2C0_Handler						;  16+ 23: I2C0_Handler
                    DCD  0          						;  16+ 24: Not Used
                    DCD  SPI0_Handler						;  16+ 25: SPI0_Handler
                    DCD  0									;  16+ 26: Not Used		
                    DCD  UART0_Handler						;  16+ 27: UART0_Handler					
                    DCD  UART1_Handler						;  16+ 28: UART1_Handler
                    DCD  RTC_Handler						;  16+ 29: RTC_Handler
                    DCD  TIM6_Handler		                ;  16+ 14: TIM6_Handler
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
					EXPORT  DMA_Handler                     [WEAK]
                    EXPORT  EXTIA_Handler                   [WEAK]
                    EXPORT  EXTIB_Handler                   [WEAK]
                    EXPORT  EXTIC_Handler                   [WEAK]
                    EXPORT  EXTID_Handler                   [WEAK]
                    EXPORT  EXTIE_Handler                   [WEAK]	
                    EXPORT  LPTIM_Handler                   [WEAK]							
                    EXPORT  SDADC0_Handler	               	[WEAK]
                    EXPORT  TIM5_Handler                    [WEAK]													
                    EXPORT  TIM4_Handler                    [WEAK]
                    EXPORT  TIM3_Handler                    [WEAK]
                    EXPORT  TIM2_Handler                    [WEAK]
                    EXPORT  LPUART1_Handler                 [WEAK]				
                    EXPORT  LPUART0_Handler                 [WEAK]	
                    EXPORT  I2C0_Handler                    [WEAK]
					EXPORT  SPI0_Handler                  	[WEAK]
                    EXPORT  UART0_Handler                  	[WEAK]
                    EXPORT  UART1_Handler                  	[WEAK]
                    EXPORT  RTC_Handler                  	[WEAK]
                    EXPORT  TIM6_Handler                    [WEAK]							



PVD_Handler
IFMC_Handler
DMA_Handler
EXTIA_Handler
EXTIB_Handler
EXTIC_Handler
EXTID_Handler
EXTIE_Handler
LPTIM_Handler
SDADC0_Handler
TIM5_Handler
TIM4_Handler
TIM3_Handler
TIM2_Handler
LPUART1_Handler
LPUART0_Handler
I2C0_Handler
SPI0_Handler
UART0_Handler
UART1_Handler
RTC_Handler
TIM6_Handler


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
