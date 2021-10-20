.section .data
    NEWLINE: .string "\n"
    .equ INCREMENT, 4096
    .equ TAM_HEADER, 16

.section .text
.globl _start
.globl iniciaAlocador
.globl finalizaAlocador
.globl alocaMem
.globl liberaMem
.globl imprimeMapa

iniciaAlocador:
    pushq %rbp
    movq %rsp, %rbp

    # printf ("\n")
    movq $NEWLINE, %rdi
    call printf

    # topoInicialHeap = sbrk(0) 
    movq $0, %rdi
    movq $12, %rax
    syscall
    movq %rax, topoInicialHeap
    
    # topoBlocos = sbrk(0)
    movq %rax, topoBlocos

    popq %rbp
    ret

finalizaAlocador:
    pushq %rbp
    movq %rsp, %rbp

    # brk(topoInicialHeap)
    movq topoInicialHeap, %rdi
    movq $12, %rax
    syscall

    popq %rbp
    ret

alocaMem:
    pushq %rbp
    movq %rsp, %rbp
    subq $88, %rsp

    # VARIÁVEIS:
    #   -8(%rbp)  long int bestFit;
    #   -16(%rbp) long int *cabecalho;
    #   -24(%rbp) void *iterator;
    #   -32(%rbp) void *comecoBloco;
    #   -40(%rbp) long int *isDisp;
    #   -48(%rbp) long int disp;
    #   -56(%rbp) long int mult;
    #   -64(%rbp) long int excesso;
    #   -72(%rbp) long int *bloco;
    #   -80(%rbp) long int *brk
    #   -88(%rbp) long int num_bytes

    // salva parâmetro na stack
    movq %rdi, -88(%rbp)

    // if (num_bytes <= 0) return NULL
    cmpq $0, %rdi
    jge fimIf1

    movq $0, %rax
    popq %rbp
    ret
    fimIf1:
    movq $0, -8(%rbp)
    movq topoInicialHeap, %r8
    movq %r8, -24(%rbp)
    movq topoBlocos, %r8
    movq %r8, -32(%rbp)

    // while (iterator < topoBlocos)
    while:
    movq -24(%rbp), %r8
    movq topoBlocos, %r9
    cmpq %r9, %r8
    jge fimwhile

    // cabecalho = iterator
    movq %r8, -16(%rbp)
    
    // if !cabecalho[0]
    movq (%r8), %r8
    movq $0, %r9
    cmpq %r9, %r8
    jne fimIf

    // if cabecalho[1] >= num_bytes
    movq -16(%rbp), %r8
    movq 8(%r8), %r8
    movq -88(%rbp), %rdi
    cmpq %rdi, %r8
    jl fimIf

    // if ((cabecalho[1] < bestFit) || !bestFit)
    movq -8(%rbp), %r9
    cmpq %r9, %r8
    jl dentroIf
    
    movq $0, %r10
    cmpq %r9, %r10
    jne fimIf

    dentroIf:
    // bestFit = cabecalho[1]
    movq %r8, -8(%rbp)

    // comecoBloco = iterator
    movq -24(%rbp), %r9
    movq %r9, -32(%rbp)

    fimIf:
    // iterator += TAM_HEADER + cabecalho[1]
    movq -16(%rbp), %r8
    movq 8(%r8), %r8
    addq $TAM_HEADER, %r8
    addq %r8, -24(%rbp)

    jmp while
    fimwhile:

    // if (bestfit)
    movq -8(%rbp), %r8
    movq $0, %r9
    cmpq %r9, %r8
    je fimIf2

    // isDisp = comecobloco
    movq -32(%rbp), %r8
    // *isDisp = 1
    movq $1, (%r8)

    // return comecoBloco + TAM_HEADER
    addq $TAM_HEADER, %r8
    movq %r8, %rax

    addq $88, %rsp
    popq %rbp
    ret

    fimIf2:

    // sbrk(0)
    movq $0, %rdi
    movq $12, %rax
    syscall
    movq %rax, -80(%rbp)

    // disp = sbrk(0) - topoBlocos
    movq topoBlocos, %r8
    subq %rax, %r8
    movq %r8, -48(%rbp)

    // if (TAM_HEADER + num_bytes > disp)
    movq $TAM_HEADER, %r8
    movq -88(%rbp), %rdi
    addq %rdi, %r8
    movq -48(%rbp), %r9
    cmpq %r9, %r8
    jle endif3

    // excesso = TAM_HEADER + num_bytes - disp
    subq %r9, %r8
    movq %r8, -64(%rbp)
    // mult = 1 + ((excesso - 1)/INCREMENT)
    subq $1, %r8
    movq %r8, %rax
    xor %rdx, %rdx
    movq $INCREMENT, %r9
    idiv %r9
    addq $1, %rax
    imul $INCREMENT, %rax

    // sbrk(INCREMENT * mult)
    addq -80(%rbp), %rax
    movq %rax, %rdi
    movq $12, %rax
    syscall

    endif3:
    
    # bloco = topoBlocos
    movq topoBlocos, %r8
    movq %r8, -72(%rbp)
    # bloco[0] = 1
    movq $1, (%r8)
    # bloco[1] = num_bytes
    movq -88(%rbp), %rdi
    movq %rdi, 8(%r8)
    # topoBlocos += TAM_HEADER + num_bytes
    movq topoBlocos, %r9
    addq $TAM_HEADER, %r9
    addq %rdi, %r9
    movq %r9, topoBlocos

    # return bloco + HEADER_SIZE
    movq -72(%rbp), %rax
    addq $TAM_HEADER, %rax
    
    addq $88, %rsp
    popq %rbp
    ret

imprimeMapa:
    pushq %rbp                    # Empilha informacoes da funcao
    movq %rsp, %rbp    
    movq topoInicialHeap, %r12    # Endereco de inicio da heap
    movq topoBlocos, %r15         # Endereco de topo da heap

    initWhileImprime:
    cmpq %r15, %r12
    jge fimWhileImprime
    call imprimeHeader

    movq (%r12), %r10           # %r10 = Conteudo de %r12 (flag ocupado)
    movq 8(%r12), %r11          # %r11 = Conteudo de %r12 + 8 bytes (tamanho do bloco)
    cmpq $1, %r10
    je imprimeOcupado

    movq %r11, %rdi             # %rdi e o primeiro parametro da funcao seguinte
    call imprimeBlocoLivre      # Imprime bytes do bloco livre
    jmp incrementaAtual

    imprimeOcupado:
    movq %r11, %rdi             # %rdi e o primeiro parametro da funcao seguinte
    call imprimeBlocoOcupado    # Imprime bytes do bloco ocupado

    incrementaAtual:
    addq 8(%r12), %r12          # Aumenta o endereco apontado pelo inicio da heap
    addq $TAM_HEADER, %r12
    jmp initWhileImprime

    fimWhileImprime:
    movq $10, %rdi              # 10 igual a \n
    call putchar
    popq %rbp                   # return
    ret

imprimeBlocoOcupado:
    pushq %rbp
    movq %rsp, %rbp
    movq %rdi, %rbx             # %rbx = Tamanho do bloco (primeiro parametro passado)
    movq $0, %r13               # Inicializa contador

    initWhileOcupado:
    cmpq %rbx, %r13
    jge fimWhileOcupado
    movq $43, %rdi              # 43 igual a +
    call putchar
    addq $1, %r13
    jmp initWhileOcupado
    
    fimWhileOcupado:
    popq %rbp                   # return
    ret

imprimeBlocoLivre:
    pushq %rbp
    movq %rsp, %rbp
    movq %rdi, %rbx             # %rbx = Tamanho do bloco (primeiro parametro passado)
    movq $0, %r13               # Inicializa contador

    initWhileLivre:
    cmpq %rbx, %r13
    jge fimWhileLivre
    movq $45, %rdi              # 45 igual a -
    call putchar
    addq $1, %r13
    jmp initWhileLivre
    
    fimWhileLivre:
    popq %rbp                   # return
    ret

imprimeHeader:
    pushq %rbp
    movq %rsp, %rbp
    movq $TAM_HEADER, %rbx
    movq $0, %r13

    initWhileHeader:
    cmpq %rbx, %r13
    jge fimWhileHeader
    movq $35, %rdi
    call putchar
    addq $1, %r13
    jmp initWhileHeader

    fimWhileHeader:
    popq %rbp
    ret

liberaMem:
    pushq %rbp
    movq %rsp, %rbp
    movq $0, -16(%rdi)             # Como fazer retorno?
    movq $0, %rax
    popq %rbp
    ret
