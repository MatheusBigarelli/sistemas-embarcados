		THUMB
			
		AREA |.text|, READONLY, CODE, ALIGN=2
			
			
		IMPORT IMAGE_WIDTH
		IMPORT IMAGE_HEIGTH
		IMPORT DISPLAY_WIDTH
		IMPORT DISPLAY_HEIGTH
		IMPORT images
		IMPORT image				; The current image being displayed
		IMPORT current_image		; Index of the current image being displayed
		IMPORT current_state

		EXPORT resizeImage


; Function to resize the current image acording to the image state
;
; Possible states:
;  0     1     2    3    4   5  6  7  8   9  10
; 1/32  1/16  1/8  1/4  1/2  1  2  4  8  16  32
;
; This function will take the bytes of the image in memory and copy
; them to another region with the ratio based on the states
;
; Variables:
; - R0: Current byte being copied
; - R1: Address of the current byte being copied from base image (offset in memory - iterator)
; - R2: Address of the current image that will be filled
; - R3: Number of total bytes copied
; - R4: Number of total bytes that need to be copied
; - R5: Resize factor
; - R6: Reduction flag
; - R7: Number of times that one byte (R1) was copied
; - R8: Number of times that one byte needs to be copied (k^2 - R5^2)
;
; If the number of bytes we should copy is less than the default,
; we will skip some bytes accordingly
;
resizeImage
		PUSH{LR}
		
		LDR R1, =images			; Loading base images array address
		LDR R0, =current_image	; Loading address of index from current image in images array
		LDRB R0, [R0]			; Loading index from current image
		ADD R1, R1, R0			; Calculating base image address
		LDR R1, [R1]			; Loading base image address
		LDR R2, =image			; Loading base address that will be filled with the image
		ADD R2, R2, #2			; Adding offset from 2 bytes (uint8_t width, heigth)
		
		

		; Computes the number of times one byte will be copied to the image
		LDR R0, =current_state
		LDRB R0, [R0]
		MOV R9, #0
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #32		; Resize factor is 32
			MOVEQ R6, #01		; Image is shrinking
		
		MOV R9, #1
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #16		; Resize factor is 16
			MOVEQ R6, #01		; Image is shrinking
		
		MOV R9, #2
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #08		; Resize factor is 08
			MOVEQ R6, #01		; Image is shrinking
		
		MOV R9, #3
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #04		; Resize factor is 04
			MOVEQ R6, #01		; Image is shrinking
		
		MOV R9, #4
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #02		; Resize factor is 02
			MOVEQ R6, #01		; Image is shrinking
			
		MOV R9, #5
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #01		; Resize factor is 01
			MOVEQ R6, #00		; Image is "expanding" (1B will be copied)

		
		MOV R9, #6
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #02		; Resize factor is 02
			MOVEQ R6, #00		; Image is expanding
		
		MOV R9, #7
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #04		; Resize factor is 04
			MOVEQ R6, #00		; Image is expanding
		
		MOV R9, #8
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #08		; Resize factor is 08
			MOVEQ R6, #00		; Image is expanding
		
		MOV R9, #9
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #16		; Resize factor is 16
			MOVEQ R6, #00		; Image is expanding
		
		MOV R9, #10
		CMP R0, R9
		ITT EQ
			MOVEQ R5, #32		; Resize factor is 32
			MOVEQ R6, #00		; Image is expanding
				
		
		
		; Computing number of bytes that needs to be copied
		LDR R9, =IMAGE_WIDTH
		LDRB R9, [R9]
		LDR R10, =IMAGE_HEIGTH
		LDRB R10, [R10]
		MUL R10, R10, R9
		MUL R9, R5, R5			; Resize factor ^ 2
		MUL R4, R10, R9			; Computing how many bytes in total will be copied
		LDR R9, =DISPLAY_WIDTH
		LDRB R9, [R9]
		LDR R10, =DISPLAY_HEIGTH
		LDRB R10, [R10]
		MUL R9, R9, R10		; Computing how many bytes are there in the display
		CMP R9, R4
		IT LE
			MOVLE R4, R9		; If less, sets as the maximum for the display
		MOV R3, #0				; Total of bytes already copied
		MUL R8, R5, R5			; Calculates how many bytes needs to be copied if expanding
			
CopyLoop
		CMP R3, R4				; Checking if already copied all the bytes
		BGE EEnd				; Finisheds if so
		MOV R9, #0
		CMP R6, R9				; Checks if expanding
		BNE Skip				; If not, skip k bytes
		; Copy k bytes
		MOV R7, #0				; Initializes number of times the byte was copied
CopyRepeatLoop
		CMP R7, R8				; Checks if copied k bytes
		BEQ CopyLoop			; Continue copying the next bytes
		LDRB R0, [R1]			; Gets byte from base image
		STRB R0, [R2]			; Stores byte in destination image
		ADD R7, #1				; Increment the number of times the byte was copied
		ADD R3, #1				; Increment the number of total copied bytes
		ADD R2, #1				; Incrementing base image address
		ADD R1, #1				; Incrementing target image address
		B CopyRepeatLoop
Skip
		LDRB R0, [R1]
		STRB R0, [R2]
		ADD R2, R2, R5			; Adding k to the address (skipping k bytes)
		ADD R3, R3, #1			; Incrementing total of bytes copied
		B CopyLoop

EEnd
		
		POP{LR}
		BX LR

; Function to compute the next byte that should be copied
; If image is being enlarged by a factor of 2, the byte is copied 2 times
; If image is being reduced by a factor of 2, we should skip 1 byte (copy one
; ignore one)
; And so on
;
; Input:
; - R3: Number of times the byte was already copied
; - R4: Number of times the byte should be copied
;
; Variables:
; We cannot mess with the registers being used in the other function,
; so we'll use the ones above R7
; - R8: Current state of the image (used to see which of the actions to take
;
GetNextByte
		LDR R8, =current_state
		CMP R8, #0
		IT EQ
			ADDEQ R1, R1, #32	; Skips 32 bytes and continue copying
		
		CMP R8, #1
		IT EQ
			ADDEQ R1, R1, #16	; Skips 32 bytes and continue copying
		
		CMP R8, #2
		IT EQ
			ADDEQ R1, R1, #8	; Skips 32 bytes and continue copying
		
		CMP R8, #3
		IT EQ
			ADDEQ R1, R1, #4	; Skips 32 bytes and continue copying
		
		CMP R8, #4
		IT EQ
			ADDEQ R1, R1, #2	; Skips 32 bytes and continue copying

		; Now it gets more complicated
		; If the image is getting bigger
		; we need to make sure the byte is copied more X times
		; We can use this code segment to everyone
		CMP R3, R4				; Checks if the byte was copied the right
								; ammount of times
		BNE CopyEnd				; Skips if not
		ADDEQ R1, R1, #1	; Increments the byte address
		MOV R3, #0			; Restarts the number of times the byte was copied
		

CopyEnd		
		LDREQ R0, [R1]		; Loads next byte to be copied in R0
		BX LR
			
			
		ALIGN
		END