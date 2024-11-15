;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

; Add 27 + 30 into R11
			mov.w	#27, R10				; Load 27 into R10
			add.w	#30, R10				; Add 30 into R10
			mov.w	R10, R11				; Store 27 + 30 into R11

; Subtract 27 - 30 into R12
			mov.w	#27, R10				; Load 27 into R10
			sub.w	#30, R10				; Subtract 30 from R10
			mov.w	R10, R12				; Store 27 - 30 into R12

; Series (2k + 1) from k = 0 to k = 18 into R13
			mov.w	#0, R13
			mov.w	#0, R14
			mov.w	#19, R15

Loop:		cmp.w	R15, R14
			jge		LoopEnd
			mov.w	R14, R10
			rla.w	R10
			add.w	#1, R10
			add.w	R10, R13
			inc.w	R14
			jmp		Loop

LoopEnd:

; Toggle LED1 on Breakout Board
			mov.w	#__STACK_END, SP
			mov.w	#WDTPW|WDTHOLD, &WDTCTL
			bic.b	#LOCKLPM5, &PM5CTL0
			bis.b	#0x04, &P1DIR
			bis.b	#0x04, &P1OUT
_loop:
			xor.b	#0x04, &P1OUT
			mov.w 	#0xF400, R10
_lp1:
			dec.w	R10
			cmp.w	#0x00, R10
			jne		_lp1
			jmp		_loop
			nop
                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
