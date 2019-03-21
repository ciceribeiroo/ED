#include <iostream>

using namespace std;

class noh // objeto da lista
{ 
    friend class lista;
    
    private:
        unsigned numero;
        noh* proximo;
        noh* anterior;
        
    public:
        noh(unsigned num);
};

noh::noh(unsigned num) : numero(num) 
{
    proximo = NULL;
    anterior = NULL;
}

class lista 
{
	friend class noh;
    private: // atributos
        noh* primeiro;
        noh* ultimo;
        unsigned tamanho;
        
    public: // metodos
        lista(unsigned tam);
        ~lista();
        void Insere(unsigned tam);
        void apagar(int k);
        void remove(int posicao , unsigned tam);
        unsigned resposta();
        inline bool vazia();
};

// constrói uma lista inicialmente vazia
lista::lista(unsigned tam) 
{
    primeiro = NULL;
    ultimo = NULL;
     for (tamanho = 0; tamanho < tam; tamanho++)//tamanho = cont como um atributo
    {
        Insere(tamanho+1); //construindo com valores ja
    }
    ultimo->proximo = primeiro; //circular
    primeiro->anterior = ultimo; //circular
}

lista::~lista()  // ???
{
	cout << "descontrutor" << endl;
    noh* iter = primeiro;
    for (tamanho = tamanho; tamanho > 0; tamanho--) //nao pode usar null~>erro
	{
        primeiro = iter->proximo;
        delete iter;
        iter = primeiro;
    }
}
inline bool lista::vazia(){
    return primeiro == NULL;
}

// método básico que *percorre* a lista, imprimindo seus elementos
unsigned lista::resposta() 
{
   cout << "retornando" << endl;
   return primeiro->numero;
}

void lista::Insere(unsigned tam) 
{ 
		noh* aux = new noh(tam);
		if(tamanho == 0) //LISTA VAZIA
		{
			primeiro = aux;
			ultimo = aux;	
		}
		else //prox do ultimo aponta pro primeiro e o anterior do primeiro aponta pro ultimo
		{
			ultimo->proximo = aux;
			aux->anterior = ultimo;
			ultimo = aux;
			ultimo->proximo= primeiro;
			primeiro->anterior = ultimo;
		}
}

void lista::apagar(int posicao) 
{	
	noh* ant = NULL;
	noh* prox = NULL;
	noh* aux = primeiro; // percorrer toda a lista
	noh* deleta = NULL;
	//while (tamanho != 1 ) fora do metodo
		for ( int i = 0; i < posicao; i++ )
		{
			aux=aux->proximo;
		}
		ant = aux->anterior;
        prox = aux->proximo;
		ant->proximo = prox;
		prox->anterior = ant;
		if(aux == primeiro)
		{
            primeiro = prox;
        }
        if(aux == ultimo)
        {
            ultimo = ant;
        }
        deleta = aux;
        delete deleta;
        tamanho--;
}
int main() 
{ 
    unsigned tam, k;
    cin >> tam;
    cin >> k;
    lista minhaLista(tam);
    while ( !minhaLista.vazia() )
    {
		minhaLista.apagar(k);
	}
    minhaLista.resposta();
    return 0;
}
