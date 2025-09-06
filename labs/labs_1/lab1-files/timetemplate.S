# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
.macro	PUSH (%reg)
	addi	sp,sp,-4
	sw	%reg,0(sp) 
.end_macro

.macro	POP (%reg)
	lw	%reg,0(sp)
	addi	sp,sp,4
.end_macro
###################
# Macros for DTEK #
###################
#.macro	PUSH reg
#	addi sp,sp,-4
#	sw \reg,0(sp) 
#.endm

#.macro	POP reg
#	lw	\reg,0(sp)
#	addi	sp,sp,4
#.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################



delay: 

	PUSH (ra) 			#för att kunna gå tillbaka 
	PUSH (a0)			#för att behålla orignal time gap 
	
		
while: 	
	slti 	t0, a0, 1  		# while (a0!<1)  så  while( ms > 0 )
	bne 	t0, zero, while_done 	# break if a0 < 1
	
	addi 	a0, a0, -1 		# decrease: ms = ms – 1;
	
	li 	t1, 30			# smaller than int
	li	t2, 0  			# i
	
	j  	for
	
 		
	


for: 	bge t2, t1, for_done 	# i >= 10?

 	addi t2, t2, 1 		# i = i + 1

 	j for 			# repeat loop

for_done:
	j while

while_done:

	POP (a0)
	POP (ra)

	jr ra


time2string:

	

    	#PUSH(t0)		blir caleld från timetemplate där t0 inte är används därför behövs de int sparas
    	#PUSH(t1)
    	#PUSH(t2)
    	#PUSH(t3)
	PUSH(ra)		# sparas för att kunna gå tillbacka till timetemplate
	PUSH(a0)		# sparar för att den va initerads som timstr
	
	# M1M2 : S1S2

	#push the 4 bit that matters into the 4 lsb possition 
	
	srli	t0, a1, 12	#M1  	
	srli	t1, a1, 8	#M2
	srli	t2, a1, 4 	#S1
 	
 	#take only the 4 bits that mattters
 	 
	andi	t0, t0, 0x000f		#M1
	andi	t1, t1, 0x000f		#M2
	andi	t2, t2, 0x000f		#S1
	andi	t3, a1, 0x000f		#S2
	
	#converst each 4 bit into asci 
	
	addi 	a0, t0, 0  		#M1	
	jal 	hex2asc
	addi	t0, a0, 0
	
	addi 	a0, t1, 0 		#M2
	jal 	hex2asc
	addi	t1, a0, 0
	
	addi 	a0, t2, 0 		#S1
	jal 	hex2asc
	addi	t2, a0, 0
	
	addi 	a0, t3, 0 		#S2
	jal 	hex2asc
	addi	t3, a0, 0
	
	# creates a array like structore for M1M2 : S1S2 /0
	
	POP(a0)   			# gets back the adress 
	
	sb 	t0, 0(a0)       	# M1 
	sb 	t1, 1(a0)       	# M2 

	li 	t4, 0x3A         	# create :
	sb 	t4, 2(a0)        	# store :

	sb 	t2, 3(a0)        	# S1 
	sb 	t3, 4(a0)        	# S2 
	
	sb 	zero, 5(a0)      	# null terminator
	
	#POP(t3)
    	#POP(t2)
    	#POP(t1)
	#POP(t0)

    	POP(ra)
	
	jr ra

hex2asc:
		
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra
hexasc:
  	andi a0, a0, 0xF     # restettar allt förutom sista 4 LSB till 0     
  
	addi	a2, zero, 0x37  
  	addi	a1, zero, 0x30
  	addi	a4, zero, 0
  	
  	slti	a3, a0, 10      # if a0 < 10
  	
  	bne	a3, a4, else     # if a0 < 10 add 0x30 if a0 > 10 add 0x37
  	add	a0, a0, a2
  	jr	ra
  	
  	else:
  	add	a0, a0, a1
  	jr	ra	

