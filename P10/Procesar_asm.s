.text
	.align 4
	.globl procesar
	.type	procesar, @function
procesar:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi

# Aqui has de introducir el codigo
	movl	8(%ebp), %eax		# eax = @mata
	movl	12(%ebp), %ebx		# ebx = @matb
	movl	16(%ebp), %ecx		# 
	imul	%ecx, %ecx		# ecx = n * n
	xorl	%edi, %edi		# i = 0
for:	cmpl	%ecx, %edi		# i < n * n ?
	jge	endfor
	movb	(%eax, %edi), %ah	# 
	andb	$1, %ah			# ah = mata[i] & 1
if:	cmpb	$0, %ah			# ah != 0 ?
	je	else
	movb	$255, (%ebx, %edi)	# matb[i] = 255
	jmp	endif
else:	movb	$0, (%ebx, %edi)	# matb[i] = 0
endif:	incl	%edi			# ++i
	jmp	for
endfor:	

# El final de la rutina ya esta programado

	popl	%edi
	popl	%esi
	popl	%ebx
	movl %ebp,%esp
	popl %ebp
	ret
