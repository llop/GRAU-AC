.text
	.align 4
	.globl OperaVec
	.type	OperaVec, @function
OperaVec:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	movl	8(%ebp), %eax  # eax -> la dir de Vec
	movl	(%eax), %eax   # eax -> 1er elem de Vec
	movl	%eax, -4(%ebp) # res = Vec[0]
# Aqui has de introducir el codigo

	movl $1, %ecx  			# i = 1
loop:   
        cmpl 12(%ebp), %ecx  		# i < elem?
        jge endloop
	
	movl (%eax, %ecx, 4), %ebx	# %ebx = Vec[i]
	cmpl -4(%eax, %ecx, 4), %ebx	# Vec[i] = Vec[i - 1]
	jne endif
        movl %ecx, -4(%ebp)  		# res = i

endif:
        incl %ecx  			# ++i
        jmp loop
        
endloop:

# El final de la rutina ya esta programado
	movl	-4(%ebp), %eax
	popl	%edi
	popl	%esi
	popl	%ebx
	movl %ebp,%esp
	popl %ebp
	ret
