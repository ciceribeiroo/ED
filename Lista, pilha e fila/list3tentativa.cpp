#include <iostream>

using namespace std;

typedef int matricula;
typedef string nome;

class aluno
{
	friend class noh;
	
	private:
		const matricula mat;
		const nome name;
	public:
};
class noh // objeto da lista
{ 
    friend class lista;
    
    private:
        const aluno Aluno;
        noh* proximo;
        noh* anterior;
        
    public:
        noh(Dado d = 0);
};

noh::noh(Dado d) : dado(d) 
{
    proximo = NULL;
    anterior = NULL;
}

class lista 
{
    private: // atributos
        noh* primeiro;
        noh* ultimo;
        int tamanho;
        
    public: // metodos
        lista();
        ~lista();
        void InsereInicio(matricula mat, nome name);
        void Remove(matricula mat);
        void ImprimeDireta();
        void ImprimeReversa();
};

// constrói uma lista inicialmente vazia
lista::lista() 
{
    tamanho = 0;
    primeiro = NULL;
    ultimo = NULL;
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

// método básico que *percorre* a lista, imprimindo seus elementos
void lista::ImprimeDireta() 
{
    noh* aux = primeiro; //ponteiro auxiliar que vai percorrer a lista e acessar as classes do objeto
    while (aux != NULL)  //enquanto nao chegar no fim
    {
        cout << aux->Aluno->mat << " " << aux->Aluno->name ;
        aux = aux->proximo; // aponta pra onde o proximo aponta indo assim p proximo
    }
    cout << endl;
}

void lista::ImprimeReversa() 
{
    noh* aux = ultimo; //ponteiro auxiliar que vai percorrer a lista e acessar as classes do objeto
    while (aux != NULL)  //enquanto nao chegar no fim
    {
        cout << aux->Aluno->mat << " " << aux->Aluno->name ;
        aux = aux->anterior; // aponta pra onde o proximo aponta indo assim p proximo
    }
    cout << endl;
}

// insere por padrão no final da lista
void lista::InsereInicio(aluno Aluno) 
{
	noh* aux = new noh(Aluno); // cria um ponteiro q armazena uma lista ate o final
	if(primeiro == NULL) //LISTA VAZIA
	{	
		primeiro = aux;
		ultimo = aux;	
	}
	else
	{
		aux->anterior = NULL;
		aux->proximo = primeiro
		primeiro = aux 
	}
	tamanho++;
}

// remove o item da posição passada por parâmetro
// Atenção - o código deve tratar de posição inválidas
void lista::Remove(matricula mat) 
{	
	if(posicao >= 0 and posicao < tamanho) // posicao valida
	{
		int cont = 0;
		noh* nav = primeiro; // percorrer toda a lista
		while(cont < posicao-1)
		{
			nav = nav->proximo; // nav fica com o endereço de memoria do que queremos
			cont++;
		}
		if(posicao == 0) // se for o primeiro
		{
			primeiro = nav->proximo; // o primeiro aponta pro proximo do primeiro
			delete(nav); // deleta 
		}
		else if(posicao == tamanho-1) // se for o ultimo
		{
			delete(nav->proximo); // no penultimo deleta o ponteiro pro proximo
			nav->proximo = NULL; // coloca o proximo como vazio
			ultimo = nav; // aponta o ponteiro do ultimo pro penultimo e encerra a lista nele
			// o ultimo ainda existe mas nao esta conectado a lista
		}
		else // se for em uma posicao aleatória
		{
			noh* aux = nav->proximo; // auxiliar que guarda o proximo do que queremos
			nav->proximo = nav->proximo->proximo; // o proximo do que queremos pula um e vai pra frente
			delete(aux); // deleta o auxiliar que contem o endereco de memoria que queremos isolando o valor da lista e assim o removendo
		}
		tamanho--;
	}
}

int main() 
{ 
    lista minhaLista;
    string opcao;
    int matri;
    string noome;
    cin >> opcao;
    while (opcao != "t") 
    {
        switch(opcao) 
        {
            case "i":
                cin >> matri;
                cin >> noome;
                minhaLista.InsereInicio(matri, noome);
                break;
            case "r":
                cin >> matri;
                minhaLista.Remove(matri);
                break;
            case "ed":
                cout << minhaLista.ImprimeDireta << endl;
                break;
            case "er":
				cout << minhaLista.ImprimeReversa << endl;
				break;
        }
        cin >> opcao;
    }
    return 0;
}
