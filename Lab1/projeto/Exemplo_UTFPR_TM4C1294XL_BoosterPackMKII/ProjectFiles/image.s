		THUMB
			
		AREA |.text|, READONLY, CODE, ALIGN=2
			
			
		IMPORT IMAGE_WIDTH
		IMPORT IMAGE_HEIGHT
		IMPORT DISPLAY_WIDTH
		IMPORT DISPLAY_HEIGHT
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
; Algorithm:
;
; If expanding:
; 	For each row in the base image:
;		For each column in the row:
;			Get byte from base image
;			Copy the byte in the target image in img[base_row:base_row+k][base_col:base_col+k]
;														 /\       /\          /\       /\
;														 ||       ||          ||	   ||
;														Will need 2 FORs inside (row + column)
;			These will get a slot and transform in k^2:							|
;			k = 2 ->		*  ->   * *											|
;									* *											|
;																				|
; Else (if shrinking):															|
;	For each row in the base image:												|
;		For each column in the row:												|
;			Get byte from base image											|
;			For each copied row:	<-------------------------------------------/
;				For each copied column:
;					Copy the byte
;					Add k to column iterator 
;			Add k to row iterator
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
;  -----------------  <- Previous stack pointer
; |       SP        |
; |-----------------|
; |	    Padding     |
; |-----------------| <- Current stack pointer
; |  Base img addr  | -0x04
; | Target img addr | -0x08
; | 	Row max		| -0x09
; |   Column max    | -0x0A
; |  Resize factor  | -0x0B
; | Reduction flag  | -0x0C
; |   Row iterator  | -0x0D
; | Column iterator | -0x0E
; | Row copy itera  | -0x0F
; | Col copy itera  | -0x10
; |		   .        |
; |		   .        |
; |  	   .        |
;
; All information will be stored and accessed as BYTES
;
;
resizeImage
		PUSH{LR}
		
		; Creating new stack frame
		MOV R0, SP
		PUSH{R0}
		AND R0, #0xFFFFFFF0		; Padding memory
		SUB R0, #0x20			; Creating frame of 32 Bytes (extra in case needed)
		MOV SP, R0				; Updating stack pointer
		
		; Getting images addresses
		LDR R1, =images			; Loading base images array address
		LDR R0, =current_image	; Loading address of index from current image in images array
		LDRB R0, [R0]			; Loading index from current image
		ADD R1, R1, R0			; Calculating address for base image address
		LDR R4, [R1]			; Loading base image address
		
		STR R4, [SP, #-0x04]	; Storing base image address
		
		LDR R2, =image			; Loading address that will be filled with the image
		ADD R5, R2, #2			; Adding offset from 2 bytes (uint8_t width, height)
		
		STR R5, [SP, #-0x08]	; Storing target image address
		
		
		; Gets resize factor and shrinking flag
		LDR R0, =current_state
		LDRB R0, [R0]
		MOV R1, #0
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #32		; Resize factor is 32
			MOVEQ R3, #01		; Image is shrinking
		
		MOV R1, #1
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #16		; Resize factor is 16
			MOVEQ R3, #01		; Image is shrinking
		
		MOV R1, #2
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #08		; Resize factor is 08
			MOVEQ R3, #01		; Image is shrinking
		
		MOV R1, #3
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #04		; Resize factor is 04
			MOVEQ R3, #01		; Image is shrinking
		
		MOV R1, #4
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #02		; Resize factor is 02
			MOVEQ R3, #01		; Image is shrinking
			
		MOV R1, #5
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #01		; Resize factor is 01
			MOVEQ R3, #00		; Image is "expanding" (1B will be copied)

		
		MOV R1, #6
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #02		; Resize factor is 02
			MOVEQ R3, #00		; Image is expanding
		
		MOV R1, #7
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #04		; Resize factor is 04
			MOVEQ R3, #00		; Image is expanding
		
		MOV R1, #8
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #08		; Resize factor is 08
			MOVEQ R3, #00		; Image is expanding
		
		MOV R1, #9
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #16		; Resize factor is 16
			MOVEQ R3, #00		; Image is expanding
		
		MOV R1, #10
		CMP R0, R1
		ITT EQ
			MOVEQ R2, #32		; Resize factor is 32
			MOVEQ R3, #00		; Image is expanding
			
		STRB R2, [SP, #-0x0B]	; Storing resize factor
		STRB R3, [SP, #-0x0C]	; Storing shrinking flag
	

		LDR R1, =image
		LDRB R0, [R1]			; Loading height (offset 0 inside struct - row max)
		STRB R0, [SP, #-0x09]	; Storing max rows
		
		LDR R1, =image
		LDRB R0, [R1, #1]		; Loading width (column max)
		STRB R0, [SP, #-0x0A]	; Storing max columns

		; Initializing iterators in memory
		MOV R0, #0				; Initializing as 0
		STRB R0, [SP, #-0x0D]	; Initializing row iterator
		STRB R0, [SP, #-0x0E]	; Initializing col iterator
		STRB R0, [SP, #-0x0F]	; Initializing row copy iterator
		STRB R0, [SP, #-0x10]	; Initializing col copy iterator

ForRow
		; Must be done for when branching from code inside this for
		LDRB R0, [SP, #-0x0D]	; Loading row iterator
		LDRB R1, [SP, #-0x09]	; Loading max rows
		CMP R0, R1				; If finished with rows
		BGE EEnd				; Return

ForColumn
		LDRB R0, [SP, #-0x0E]	; Loading column iterator
		LDRB R1, [SP, #-0x0A]	; Loading max columns
		CMP R0, R1				; If finished with columns
		BLT ForColumnCont
ForColumnEnd
		LDRB R0, [SP, #-0x0D]	; Loading row iterator
		ADD R0, R0, #1			; Incrementing
		STRB R0, [SP, #-0x0D]	; Storing back
		B ForRow				; Go to next row
ForColumnCont
		LDRB R0, [SP, #-0x0C]	; Loading reduction flag
		CMP R0, #1				; Checks if is reducing the size
		BEQ SkipBytes
		
		MOV R0, #0				; Initializing row copy iterator
		STRB R0, [SP, #-0xF]	; Must be done every time it enters this scope

; Starting new copy cycle
CopyForRow
		LDRB R0, [SP, #-0x0F]	; Loading row copy iterator
		LDRB R1, [SP, #-0x0B]	; Loading resize factor
		CMP R0, R1				; If finished with copying the rows
		BLT CopyForRowCont

CopyForRowEnd
		LDRB R0, [SP, #-0x0E]	; Loading column iterator
		ADD R0, #1				; Incrementing it
		STRB R0, [SP, #-0x0E]	; Storing back
		B ForColumn				; Continuing with outer column loop

CopyForRowCont
		MOV R0, #0				; Initializing column copy iterator
		STRB R0, [SP, #-0x10]	; Storing to be accesses inside copy column label

; Copying row of bytes
CopyForColumn
		LDRB R0, [SP, #-0x10]	; Loading column copy iterator
		LDRB R1, [SP, #-0x0B]	; Loading resize factor
		
		CMP R0, R1				; If finished copying columns
		BLT CopyForColumnCont

CopyForColumnEnd
		LDRB R0, [SP, #-0x0F]	; Loading row copy iterator
		ADD R0, #1				; Incrementing it
		STRB R0, [SP, #-0x0F]	; Storing back
		B CopyForRow			; Continuing with next row

CopyForColumnCont
		; Computing address in target image for byte
		LDR R2, [SP, #-0x0F]		; Loading row copy iterator
		MOV R3, R0					; Moving col copy iterator
		; Resize factor is in r1
		MUL R2, R2, R1				; R2 = resize_factor * row_copy_iter
		ADD R2, R2, R3				; R2 = resize_factor * row_copy_iter + col_copy_iter
		LDR R1, [SP, #-0x08]		; Loading target image base address
		ADD R1, R1, R2				; R1 = row + col + resize_factor * row_copy_iter + col_copy_iter
		; Address of the target byte is in R1
		LDR R2, [SP, #-0x04]				; Loading base image base address - base_image[0][0]
		; We should copy image[row][column]
		; image[row*col_max + column]
		; address = base + row*col_max + column
		LDRB R3, [SP, #-0x0D]		; Loading row iterator
		LDRB R4, [SP, #-0x0E]		; Loading column iterator
		LDRB R5, [SP, #-0x0A]		; Loading col_max
		MUL R3, R3, R5				; R3 = row*col_max
		ADD R3, R3, R4				; R3 = row*col_max + column
		ADD R2, R2, R3				; R2 = base_image_address + row*col_max + column
		LDRB R0, [R2]				; Loading byte to be copied
		STRB R0, [R1]				; Storing copied byte
		
		; Incrementing col_copy_iter
		LDRB R0, [SP, #-0x10]
		ADD R0, #1
		STRB R0, [SP, #-0x10]
		B CopyForColumn
		
; @TODO: Finish skip bytes function
SkipBytes
		B EEnd
		B ForColumn














		B EEnd




;Old	
;		; Computing number of bytes that needs to be copied
;		LDR R9, =IMAGE_WIDTH
;		LDRB R9, [R9]
;		LDR R10, =IMAGE_HEIGHT
;		LDRB R10, [R10]
;		MUL R10, R10, R9
;		MUL R9, R5, R5			; Resize factor ^ 2
;		MUL R4, R10, R9			; Computing how many bytes in total will be copied
;		LDR R9, =DISPLAY_WIDTH
;		LDRB R9, [R9]
;		LDR R10, =DISPLAY_HEIGHT
;		LDRB R10, [R10]
;		MUL R9, R9, R10		; Computing how many bytes are there in the display
;		CMP R9, R4
;		IT LE
;			MOVLE R4, R9		; If less, sets as the maximum for the display
;		MOV R3, #0				; Total of bytes already copied
;		MUL R8, R5, R5			; Calculates how many bytes needs to be copied if expanding

;CopyLoop
;		CMP R3, R4				; Checking if already copied all the bytes
;		BGE EEnd				; Finisheds if so
;		MOV R9, #0
;		LDRB R0, [R1]			; Gets byte from base image
;		CMP R6, R9				; Checks if expanding
;		BNE Skip				; If not, skip k bytes
;		; Copy k bytes
;		MOV R7, #0				; Initializes number of times the byte was copied
;CopyRepeatLoop
;		CMP R7, R8				; Checks if copied k bytes
;		BEQ CopyRepeatEnd		; Finishes with the current byte and increment base image address
;		STRB R0, [R2]			; Stores byte in destination image
;		ADD R7, #1				; Increment the number of times the byte was copied
;		ADD R3, #1				; Increment the number of total copied bytes
;		ADD R1, #1				; Incrementing target image address
;		B CopyRepeatLoop
;CopyRepeatEnd
;		; Needs to be here, because we want to copy the same byte
;		; k (R5) times, and increment base address just by 1
;		ADD R2, #1				; Incrementing base image address
;		B CopyLoop
;Skip
;		STRB R0, [R2]
;		ADD R2, R2, R5			; Adding k to the address (skipping k bytes)
;		ADD R3, R3, #1			; Incrementing total of bytes copied
;		B CopyLoop

EEnd
		POP{R0}
		MOV SP, R0
		POP{LR}
		BX LR

			
		ALIGN
		END