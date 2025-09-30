


	addi t0, zero, 3


	addi t2, zero, 1     # Start with result = 1
	add  t1, t0, zero    # Copy n to counter (t1)

loop:
    	beq  t1, zero, stop  # If counter == 0, we're done
   	add  sp, t2, zero    # Save current result

multiply:

    	addi t1, t1, -1      # Decrement counter
    	beq  t1, zero, next  # If counter == 0, go to next number
    	add  t2, t2, sp      # Add saved result to current result
    	beq  zero, zero, multiply

next:

    	addi t1, t0, -1      # Reset counter to n-1
    	add  t0, t1, zero    # Update n to n-1
    	beq  zero, zero, loop

stop:

    	beq  zero, zero, stop  # Infinite stop loop