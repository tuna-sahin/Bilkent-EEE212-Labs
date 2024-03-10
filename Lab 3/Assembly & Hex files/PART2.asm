ORG 0H
CLR P3.3; for debugging purposes
MOV 38H,#0C6H;6193
MOV 39H,#0A8H

RESET:
ACALL SETUP
MOV TCON,#0
MOV TMOD,#0
MOV TH0,#0
MOV TL0,#0
MOV R0,#30H
MOV 30H,#0
MOV 31H,#0


KEYBOARD_LOOP:
ACALL KEYBOARD
CJNE A,#23H,NVM1
AJMP BREAK
NVM1: ACALL RECORD_DATA
SJMP KEYBOARD_LOOP

RECORD_DATA:
ACALL SEND_DATA
SUBB A,#30H
MOV @R0,A
INC R0
RET

BREAK:
MOV A,#01H
ACALL SEND_COMMAND
MOV A,#80H
ACALL SEND_COMMAND

CJNE R0,#031H,IF0
MOV R6,30H
CJNE R6,#0,PRE_PROCESS
SJMP ERROR

IF0: CJNE R0,#032H,NVM2
MOV R6,30H
CJNE R6,#0,IF2_2
SJMP IF2_3
IF2_2:CJNE R6,#1,ERROR
IF2_3:MOV R6,31H
CJNE R6,#6,NVM2
NVM2: JNC ERROR
SJMP PROCESS


PRE_PROCESS:
MOV 31H,30H
MOV 30H,#0
SJMP PROCESS

ERROR:
MOV DPTR,#ERROR_MESSAGE
ERROR_LOOP:
CLR A
MOVC A,@A+DPTR
INC DPTR
JZ RESET3
ACALL SEND_DATA
SJMP ERROR_LOOP

RESET3:
AJMP RESET2

PROCESS:
MOV A,30H
MOV B,#10
MUL AB
MOV B,31H
ADD A,B
MOV B,#2
MUL AB
MOV R2,A

DISPLAY_TIME:
MOV TMOD,#00000000B
MOV TCON,#00010000B
MOV TH0,38H
MOV TL0,39H
SETB P3.3 ; for debugging purposes
OUTER_LOOP:
MOV R3,#0
LOOP_START:
CJNE R3,#0,IF1
	MOV A,R2
	MOV B,#2
	DIV AB
	MOV R6,B
	CJNE R6,#1,BLANK
	MOV 42H,#2EH
	MOV 43H,#35H
	SJMP NVM3
	BLANK:
	MOV 42H,#20H
	MOV 43H,#20H
	NVM3:
	MOV B,#10
	DIV AB
	ADD A,#30H
	MOV 40H,A
	MOV A,B
	ADD A,#30H
	MOV 41H,A
	AJMP CONTINUE
IF1:CJNE R3,#1,IF2
	MOV P1,40H
	SETB P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF2:CJNE R3,#7,IF3
	CLR P3.7
	AJMP CONTINUE
IF3:CJNE R3,#8,IF4
	MOV P1,41H
	SETB P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF4:CJNE R3,#14,IF5
	CLR P3.7
	AJMP CONTINUE
IF5:CJNE R3,#15,IF6
	MOV P1,42H
	SETB P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF6:CJNE R3,#21,IF7
	CLR P3.7
	AJMP CONTINUE
IF7:CJNE R3,#22,IF8
	MOV P1,43H
	SETB P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF8:CJNE R3,#28,IF9
	CLR P3.7
	AJMP CONTINUE
IF9:CJNE R3,#111,IF10
	MOV P1,#01H
	CLR P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF10:CJNE R3,#117,IF11
	CLR P3.7
	AJMP CONTINUE
IF11:CJNE R3,#118,IF12
	MOV P1,#80H
	CLR P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	AJMP CONTINUE
IF12:CJNE R3,#124,IF13
	CLR P3.7
	AJMP CONTINUE	
IF13:CJNE R3,#125,CONTINUE
	DEC R2
	CPL P3.4;for debugging purposes
	CJNE R2,#0,CONTINUE
	CLR P3.3;for debugging purposes
	AJMP LAUNCH
CONTINUE:
JNB TF0,$
MOV TH0,38H
MOV TL0,39H
CLR TF0
INC R3
AJMP LOOP_START

LAUNCH:
MOV DPTR,#LAUNCH_MESSAGE
LAUNCH_LOOP:
CLR A
MOVC A,@A+DPTR
INC DPTR
JZ RESET2
ACALL SEND_DATA
SJMP LAUNCH_LOOP

RESET2:
ACALL KEYBOARD
CJNE A,#2AH,RESET2
AJMP RESET

SETUP:
	MOV A,#38H	;TWO LINES, 5X7 MATRIX
	ACALL SEND_COMMAND
	MOV A,#0FH	;DISPLAY ON, CURSOR BLINKING
	ACALL SEND_COMMAND
	MOV A,#06H	;INCREMENT CURSOR (SHIFT CURSOR TO RIGHT)
	ACALL SEND_COMMAND
	MOV A,#01H	;CLEAR DISPLAY SCREEN
	ACALL SEND_COMMAND
	MOV A,#80H	;FORCE CURSOR TO BEGINNING OF THE FIRST LINE
	ACALL SEND_COMMAND
	RET

SEND_COMMAND:
	MOV P1,A
	CLR P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	ACALL DELAY
	CLR P3.7
	RET
SEND_DATA:
	MOV P1,A
	SETB P3.5		;RS=0 BEFORE SENDING COMMAND
	CLR P3.6		;R/W=0 TO WRITE
	SETB P3.7	;SEND A HIGH TO LOW SIGNAL TO ENABLE PIN
	ACALL DELAY
	CLR P3.7
	RET

DELAY:	
	PUSH 0
	PUSH 1
	MOV R7,#50
DELAY_OUTER_LOOP:
	MOV R6,#255
	DJNZ R6,$
	DJNZ R7,DELAY_OUTER_LOOP
	POP 1
	POP 0
	RET



KEYBOARD: ; takes the key pressed from the keyboard and puts it to A
	mov	P0, #0ffh	;makes P0 input
K1:
	mov	P2, #0	;ground all rows
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, K1
K2:
	acall	DELAY
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, KB_OVER
	sjmp	K2
KB_OVER:
	acall DELAY
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, KB_OVER1
	sjmp	K2
KB_OVER1:
	mov	P2, #11111110B
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, ROW_0
	mov	P2, #11111101B
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, ROW_1
	mov	P2, #11111011B
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, ROW_2
	mov	P2, #11110111B
	mov	A, P0
	anl	A, #00001111B
	cjne	A, #00001111B, ROW_3
	ljmp	K2
	
ROW_0:
	mov	DPTR, #KCODE0
	sjmp	KB_FIND
ROW_1:
	mov	DPTR, #KCODE1
	sjmp	KB_FIND
ROW_2:
	mov	DPTR, #KCODE2
	sjmp	KB_FIND
ROW_3:
	mov	DPTR, #KCODE3
KB_FIND:
	rrc	A
	jnc	KB_MATCH
	inc	DPTR
	sjmp	KB_FIND
KB_MATCH:
	clr	A
	movc	A, @A+DPTR; get ASCII code from the table 
	ret

;ASCII look-up table 
KCODE0:	DB	'1', '2', '3', '#'
KCODE1:	DB	'4', '5', '6', '#'
KCODE2:	DB	'7', '8', '9', '#'
KCODE3:	DB	'*', '0', '#', '#'

ERROR_MESSAGE: DB 'ERROR',0
LAUNCH_MESSAGE: DB 'LAUNCH!',0

ENDD: 
SJMP $
END