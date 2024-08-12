#pragma once
#include "Produto.h"

// Protótipos das funções do sistema de vendas
produto* expande_vetor(produto[], unsigned*);
produto* adiciona_produto(produto[], unsigned*, produto*, bool);
produto* pedido(produto[], unsigned, produto[], unsigned*);
void menu_produtos(produto[], unsigned, produto[], unsigned);
void sistema_vendas(produto[], unsigned*);