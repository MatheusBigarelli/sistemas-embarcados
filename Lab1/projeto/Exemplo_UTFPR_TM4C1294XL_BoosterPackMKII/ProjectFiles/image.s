		THUMB
			
BASE_IMG_OFFSET		EQU		-0x0C
TGT_IMG_OFFSET		EQU		-0x10

H1_OFFSET			EQU		-0x14
W1_OFFSET			EQU		-0x18
H2_OFFSET			EQU		-0x1C
W2_OFFSET			EQU		-0x20
	
X_RATIO_OFFSET		EQU		-0x24
Y_RATIO_OFFSET		EQU		-0x28

ROW_ITER_OFFSET		EQU		-0x2C
COL_ITER_OFFSET		EQU		-0x30


		AREA |.text|, READONLY, CODE, ALIGN=2
			
		IMPORT images
		IMPORT image				; The current image being displayed
		IMPORT current_image		; Index of the current image being displayed

		EXPORT resizeImageASM


; Function to resize the current image acording to the image state
;
;
; This function will take the bytes of the image in memory and copy
; them to another region with the ratio based on the states
;
;
; Registers:
; * These will be kept constant
; R4 - Address from base image
; R5 - Address from target image
;
; Will leave R0 free to do calculations and store values
; for later insertion in memory
;
; The rest of the registers will be used for intermediary calculations
;
; Memory structure:
; Some values are there for convenience
; If it works
; @TODO: optimize memory - alocate registers for iterators and often accessed values
; 	and remove them from stack
;
; |-----------------| <- Current stack pointer
; |  Prev stack ptr | -0x04
; |  Return address | -0x08
; |  Base img addr  | -0x0C
; | Target img addr | -0x10
; | 	  h1		| -0x14
; | 	  w1		| -0x18
; | 	  h2		| -0x1C
; | 	  w2		| -0x20
; |     X ratio     | -0x24
; |     Y ratio     | -0x28
; |   Row iterator  | -0x2C
; | Column iterator | -0x30
; |		   .        |
; |		   .        |
; |  	   .        |
;
; All information will be stored and accessed as WORDS (32 Bits)
;
;
resizeImageASM
		; Creating new stack frame
		
		MOV R5, SP
		MOV R6, SP
		AND R5, #0xFFFFFFF0					; Padding memory
		SUB R5, #0x40						; Creating frame of 64 Bytes (extra in case needed)
		MOV SP, R5							; Updating stack pointer
		PUSH{R6}
		PUSH{R4}
		
		; Storing parameters
		STR R0, [SP, #H1_OFFSET]			; Storing h1
		STR R1, [SP, #W1_OFFSET]			; Storing w1
		STR R2, [SP, #H2_OFFSET]			; Storing h2
		STR R3, [SP, #W2_OFFSET]			; Storing w2
		
		; Getting images addresses
		LDR R1, =images						; Loading base images array address
		LDR R0, =current_image				; Loading address of index from current image in images array
		LDR R0, [R0]						; Loading index from current image
		MOV R2, #4
		MUL R0, R0, R2						; Multiplying image index to match bytes
		ADD R1, R1, R0						; Calculating address for base image address
		LDR R4, [R1]						; Loading base image address
		
		STR R4, [SP, #BASE_IMG_OFFSET]		; Storing base image address
		
		LDR R2, =image						; Loading address that will be filled with the image
		ADD R5, R2, #8						; Adding offset of 8 bytes (int width, height)
		
		STR R5, [SP, #TGT_IMG_OFFSET]		; Storing target image address
		

		; Calculates x and y ratios
		LDR R0, [SP, #W1_OFFSET]			; Loading previous width
		LDR R1, [SP, #W2_OFFSET]			; Loading next width
		LSL R0, #16							; R0 = w1 << 16
		UDIV R0, R0, R1						; R0 = (w1 << 16) / w2
		ADD R0, #1							; R0 = (w1 << 16) / w2 + 1
		STR R0, [SP, #X_RATIO_OFFSET]		; Storing x_ratio
		
		LDR R0, [SP, #H1_OFFSET]			; Loading previous width
		LDR R1, [SP, #H2_OFFSET]			; Loading next width
		LSL R0, #16							; R0 = w1 << 16
		UDIV R0, R0, R1						; R0 = (w1 << 16) / w2
		ADD R0, #1							; R0 = (w1 << 16) / w2 + 1
		STR R0, [SP, #Y_RATIO_OFFSET]		; Storing x_ratio
		

		; Initializing iterators in memory
		MOV R0, #0							; Initializing as 0
		STR R0, [SP, #ROW_ITER_OFFSET]		; Initializing row iterator
		STR R0, [SP, #COL_ITER_OFFSET]		; Initializing col iterator

ForRow
		; Must be done for when branching from code inside this for
		LDR R0, [SP, #ROW_ITER_OFFSET]		; Loading row iterator
		LDR R1, [SP, #H2_OFFSET]			; Loading max rows
		CMP R0, R1							; If not finished
		BLT ForRowCont						; Continue with row
ForRowEnd
		B EEnd
ForRowCont
		MOV R0, #0							; Restarting col iterator
		STR R0, [SP, #COL_ITER_OFFSET]		; Storing

ForColumn
		LDR R0, [SP, #COL_ITER_OFFSET]		; Loading column iterator
		LDR R1, [SP, #W2_OFFSET]			; Loading max columns
		CMP R0, R1							; If not finished with columns
		BLT ForColumnCont					; Continue
ForColumnEnd
		LDR R0, [SP, #ROW_ITER_OFFSET]		; Loading row iterator
		ADD R0, R0, #1						; Incrementing
		STR R0, [SP, #ROW_ITER_OFFSET]		; Storing back
		B ForRow							; Go to next row
ForColumnCont
		LDR R0, [SP, #COL_ITER_OFFSET]		; Loading j
		LDR R1, [SP, #X_RATIO_OFFSET]		; Loading x_ratio
		MUL R0, R0, R1						; R0 = j * x_ratio
		LSR R0, #16							; R0 = (j * x_ratio) >> 16
		MOV R2, R0							; R2 = x2
		
		; Calculating base image offset to byte
		LDR R0, [SP, #ROW_ITER_OFFSET]		; Loading i
		LDR R1, [SP, #Y_RATIO_OFFSET]		; Loading y_ratio
		MUL R0, R0, R1						; R0 = i * y_ratio
		LSR R0, #16							; R0 = (i * y_ratio) >> 16
		MOV R3, R0							; R3 = y2

		; Loading byte from base image
		LDR R0, [SP, #BASE_IMG_OFFSET]		; Loading base image base address
		LDR R1, [SP, #W1_OFFSET]			; Loading w1
		MUL R1, R1, R3						; R1 = y2 * w1
		ADD R1, R1, R2						; R1 = y2 * w1 + x2
		ADD R0, R0, R1						; R0 = base_img_address + (y2*w1) + x2
		LDRB R6, [R0]						; R6 = images[current_image][(y2*w1)+x2]
		
		; Calculating target image offset to byte
		LDR R0, [SP, #ROW_ITER_OFFSET]		; Loading i
		LDR R1, [SP, #W2_OFFSET]			; Loading w2
		MUL R0, R0, R1						; R0 = i * w2
		LDR R1, [SP, #COL_ITER_OFFSET]		; Loading j
		ADD R0, R0, R1						; R0 = (i*w2) + j
		LDR R1, [SP, #TGT_IMG_OFFSET]		; R1 = tgt_img_offset
		ADD R0, R0, R1						; R0 = tgt_img_offset + (i*w2) + j
		
		; Storing byte in target image
		STRB R6, [R0]						;image.data[(i*w2)+j] = images[current_image][(y2*w1)+x2]
		
		; Continuing with loop
		LDR R0, [SP, #COL_ITER_OFFSET]		; Loading j
		ADD R0, #1							; Incrementing
		STR R0, [SP, #COL_ITER_OFFSET]		; Storing back
		B ForColumn
	
EEnd

		POP{R4}
		POP{R0}
		MOV SP, R0
		
		
		BX LR

			
		ALIGN
		END