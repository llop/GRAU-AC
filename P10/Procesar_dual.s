.text
	.align 4
	.globl procesar
	.type	procesar, @function
	.data
	.align 16
ones: 	.byte 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
zeros: 	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

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
	
	movl	%eax, %edx		# comprobar si mata o matb estan alineadas a 16
	andl	$15, %edx		# si lo estan: tirar de la version movdqa
	cmpl	$0, %edx		# si no: utilizar movdqu
	jne	forb
	movl	%ebx, %edx
	andl	$15, %edx
	cmpl	$0, %edx
	jne	forb

fora:	cmpl	%ecx, %edi		# i < n * n ?
	jge	endfor
	movdqa	(%eax, %edi), %xmm1		# 
	pand	(ones), %xmm1	# xmm1 = mata[i] & 1, mata[i + 1] & 1, ..., mata[i + 15] & 1
	pcmpgtb	(zeros), %xmm1
	movdqa	%xmm1, (%ebx, %edi)
	addl	$16, %edi
	jmp	fora

forb:	cmpl	%ecx, %edi		# i < n * n ?
	jge	endfor
	movdqu	(ones), %xmm1		# 
	pand	(%eax, %edi), %xmm1	# xmm1 = mata[i] & 1, mata[i + 1] & 1, ..., mata[i + 15] & 1
	pcmpgtb	(zeros), %xmm1
	movdqu	%xmm1, (%ebx, %edi)
	addl	$16, %edi
	jmp	forb

endfor:

# El final de la rutina ya esta programado

	emms	# Instruccion necesaria si os equivocais y usais MMX
	popl	%edi
	popl	%esi
	popl	%ebx
	movl %ebp,%esp
	popl %ebp
	ret
