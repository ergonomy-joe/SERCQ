;/*----------------------------------------*/
;/* "MEURTRES EN SERIE"                    */
;/* Copyright Cobra Soft 1987              */
;/* Decompiled by ergonomy_joe in 2020     */
;/*----------------------------------------*/
dataseg	SEGMENT
EXTRN MUSIC_flag_:WORD	;0A4A
EXTRN D_0A4C_:WORD
EXTRN D_0A4E_:WORD
EXTRN D_0A52_:WORD
EXTRN D_0ACA_:BYTE
EXTRN D_0ACB_:BYTE
EXTRN D_0ACC_:WORD
EXTRN D_0ADE_:WORD
EXTRN D_0AE0_:WORD
EXTRN MUSIC_data_:WORD	;1A4E
dataseg	ENDS

codeseg	SEGMENT
	ASSUME DS:dataseg

$sound_ds	dw 0	;34D4

$sound:	;34D6
	PUSH	AX
	PUSH	BX
	;-- --
	IN	AL,61h
	OR	AL,3
	OUT	61h,AL
	;-- --
	MOV	AL,0B6h
	OUT	43h,AL
	MOV	BX,D_0A52_[SI]
	MOV	AL,BL
	OUT	42h,AL
	MOV	AL,BH
	OUT	42h,AL
	;-- --
	POP	BX
	POP	AX
	RET

$nosound:	;34F1
	PUSH	AX
	PUSH	BX
	;-- --
	IN	AL,61h
	AND	AL,NOT 3
	OUT	61h,AL
	;-- --
	POP	BX
	POP	AX
	RET

;int 1c - user timer?
INT_34FC:
	NOP
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	PUSH	DI
	PUSHF
	PUSH	DS
	;-- --
	MOV	AX,CS:$sound_ds
	MOV	DS,AX
	;-- --
	CMP	MUSIC_flag_,1
	JNZ	C_3515
	JMP	C_3524
C_3515:
	;MOV	BYTE PTR CS:INT_34FC,90h	;i.e NOP
	db 2Eh,0C6h,6
	dw OFFSET codeseg:INT_34FC
	db 90h	;i.e NOP
	;-- --
	POP	DS
	POPF	
	POP	DI
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
	IRET	
C_3524:
	MOV	CX,28h
C_3527:
	MOV	AX,D_0AE0_
	DEC	AX
	MOV	D_0AE0_,AX
	JZ	C_3535
	LOOP	C_3527
	JMP	C_35CE
C_3535:
	CALL	$nosound

	MOV	BX,D_0A4C_
	INC	D_0A4C_
	ADD	BX,MUSIC_data_
	XOR	AH,AH
	MOV	AL,[BX]
	CMP	AL,0FFh
	JNZ	C_355D
	MOV	D_0A4C_,0
	MOV	D_0AE0_,1
	CALL	MUSIC_stop_
	JMP	C_3524
C_355D:
	TEST	AL,80h
	JNZ	C_356E
	AND	AL,7Fh
	MOV	D_0ACB_,AL
	MOV	D_0AE0_,1
	JMP	C_3524
C_356E:
	TEST	AL,40h
	JNZ	C_3590
	TEST	AL,20h
	JZ	C_3579
	JMP	C_35C0
C_3579:
	AND	AL,1Fh
	XOR	BH,BH
	MOV	BL,AL
	SHL	BX,1
	MOV	AX,D_0ACC_[BX]
	MOV	D_0ADE_,AX

	MOV	D_0AE0_,1
	JMP	C_3524
C_3590:
	TEST	AL,20h
	JZ	C_359D
	MOV	D_0AE0_,1
	JMP	C_35CE
C_359D:
	AND	AL,0Fh
	JZ	C_35B0
	DEC	AL
	MOV	SI,D_0ADE_
	XOR	AH,AH
	ADD	SI,AX
	SHL	SI,1
	CALL	$sound
C_35B0:
	MOV	AL,D_0ACB_
	MUL	D_0ACA_
	ADD	AX,40
	MOV	D_0AE0_,AX
	JMP	C_35CE
C_35C0:
	AND	AL,1Fh
	MOV	D_0ACA_,AL
	MOV	D_0AE0_,1
	JMP	C_3524
C_35CE:
	;MOV	BYTE PTR CS:INT_34FC,90h	;i.e NOP
	db 2Eh,0C6h,6
	dw OFFSET codeseg:INT_34FC
	db 90h	;i.e NOP
	;-- --
	POP	DS
	POPF	
	POP	DI
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
	IRET

;setTimerRoutine
PUBLIC C_35DD_
C_35DD_:
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	PUSH	DI
	PUSHF	
	;-- --
	;MOV	BYTE PTR CS:INT_34FC,90h	;i.e NOP
	db 2Eh,0C6h,6
	dw OFFSET codeseg:INT_34FC
	db 90h	;i.e NOP
	
	PUSH	ES

	MOV	MUSIC_flag_,0
	MOV	D_0AE0_,1
	;-- --
	MOV	AX,DS
	MOV	CS:$sound_ds,AX
	;-- --
	MOV	AL,1Ch
	MOV	AH,35h	;getInterruptVector
	INT	21h
	MOV	AX,ES
	POP	ES
	MOV	D_0A4E_+2,AX
	MOV	D_0A4E_,BX
	;-- --
	PUSH	DS
	MOV	AX,CS
	MOV	DS,AX
	MOV	DX,OFFSET codeseg:INT_34FC
	MOV	AX,251Ch	;setInterruptVector 1c
	INT	21h
	POP	DS
	;-- --
	POPF	
	POP	DI
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
	RET

PUBLIC MUSIC_start_	;3623
MUSIC_start_:
	MOV	MUSIC_flag_,1
	RET

PUBLIC MUSIC_stop_	;362A
MUSIC_stop_:
	MOV	MUSIC_flag_,0
	CALL	$nosound
	MOV	D_0AE0_,1
	MOV	D_0A4C_,0

	RET

PUBLIC MUSIC_clean_	;3640
MUSIC_clean_:
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	PUSH	DI
	PUSHF
	;-- --
	CALL	MUSIC_stop_
	;-- restore int 1ch --
	PUSH	DS
	LDS	DX,DWORD PTR D_0A4E_
	MOV	AH,25h	;setInterruptVector
	MOV	AL,1Ch
	CLI
	INT	21h
	STI
	POP	DS
	;-- --
	POPF
	POP	DI
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
	RET

;fastClrScr
PUBLIC C_3660_
C_3660_:
	MOV	AX,0B800h
	MOV	ES,AX
	MOV	CX,4000h
	MOV	BX,0
C_366B:
	MOV	WORD PTR ES:[BX],0
	INC	BX
	LOOP	C_366B

	RET

codeseg	ENDS
