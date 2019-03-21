#include <iostream>
using namespace std;

typedef int Dado;
class noh // objeto da lista
{ 
    friend class lista;
    
    private:
        Dado dado;
        noh* proximo;
        
    public:
        noh(Dado d = 0);
};

noh::noh(Dado d) : dado(d) 
{
    proximo = NULL;
}

class lista 
{
	friend class noh;
    private: // atributos
        noh* primeiro;
        noh* ultimo;
        unsigned tam;
        
    public: // metodos
        lista();
        ~lista();
        void Insere(int tam);
        void Imprime(int tam);
};

// constrói uma lista inicialmente vazia
lista::lista() 
{
    primeiro = NULL;
    ultimo = primeiro;
}

lista::~lista()  // ???
{
    noh* iter = primeiro;
    noh* proximo = NULL;
    while (iter != NULL) {
        proximo = iter->proximo;
        delete iter;
        iter = proximo;
    }
}
void lista::Insere(int tam) 
{
	int cont = 0;
	noh* aux;
	while ( cont != tam )
	{
		aux = new noh(cont+1);
		if(primeiro == NULL) //LISTA VAZIA
		{
			primeiro = aux;
			ultimo = aux;	
		}
		else
		{
			ultimo->proximo = aux;
			aux->proximo = primeiro;
			ultimo = aux;
			
		}
		cont++;
	}
}
void lista::Imprime(int tam) 
{
    noh* aux = primeiro;
    int cont = 0;
    while ( cont < tam )
    {
        cout << aux->dado << " ";
        aux = aux->proximo;
        cont++;
    }
    cout << "saiu" << endl;
    cout << endl;
}
int main()
{
	lista minhaLista;
    int tam;
    cin >> tam;
	minhaLista.Insere(tam)
    minhaLista.Imprime(tam);
return 0;
}
