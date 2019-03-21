#include <iostream>
#include <string>

using namespace std;

int funcaoHash(string c)
{
	int h = (c.size() % 23);
	return h;
}

class noh
{
	friend class tabelaHash;
	private:
	    string chave;
	    string valor;
	    noh* proximo;
	public:
	    noh(string c, string v)
	    {
			chave = c;
			valor = v;
			proximo = NULL;
		}
};

class tabelaHash
{
	private:
	    noh** elementos;
	    int capacidade;
	public:
	    tabelaHash(int cap = 100);
	    ~tabelaHash();
	    void insere(string c, string v);
	    string busca(string c);
};

tabelaHash::tabelaHash(int cap)
{
	elementos = new noh*[cap];
	capacidade = cap;
	
	for(int i = 0; i < cap; i++)
	{
		elementos[i] = NULL;
	}
}

tabelaHash::~tabelaHash()
{
	for (int i = 0; i < capacidade; i++)
	{
		noh* atual = elementos[i];
		
		while(atual != NULL)
		{
			noh* ant = atual;
			atual = atual -> proximo;
			delete ant;
		}
	}
	delete[] elementos;
}

void tabelaHash::insere(string c, string v)
{
	int posicao = funcaoHash(c);
	
	if(elementos[posicao] == NULL)
	{
		noh* novo = new noh(c,v);
		elementos[posicao] = novo;
	}
	else
	{
		noh* atual = elementos[posicao];
		
		while(atual -> proximo != NULL)
		{
			atual = atual -> proximo;
		}
		
		noh* novo = new noh(c,v);
		atual -> proximo = novo;
	}
}

string tabelaHash::busca(string c)
{
	int posicao = funcaoHash(c);
	
	if(elementos[posicao] != NULL and elementos[posicao] -> chave == c)
        return elementos[posicao] -> valor;
    else 
    {
        noh* atual = elementos[posicao];
        while ((atual != NULL) and (atual -> chave != c)) 
        {
            atual = atual->proximo;
        }
            if ((atual != NULL) and (atual -> chave == c)) 
            {
                return atual->valor;
            } 
            else 
            {
                return "NULL";
            }
        }
}

int main()
{
	tabelaHash minhaTabela(23);
	int quantElem;
	string chave;
	string valor;
	cin >> quantElem;
	
	for (int i = 0; i < quantElem; i++)
	{
		cin >> chave;
        cin.ignore();
		getline(cin, valor);
		
		minhaTabela.insere(chave, valor);
	}
	
	string chaveAux = "NULL";
	
	while(chaveAux != "-1")
	{
		cin >> chaveAux;
        if (chaveAux != "-1")
        {
            valor = minhaTabela.busca(chaveAux);
            cout << "[" << chaveAux << "] => " << valor << endl;
        }
	}
	cout << endl;
	
	return 0;
}
