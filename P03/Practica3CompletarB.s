.text
	.align 4
	.globl OperaMat
	.type	OperaMat, @function
OperaMat:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
# Aqui has de introducir el codigo

	movl $0, -4(%ebp)  		# res = 0
        xorl %ecx, %ecx			# i = 0

iloop:   
        cmpl $3, %ecx  			# i < 3?
        jge endiloop
        xorl %edx, %edx			# j = 0

jloop:
	cmpl %ecx, %edx			# j <= i
	jg endjloop
	imul $3, %ecx, %eax		# %eax = 3 * i
	addl %ecx, %eax			# %eax += j
	movl 8(%ebp, %eax, 4), %eax	# %eax = Matriz[i][j]
	addl %eax, -4(%ebp)		# res += Matriz[i][j]
        incl %edx  			# ++j
        jmp jloop
        
endjloop:
        incl %ecx  			# ++i
        jmp iloop
        
endiloop:


# El final de la rutina ya esta programado
	movl	-4(%ebp), %eax
	popl	%edi
	popl	%esi
	popl	%ebx	
	movl %ebp,%esp
	popl %ebp
	ret
