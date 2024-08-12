#include <iostream>
#include <cstring>
#include <fstream>
#include "Estoque.h"
using namespace std;

void menu_acoes()
{
	cout << "\nPainel de Controle\n"
		<< "===================\n"
		<< "(A)dicionar\n"
		<< "(E)xcluir\n"
		<< "(L)istar\n"
		<< "(S)air\n"
		<< "===================\n";
}

produto* expande_vetor(produto vetor_produtos[], unsigned* quantidade_produtos, unsigned* tamanho_vetor)
{
	produto* auxiliar = vetor_produtos; // Cria um ponteiro auxiliar que aponta para o mesmo endere�o que o vetor original

	(*quantidade_produtos)++; // incrementa em 1 a vari�vel que representa a quantidade de produtos
	(*tamanho_vetor)++; // incrementa em 1 a vari�vel que representa o tamanho do vetor

	vetor_produtos = new produto[*quantidade_produtos]; // Aloca mem�ria com 1 espa�o a mais em um novo endere�o

	// Copia os produtos do vetor auxiliar para o novo vetor
	for (int i = 0; i < *quantidade_produtos - 1; i++)
		vetor_produtos[i] = auxiliar[i];

	delete[] auxiliar; // Libera a mem�ria apontada pelo ponteiro auxiliar;

	return vetor_produtos; // retorna o novoS vetor com 1 espa�o a mais
}

produto* adiciona_produto(produto vetor_produtos[], unsigned* quantidade_produtos, unsigned* tamanho_vetor)
{
	produto item; // Representa o produto a ser adicionado no estoque

	// Solicita ao usu�rio as informa��es do produto a ser adicionado
	cout << "\nAdicionar\n"
		<< "---------\n";
	cout << "Nome: ";
	cin.get();
	cin.getline(item.nome, 24);

	cout << "Pre�o: ";
	cin >> item.preco;

	cout << "Quantidade: ";
	cin >> item.quantidade;

	bool produto_existente = false; // Representa se o produto j� existe no estoque
	for (int i = 0; i < *quantidade_produtos; i++)
	{
		if (!strcmp(item.nome, vetor_produtos[i].nome)) // Caso o produto exista no estoque
		{
			produto_existente = true;
			vetor_produtos[i].quantidade += item.quantidade; // Soma a quantidade de unidades do produto
			vetor_produtos[i].preco = item.preco; // Atualiza o pre�o do produto
		}
	}

	if (!produto_existente) // Caso o produto n�o exista no estoque
	{
		if ((*tamanho_vetor) > (*quantidade_produtos)) // Caso o tamanho do vetor seja maior que a quantidade de produtos v�lidos nele
		{
			vetor_produtos[*quantidade_produtos] = item; // Adiciona o produto na primeira posi��o ap�s o �ltimo produto v�lido
			(*quantidade_produtos)++; // Incrementa em 1 a vari�vel que representa a quantidade de produtos no estoque
		}
		else if ((*tamanho_vetor) == (*quantidade_produtos)) // Caso o tamanho do vetor seja igual � quantidade de produtos v�lidos nele
		{
			vetor_produtos = expande_vetor(vetor_produtos, quantidade_produtos, tamanho_vetor); // Expande o vetor em 1 posi��o a mais (tamb�m incrementa em 1 a vari�vel que representa a quantidade de produtos no estoque)
			vetor_produtos[(*quantidade_produtos) - 1] = item; // Adiciona o produto na �ltima posi��o do vetor
		}
	}
	return vetor_produtos; // Retorna o vetor de estoque atualizado
}

void confirma_exclusao(produto vetor_estoque[], unsigned* quantidade_produtos, unsigned numero_produto)
{
	// Verifica se o usu�rio deseja confirmar a exclus�o

	char opcao;

	cout << "Deseja excluir \"" << vetor_estoque[numero_produto].nome << "\" (S/N) ? ";
	cin >> opcao;

	switch (opcao)
	{
	case 's':
	case 'S':
		vetor_estoque[numero_produto] = vetor_estoque[(*quantidade_produtos) - 1];
		(*quantidade_produtos)--;
		break;

	case 'n':
	case 'N': 
		cout << "Exclus�o cancelada\n";
		break;

	default:
		cout << "Op��o inv�lida. Exclus�o cancelada.\n";
	}

	// OBS: Caso o usu�rio digite uma op��o inv�lida, a exclus�o � cancelada
	
}

void exclui_produto(produto vetor_estoque[], unsigned* quantidade_produtos)
{
	char opcao;
	cout << "\nExcluir\n"
		<< "-------\n";

	// Exibe o menu de produtos para excluir
	for (int i = 0; i < (*quantidade_produtos); i++)
		cout << i + 1 << ") " << vetor_estoque[i].nome << endl;

	// Solicita  ao usu�rio a op��o escolhida
	cout << "Produto: [_]\b\b";
	cin >> opcao;

	if (opcao >= '1' && opcao < ('1' + (*quantidade_produtos))) // Caso a escolha esteja no intervalo de op��es dispon�veis
		confirma_exclusao(vetor_estoque, quantidade_produtos, opcao - '1'); // Chama a fun��o para poder confirmar ou cancelar a exclus�o
	else
		cout << "Op��o inv�lida!\n";
}

void lista_produtos(produto vetor_estoque[], unsigned* quantidade_produtos)
{
	cout << "\nListagem\n"
		<< "--------\n";

	// Exibe a lista dos produtos com suas respectivas informa��es
	for (int i = 0; i < (*quantidade_produtos); i++)
		cout << vetor_estoque[i].nome << " - R$" << vetor_estoque[i].preco << " - " << vetor_estoque[i].quantidade << " und.\n";
}

void controle_estoque(produto vetor_estoque[], unsigned* quantidade_produtos_estoque, unsigned* tamanho_vetor_estoque)
{
	char opcao; // Representa a a��o a ser escolhida

	do
	{
		menu_acoes(); // Exibe o menu de a��es

		// Solicita ao usu�rio a a��o escolhida
		cout << "Op��o: [_]\b\b";
		cin >> opcao;
		cout << endl;

		switch (opcao)
		{
		case 'a':
		case 'A':
			vetor_estoque = adiciona_produto(vetor_estoque, quantidade_produtos_estoque, tamanho_vetor_estoque); // Chama a fun��o de adicionar produto no vetor de estoque
			break;

		case 'e':
		case 'E':
			exclui_produto(vetor_estoque, quantidade_produtos_estoque); // Chama a fun��o de excluir produto do vetor de estoque
			break;

		case 'l':
		case 'L':
			lista_produtos(vetor_estoque, quantidade_produtos_estoque); // Chama a fun��o de listar os produtos
			break;

		case 's':
		case 'S':
			break; // Op��o de sair
	
		default:
			cout << "Op��o Inv�lida!\n";
		}

	} while (opcao != 's' && opcao != 'S');

	if ((*tamanho_vetor_estoque) > 0) // Caso o tamanho do vetor seja maior do que zero (ou seja, caso o vetor tenha sido criado em algum momento)
	{
		// Transcri��o dos dados do vetor de estoque para o arquivo bin�rio
		ofstream fout;
		fout.open("estoque.bin", ios_base::out | ios_base::binary);
		fout.write((char*)quantidade_produtos_estoque, sizeof(unsigned));
		fout.write((char*)vetor_estoque, sizeof(produto) * (*quantidade_produtos_estoque));
		fout.close(); // Fechamento do arquivo bin�rio de estoque

		delete[] vetor_estoque; // Libera��o da mem�ria alocada com o ponteiro do vetor de estoque
	}
}