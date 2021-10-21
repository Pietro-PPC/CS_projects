#include "chash.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize_hash(cHash_t *t)
// Inicializa struct de cuckoo hash com valores -1 (vazio nunca preenchido).
{
    for (int i = 0; i < SIZE; ++i)
    {
        t->t1[i] = -1;
        t->t2[i] = -1;
    }
}

int h1(long int k)
// Aplica função hash da primeira tabela em k.
{
    return k % SIZE;
}

int h2(long int k)
// Aplica função hash da segunda tabela em k.
{
    return floor(SIZE * (k*0.9 - floor(k*0.9)));
}

long int *search_key(cHash_t *t, long int key)
// Procura chave 'key' e retorna endereço dela, caso exista no hash.
// Caso contrário, retorna NULL
{
    // Cálculo do endereço da chave caso ela esteja em T1.
    long int *t1_pos = t->t1 + h1(key);
    // Cálculo do endereço da chave caso ela esteja em T2.
    long int *t2_pos = t->t2 + h2(key);

    if (*t1_pos == -1)
        return NULL;
    else if (*t1_pos == key)
        return t1_pos;
    else if (*t2_pos == key)
        return t2_pos;
    else
        return NULL;
}

void insert_key(cHash_t *t, long int key)
// Insere chave 'key' na struct de cuckoo hash, ignorando chaves que já existem.
{
    // Cálculo do endereço da chave caso ela seja inserida em T1.
    long int *t1_pos = t->t1 + h1(key); 
    
    if (search_key(t, key));
    else if (*t1_pos < 0)
        *t1_pos = key;
    else
    {
        // Cálculo do endereço da chave caso ela seja inserida em T2.
        long int *t2_pos = t->t2 + h2(*t1_pos);
        
        *t2_pos = *t1_pos;
        *t1_pos = key;
    }
}

void delete_key(cHash_t *t, long int key)
// Exclui chave 'key' da estrutura hash, caso ela exista.
{
    // Cálculo do endereço da chave caso ela esteja em T1.
    long int *t1_pos = t->t1 + h1(key);
    // Cálculo do endereço da chave caso ela esteja em T2.
    long int *t2_pos = t->t2 + h2(key);

    if (*t2_pos == key)
        *t2_pos = -2;
    else if (*t1_pos == key)
        *t1_pos = -2;
}

void initialize_output(output_t *out)
// inicializar struct de output
{
    out->s = 0;
}

void insert_output_value(output_t *out, long int *t, int table, int pos)
// Inserir valor na posição pos da tabela table na struct de output.
{
    int *s = &(out->s);

    out->v[*s][0] = t[pos];
    out->v[*s][1] = table;
    out->v[*s][2] = pos;

    ++(*s);
}

void insert_output_values(output_t *out, cHash_t *t)
// Inserir todos os valores válidos da tabela hash t na struct de output
{
    for (int i = 0; i < SIZE; ++i)
    {
        if (t->t1[i] >= 0)
            insert_output_value(out, t->t1, 1, i);
        if (t->t2[i] >= 0)
            insert_output_value(out, t->t2, 2, i);
    }
}

int output_compare(const void *a, const void *b)
// Função de comparação para ordenação do vetor da struct de output.
{
    long int va = ((long int *)a)[0];
    long int vb = ((long int *)b)[0];   

    if (va > vb)
        return 1;
    else
        return -1;
}

void print_output(cHash_t *t)
// Imprime hash ordenado primariamente por chave, secundariamente pelo número da tabela
// e terciariamente pela posição na tabela. 
{
    output_t out;

    initialize_output(&out);
    insert_output_values(&out, t);
    qsort(out.v, out.s, 3*sizeof(long int), output_compare);

    for (int i = 0; i < out.s; ++i)
        printf("%ld,T%ld,%ld\n", out.v[i][0], out.v[i][1], out.v[i][2]);
}