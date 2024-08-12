#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include "Produto.h"
#include "Estoque.h"
#include "Vendas.h"
using namespace std;

int main(int argc, char** argv)
{
	produto* vetor_estoque; // Ponteiro do vetor de estoque
	unsigned quantidade_produtos_estoque; // Representa a quantidade de produtos diferentes no estoque
	unsigned tamanho_vetor_estoque; // Representa o tamanho do vetor de estoque

	ifstream fin;
	fin.open("estoque.bin", ios_base::in | ios_base::binary);

	if (fin.is_open()) // Verifica se houve abertura de arquivo
	{
		// Leitura do arquivo de estoque

		// Lê o primeiro número o arquivo, que representa a quantidade de produtos diferentes e o tamanho que o vetor precisa ter
		fin.read((char*)&quantidade_produtos_estoque, sizeof(unsigned));
		tamanho_vetor_estoque = quantidade_produtos_estoque; 
		vetor_estoque = new produto[quantidade_produtos_estoque]; // Realiza alocação dinâmica de memória com o vetor de estoque
		fin.read((char*)vetor_estoque, sizeof(produto) * quantidade_produtos_estoque); // Lê os produtos do arquivo e os passa para o vetor de estoque

		fin.close(); // Fechamento do arquivo
	}
	else // Caso o arquivo ainda não exista
	{
		vetor_estoque = nullptr; // Inicialização do ponteiro do vetor de estoque como "nulo"
		quantidade_produtos_estoque = tamanho_vetor_estoque = 0; // Quantidade de produtos e tamanho do vetor zerados
	}

	cout << fixed; cout.precision(2); // Configura o cout para exibir pontos flutuantes como 2 casas decimais

	if (argc > 1 && !strcmp(argv[1], "-c"))
		controle_estoque(vetor_estoque, &quantidade_produtos_estoque, &tamanho_vetor_estoque); // Caso o programa seja iniciado com o argumento "-c", o sistema de controle de estoque é acionado por esta chamada de função
	else
		sistema_vendas(vetor_estoque, &quantidade_produtos_estoque); // Caso contrário ao anterior, o sistema de vendas é acionado por esta chamada de função

	/*	
		OBS:
		A liberação da memória alocada com o vetor de estoque ocorre ou na função controle_estoque() 
	    ou na função sistema_vendas(), dependendo de qual dos dois sistemas for acessado 
	    pelo usuário. 
	*/

	return 0;
}