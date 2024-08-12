#pragma once
#include "Produto.h"

// Protótipos das funções do sistema de controle de estoque
void menu_acoes();
produto* expande_vetor(produto[], unsigned*, unsigned*);
produto* adiciona_produto(produto[], unsigned*, unsigned*);
void confirma_exclusao(produto[], unsigned*, unsigned);
void exclui_produto(produto[], unsigned*);
void lista_produtos(produto[], unsigned*);
void controle_estoque(produto[], unsigned*, unsigned*);

