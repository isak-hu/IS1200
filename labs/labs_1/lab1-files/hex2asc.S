  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc
	.global hexasc

hex2asc:
	li	a0, 17		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  hexasc:
  	andi a0, a0, 0xF     # restettar allt förutom sista 4 LSB till 0     
  
	addi a2, zero, 0x37  
  	addi a1, zero, 0x30
  	addi a4, zero, 0
  	
  	slti a3, a0, 10      # if a0 < 10
  	
  	bne a3, a4, else     # if a0 < 10 add 0x30 if a0 > 10 add 0x37
  	add a0, a0, a2
  	jr ra
  	
  	else:
  		add a0, a0, a1
  		jr ra
