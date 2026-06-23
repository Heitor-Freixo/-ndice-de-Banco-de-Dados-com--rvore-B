#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdbool.h>

// Definição do tamanho do bloco/página alinhado com o S.O.
#define PAGE_SIZE 4096

// Ordem M calculada matematicamente para caber em 4096 bytes
#define M 200 

// Tipo para representar offsets/ponteiros de disco (posição em bytes dentro do arquivo)
typedef int64_t disk_offset_t;
#define NULL_OFFSET -1

/**
 * Estrutura que representa o Cabeçalho (Header) de uma Página.
 * Armazena os metadados do nó da Árvore B.
 */
typedef struct {
    int32_t num_keys;        // Número atual de chaves no nó (Máximo: M - 1)
    int32_t is_leaf;         // 1 se for nó folha, 0 se for nó interno (usado int para evitar problemas de padding do bool)
    int64_t reserved;        // Espaço reservado para expansões futuras ou alinhamento (8 bytes)
} PageHeader;

/**
 * Estrutura da Página/Nó da Árvore B clássica em disco.
 * Forçada a ter exatamente PAGE_SIZE (4096 bytes) através de uma região de preenchimento.
 */
typedef struct {
    PageHeader header;

    // Espaço para as chaves (Matrículas) -> Máximo de M - 1 chaves
    int32_t keys[M - 1];

    // Na Árvore B clássica, cada chave possui um registro associado.
    // Armazenamos aqui o offset de disco para onde o registro completo (tamanho variável) está salvo.
    disk_offset_t record_offsets[M - 1];

    // Ponteiros/Offsets de disco para os nós filhos -> Máximo de M filhos
    disk_offset_t child_offsets[M];

    // Preenchimento explícito (Padding) para garantir que a struct ocupe EXATAMENTE 4096 bytes em qualquer arquitetura
    uint8_t padding[PAGE_SIZE - (sizeof(PageHeader) + 
                                 ((M - 1) * sizeof(int32_t)) + 
                                 ((M - 1) * sizeof(disk_offset_t)) + 
                                 (M * sizeof(disk_offset_t)))];
} BTreePage;

#endif // STORAGE_H