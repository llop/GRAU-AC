 .text
	.align 4
	.globl Asignar
	.type Asignar,@function
Asignar:
        # Aqui viene vuestro codigo
        pushl %ebp
        movl %esp, %ebp			# enlace dinamico
        
        movl 8(%ebp), %edx		# @v en %edx
        imul $12, 24(%ebp), %ecx	# 12 (struct size) * pos en %ecx
        
	movb 12(%ebp), %ah
	movb %ah, (%edx, %ecx)		# v[pos].c = X.c
	movl 16(%ebp), %eax
	movl %eax, 4(%edx, %ecx)	# v[pos].k = X.k
	movl 20(%ebp), %eax
	movl %eax, 8(%edx, %ecx)	# v[pos].m = X.m
	
	movl %ebp, %esp
	popl %ebp
	ret
	