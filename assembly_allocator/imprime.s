.section .data
    TAM_HEADER: .quad 16
.section .text
.globl imprimeMapa

imprimeMapa:
    pushq %rbp                  # Empilha informacoes da funcao
    movq %rsp, %rbp    
    movq topoInicialHeap, %r12       # Endereco de inicio da heap
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
    movq $0, (%rdi)             # Como fazer retorno?
    movq $0, %rax
    popq %rbp
    ret
