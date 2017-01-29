 .text
	.align 4
	.globl Insertar
	.type Insertar,@function
Insertar:
        # Aqui viene vuestro codigo
        pushl %ebp
        movl %esp, %ebp			# enlace dinamico
        
        subl $12, %esp			# variables i, j, lug
        movl $0, -12(%ebp)		# i = 0
        movl $-1, -4(%ebp)		# lug = -1
        
        movl 8(%ebp), %ebx		# @v en %ebx
        movl 16(%ebp), %eax		# X.k en %eax
	imul $12, -12(%ebp), %ecx	# 12 (size struct) * i en %ecx
	movl 4(%ebx, %ecx), %edx	# v[i].k en %edx
dowhile:
        cmpl %edx, %eax			# X.k < v[i].k ?
        jge else1
        
if1:
        movl -12(%ebp), %ecx
        movl %ecx, -4(%ebp)		# lug = i
        jmp enddowhile
        
else1:
        incl -12(%ebp)			# ++i
        imul $12, -12(%ebp), %ecx	# 12 (size struct) * i en %ecx
	movl 4(%ebx, %ecx), %edx	# v[i].k en %edx
	
        cmpl $2147483648, %edx		# v[i].k != 0x80000000 ?
        jne dowhile
enddowhile:

        cmpl $2147483648, %edx		# v[i].k == 0x80000000 ?
        jne else2
        
if2:
        movl -12(%ebp), %ecx
        movl %ecx, -4(%ebp)		# lug = i
        jmp endwhile
        
else2:
while:
        imul $12, -12(%ebp), %ecx	# 12 (size struct) * i en %ecx
	movl 4(%ebx, %ecx), %edx	# v[i].k en %edx

        cmpl $2147483648, %edx		# v[i].k != 0x80000000 ?
        je endwhile
        
        incl -12(%ebp)			# ++i
        jmp while
        
endwhile:
        
        movl -12(%ebp), %eax
        movl %eax, -8(%ebp)		# j = i
        movl -4(%ebp), %edx		# lug en %edx
for:
        cmpl %edx, -8(%ebp)		# j >= lug ?
        jl endfor
        
        imul $12, -8(%ebp), %ecx	# 12 (size struct) * j en %ecx
        
	movb (%ebx, %ecx), %ah
	movb %ah, 12(%ebx, %ecx)	# v[i + 1].c = v[i].c
	
	movl 4(%ebx, %ecx), %eax
	movl %eax, 16(%ebx, %ecx)	# v[i + 1].k = v[i].k
	
	movl 8(%ebx, %ecx), %eax
	movl %eax, 20(%ebx, %ecx)	# v[i + 1].m = v[i].m
        
        decl -8(%ebp)			# --j
        jmp for
        
endwhile:
        
        pushl -4(%ebp)
        pushl 20(%ebp)
        pushl 16(%ebp)
        pushb 12(%ebp)
        pushl 8(%ebp)
        call Asignar			# crida subrutina
        addl $20, %esp
        
        movl -12(%ebp), %eax
        incl %eax			# return i + 1
        
	movl %ebp, %esp
	popl %ebp
	ret
	
