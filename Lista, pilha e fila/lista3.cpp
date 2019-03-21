#include <iostream>

using namespace std;
typedef int Dado;

class noh
{
	friend class Lista;
	private:
		noh* proximo;
		noh* anterior;
		Dado Matricula;
		string NomeAluno;
	public:
		noh(Dado matricula, string nome);
};
noh::noh(Dado matricula, string nome)
{
	Matricula = matricula;
	NomeAluno = nome;
	proximo = NULL;
	anterior = NULL;
}
class Lista{
	private:
		noh* inicio;
		noh* fim;
		int tam;
	public:
		Lista();
        ~Lista();
		void inserir(Dado matricula, string nome);
		void remove(Dado matricula);
		void imprimir();
		void imprimirreverso();
		bool vazia();
}; 
Lista::Lista() 
{
    inicio = NULL;
    fim = NULL;
    tam = 0;
}

Lista::~Lista() 
{
    noh* iter = inicio;
    noh* proximo = NULL;
    while (iter != NULL) {
        proximo = iter->proximo;
        delete iter;
        iter = proximo;
    }
}
void Lista::inserir ( Dado matricula, string nome )
{
	noh* aux = new noh(matricula, nome);
	if ( inicio == NULL)
	{
		inicio = aux;
		fim = aux;
	}
	else
	{
		noh* aux2 = inicio;
		aux2->anterior = aux;
		aux->proximo = aux2;
		inicio = aux;
	}
	tam++;
}
void Lista::remove(int matr) //ARRUMAR
{
	noh* nav = inicio;
	bool encontrou = false;
	while ( nav != NULL && encontrou == false )
	{
		if ( nav->Matricula != matr )
		{
			nav = nav->proximo;
		}
		else
		{
			encontrou = true;
		}
	}
	if ( encontrou == false )
	{
		cout << "ERRO" << endl;
	}
	else
	{
		if ( tam == 1 )
		{
			inicio = NULL;
			fim = NULL;
		}
		else
		{
			if ( nav->Matricula == inicio->Matricula ) //primeiro
			{
				inicio = nav->proximo; // o primeiro aponta pro proximo do primeiro
				inicio->anterior = NULL;
			}
			else if ( nav->Matricula == fim->Matricula ) //ultimo
			{
				fim = nav->anterior;
				fim->proximo = NULL;
			}
			else//meio
			{
				noh* aux2 = nav->anterior;
				noh* aux3 = nav->proximo;
				aux2->proximo = aux3;
				aux3->anterior = aux2;
			}
			tam--;
			delete(nav);
		}
	}
}
void Lista::imprimir() 
{
    noh* aux = inicio; //ponteiro auxiliar que vai percorrer a lista e acessar as classes do objeto
    while (aux != NULL)  //enquanto nao chegar no fim
    {
        cout << aux->Matricula << " " << aux->NomeAluno << " ";
        aux = aux->proximo; // aponta pra onde o proximo aponta indo assim p proximo
    }
    cout << endl;
}
void Lista::imprimirreverso() 
{
    noh* aux = fim; 
    while (aux != NULL)  
    {
        cout << aux->Matricula << " " << aux->NomeAluno << " ";
        aux = aux->anterior; 
    }
    cout << endl;
}
int main()
{
	Lista umalista;
	string quero;
	int mat;
	string name;
	do
	{
		cin >> quero;
		if ( quero == "i" )
		{
			cin >> mat >> name;
			umalista.inserir(mat, name);
		}
		else if ( quero == "r")
		{
			cin >> mat;
			umalista.remove(mat);
		}
		else if ( quero == "ed" )
		{
			umalista.imprimir();
		}
		else if ( quero == "er")
		{
			umalista.imprimirreverso();
		}
	}
	while ( quero != "t" );
	
return 0;
}
