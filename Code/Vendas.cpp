#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include "Vendas.h"
using namespace std;

produto* expande_vetor(produto vetor_produtos[], unsigned* quantidade_produtos)
{
	produto* auxiliar = vetor_produtos; // Cria um ponteiro auxiliar que aponta para o mesmo endere�o que o vetor original

	(*quantidade_produtos)++; // incrementa 1 na vari�vel que representa a quantidade de elementos do vetor 
	vetor_produtos = new produto[*quantidade_produtos]; // Aloca mem�ria com 1 espa�o a mais em um novo endere�o

	// Copia os produtos do vetor auxiliar para o novo vetor
	for (int i = 0; i < *quantidade_produtos - 1; i++)
		vetor_produtos[i] = auxiliar[i];

	delete[] auxiliar; // Libera a mem�ria apontada pelo ponteiro auxiliar;

	return vetor_produtos; // retorna o novo vetor com 1 espa�o a mais
}

produto* adiciona_produto(produto vetor_produtos[], unsigned* quantidade_produtos, produto* item, bool produto_existente)
{
	if (produto_existente) // Caso o produto j� exista no vetor
	{
		for (int i = 0; i < *quantidade_produtos; i++) // Percorre o vetor
			if (!strcmp(item->nome, vetor_produtos[i].nome)) // Identifica o produto j� existente no vetor
				vetor_produtos[i].quantidade += (item->quantidade); // Aumenta a quantidade de unidades do produto existente na cesta de acordo com a escolha do usu�rio
	}
	else
	{
		vetor_produtos = expande_vetor(vetor_produtos, quantidade_produtos); // Expande o tamanho do vetor em 1 (tamb�m incrementa em 1 a vari�vel que representa a quantidade de produtos no vetor)
		vetor_produtos[(*quantidade_produtos) - 1] = (*item); // Adiciona o produto na primeira posi��o ap�s o �ltimo produto v�lido do vetor
	}

	return vetor_produtos; // Retorna o vetor
}

produto* pedido(produto vetor_estoque[], unsigned numero_produto, produto cesta[], unsigned* quantidade_cesta)
{
	// Exibe as informa��es do produto selecionado
	cout << "Pedido\n"
		<< "======\n";
	cout << vetor_estoque[numero_produto].nome << endl;
	cout << vetor_estoque[numero_produto].preco << endl << "======\n";

	// Solicita a quantidade de unidades do produto escolhido
	cout << "Quantidade: [_]\b\b";
	unsigned quantidade_pedido;
	cin >> quantidade_pedido;

	produto item; // Representa um produto selecionado, em que o campo "quantidade" representa a quantidade solicitada pelo usu�rio no pedido
	bool produto_existente = false; // Representa se o produto j� est� ou n�o na cesta
	for (int i = 0; i < *quantidade_cesta; i++) // Percorre a cesta
	{
		if (!strcmp(vetor_estoque[numero_produto].nome, cesta[i].nome)) // Verifica se o produto escolhido j� est� presente na cesta
		{
			produto_existente = true;
			if ((quantidade_pedido + cesta[i].quantidade) > vetor_estoque[numero_produto].quantidade) // Verifica se a quantidade a mais solicitada pelo usu�rio � maior que a permitida
				cout << "\nQuantidade insuficiente no estoque!\n\n";
			else // Caso a quantidade seja permitida
			{
				// Atribui as informa��es do produto para a vari�vel item
				item = vetor_estoque[numero_produto];
				item.quantidade = quantidade_pedido;

				cesta = adiciona_produto(cesta, quantidade_cesta, &item, produto_existente); // Adiciona o produto � cesta
			}
		}
	}

	if (!produto_existente) // Caso o produto n�o esteja na cesta
	{
		if (quantidade_pedido > vetor_estoque[numero_produto].quantidade) // Verifica se a quantidade solicitada pelo usu�rio � maior que a permitida
			cout << "\nQuantidade insuficiente no estoque!\n\n";
		else // Caso a quantidade seja permitida
		{
			// Atribui as informa��es do produto para a vari�vel item
			item = vetor_estoque[numero_produto];
			item.quantidade = quantidade_pedido;

			cesta = adiciona_produto(cesta, quantidade_cesta, &item, produto_existente); // Adiciona o produto � cesta
		}
	}

	return cesta; // Retorna o vetor de cesta
}

void menu_produtos(produto estoque[], unsigned quantidade_estoque, produto cesta[], unsigned quantidade_cesta)
{
	cout << "\n RapiZinho \n"
		<< "===========\n";
	// Exibe a cesta do usu�rio no momento
	for (int i = 0; i < quantidade_cesta; i++)
		cout << cesta[i].quantidade << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].quantidade * cesta[i].preco << endl;

	cout << "===========\n";

	// Exibe o menu
	for (int i = 0; i < quantidade_estoque; i++)
	{
		cout << '(' << char('A' + i) << ") " << estoque[i].nome << endl;
	}
	cout << "(S) Sair\n===========\n";
}

void sistema_vendas(produto vetor_estoque[], unsigned* quantidade_produtos_estoque)
{
	ofstream fout; // Cria uma vari�vel que ser� utilizada para a escrita do recibo em arquivo texto
	char arquivo_pedido[20]; // Representa o nome do arquivo texto que ser� gerado

	unsigned quantidade_pedidos = 0; // Representa a quantidade de pedidos feitos durante a execu��o

	unsigned quantidade_cesta = 0; // Representa a quantidade de produtos na cesta
	produto* cesta = nullptr; // Inicializa o ponteiro do vetor cesta como "nulo"

	float total_pagamento = 0; // Representa o valor da conta a pagar
	const float taxa_entrega = 6; // Representa uma taxa fixa para e entrega do pedido

	char opcao; // Representa a op��o do menu escolhida pelo usu�rio
	char forma_pagamento;
	float desconto; // Representa a taxa de desconto
	char confirma_pedido;
	char numero_pedido[11]; // 10 caracteres v�lidos pois o maior unsigned int tem 10 d�gitos

	do
	{
		do
		{
			menu_produtos(vetor_estoque, *quantidade_produtos_estoque, cesta, quantidade_cesta); // Exibe o menu

			// Solicita a op��o
			cout << "Op��o: [ ]\b\b";
			cin >> opcao;
			cout << endl;

			if (tolower(opcao) >= 'a' && tolower(opcao) < ('a' + *quantidade_produtos_estoque)) // Verifica se a op��o escolhida est� no intervalo das op��es dispon�veis
			{
				cesta = pedido(vetor_estoque, tolower(opcao) - 'a', cesta, &quantidade_cesta); // Chama a fun��o para realizar o pedido do produto escolhido
			}
			else if (opcao != 's' && opcao != 'S')
			{
				cout << "Op��o inv�lida!\n";
			}
		} while (opcao != 's' && opcao != 'S');

		if (quantidade_cesta > 0) // Caso a cesta n�o esteja vazia
		{
			cout << "\n RapiZinho \n"
				<< "===========\n";
			// Exibe a cesta 
			for (int i = 0; i < quantidade_cesta; i++)
			{
				cout << cesta[i].quantidade << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].quantidade * cesta[i].preco << endl;
				total_pagamento += cesta[i].quantidade * cesta[i].preco; // Adiciona o valor � conta a ser paga

			}
			cout << "Taxa de entrega = R$" << taxa_entrega << endl
				<< "===========\n"
				<< "Total: R$" << (total_pagamento += taxa_entrega) << endl; // Atualiza o valor da conta com a taxa de entrega

			// Solicita a forma de pagamento
			cout << "\n[P] Pix\n[C] Cart�o\n";
			cout << "\nPagamento: [_]\b\b";
			cin >> forma_pagamento;

			desconto = 0;
			while (desconto == 0)
			{
				switch (forma_pagamento)
				{
				case 'p':
				case 'P':
					desconto = 0.10;
					break;

				case 'c':
				case 'C':
					desconto = 0.05;
					break;

				default:
					cout << "Op��o Inv�lida, tente novamente.\n Pagamento: [_]\b\b";
					cin >> forma_pagamento;
				}
			}

			cout << "\n RapiZinho \n"
				<< "===========\n";
			// EXibe a cesta
			for (int i = 0; i < quantidade_cesta; i++)
				cout << cesta[i].quantidade << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = R$" << cesta[i].quantidade * cesta[i].preco << endl;

			cout << "Taxa de entrega = R$" << taxa_entrega << endl
				<< "Desconto de " << int(desconto * 100) << "% = R$" << total_pagamento * desconto << endl // Mostra o desconto
				<< "===========\n";
			cout << "Total = R$" << (total_pagamento * (1 - desconto)) << endl; // Mostra o valor da conta com desconto

			// Solicita a confirma��o do pedido
			cout << "Confirmar Pedido (S/N): [_]\b\b";
			cin >> confirma_pedido;

			if (confirma_pedido == 's' || confirma_pedido == 'S') // Caso o pedido seja confirmado
			{
				quantidade_pedidos++; // Incrementa em 1 a quantidade de pedidos

				// Atualiza a quantidade dos produtos vendidos no estoque
				for (int i = 0; i < quantidade_cesta; i++)
					for (int j = 0; j < *quantidade_produtos_estoque; j++)
						if (!strcmp(cesta[i].nome, vetor_estoque[j].nome))
							vetor_estoque[j].quantidade -= cesta[i].quantidade;

				// Cria o nome do arquivo texto de recibo
				strcpy(arquivo_pedido, "pedido_");
				_itoa(quantidade_pedidos, numero_pedido, 10);
				strcat(arquivo_pedido, numero_pedido);
				strcat(arquivo_pedido, ".txt");

				// Abertura do aquivo texto de recibo
				fout.open(arquivo_pedido);
				fout.setf(ios_base::fixed, ios_base::floatfield); 
				fout.precision(2); // Configura o fout para escrever pontos flutuantes com 2 casas decimais no arquivo
				if (!fout.is_open())
				{
					cout << "Falha em abertura de arquivo.\n";
					exit(EXIT_FAILURE);
				}

				// Escreve os dados do pedido no arquivo
				fout << "Pedido #" << quantidade_pedidos << endl << "--------------------------------------------------\n";
				for (int i = 0; i < quantidade_cesta; i++)
					fout << cesta[i].quantidade << " x " << cesta[i].nome << " de R$" << cesta[i].preco << " = " << cesta[i].preco * cesta[i].quantidade << endl;
				fout << "Taxa de entrega = R$" << taxa_entrega << endl;
				fout << "Desconto de " << int(desconto * 100) << "% = R$" << (total_pagamento * desconto) << endl;
				fout << "--------------------------------------------------\n";
				fout << "Total = R$" << (total_pagamento *= (1 - desconto));

				fout.close(); // Fechamento do arquivo texto
			}

			delete[] cesta; // Libera��o da mem�ria alocada com cesta
			cesta = nullptr; // atribui valor "nulo" ao ponteiro de cesta

			// "Reseta" as vari�veis
			quantidade_cesta = 0;
			total_pagamento = 0;
			opcao = '\0';
		}
	} while (opcao != 's' && opcao != 'S');

	if ((*quantidade_produtos_estoque) > 0) // Caso exista produtos no vetor de estoque
	{

		// Transcri��o dos dados do vetor estoque para o arquivo bin�rio de estoque
		fout.open("estoque.bin", ios_base::out | ios_base::binary);
		fout.write((char*)quantidade_produtos_estoque, sizeof(unsigned));
		fout.write((char*)vetor_estoque, sizeof(produto) * (*quantidade_produtos_estoque));
		fout.close();

		delete[] vetor_estoque; // Libera��o da mem�ria alocada com o ponteiro do vetor de estoque
	}
}