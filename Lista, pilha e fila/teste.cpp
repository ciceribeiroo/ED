#include <iostream>

typedef int Dado;

class Noh 
{
	friend class Fila;
	private:
		Dado mDado;
		Noh* mPtProx;
};

class Fila 
{
	public:
		// Constroi uma fila vazia.
		Fila();
		// Desaloca memória de todos os nós da fila.
		~Fila();
		// Remove e retorna o primeiro item da fila.
		Dado Remover();
		// Insere um item no final da fila.
		void Inserir(const Dado& d);
		// Retorna o primeiro item da fila sem remover.
		Dado Proximo() const;
		// Remove todos os itens da fila.
		void LimparTudo();
		// Consulta a quantidade de itens na fila.
		unsigned Tamanho() const { return mTamanho; }
		// Consulta se a fila está vaiza.
		bool Vazia() const;
	private:
		Noh* mPrimeiro;
		Noh* mUltimo;
		unsigned mTamanho;
};

using namespace std;

Fila::Fila() 
{
	mPrimeiro = NULL;
	mUltimo = NULL;
	mTamanho = 0;
}

Fila::~Fila() 
{
	while (!Vazia())
	{
		Remover();
	}
}

void Fila::Inserir(const Dado& d) 
{
	Noh* aux = new Noh();
	aux->mDado = d;
	aux->mPtProx = NULL;
	if ( mPrimeiro == NULL)
	{
		mPrimeiro = aux;
		mUltimo = aux;
	}
	 else
	{
		mUltimo->mPtProx = aux;
		mUltimo = aux;
	}
	mTamanho++;
}

Dado Fila::Remover() 
{
	Noh* aux = mPrimeiro;
	int guarda;
	if(mTamanho == 0)
	{
		cerr<< "Erro" << endl;
		return Dado();
	}
	if(mTamanho == 1)
	{
		guarda = mPrimeiro->mDado;
		mPrimeiro = NULL;
		mUltimo = NULL;
		mTamanho = 0; 
	}
	else
	{
		guarda = aux->mDado;
		mPrimeiro = mPrimeiro->mPtProx;
		mTamanho--;
	}
	delete(aux);
	return guarda;
}

Dado Fila::Proximo() const 
{
	return mPrimeiro->mDado;
}

void Fila::LimparTudo() 
{
		while (!Vazia())
	{
		Remover();
	}
}

bool Fila::Vazia() const 
{
	if(mTamanho == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main() 
{
	Fila fila;
	Dado valor;
	char comando;
	do 
	{
		cin >> comando;
		switch (comando) 
		{
			case 'i': // inserir
				cin >> valor;
				fila.Inserir(valor);
				break;
			case 'r': // remover
				cout << fila.Remover() << endl;
				break;
			case 'l': // limpar tudo
				fila.LimparTudo();
				break;
			case 't': // escrever tamanho
				cout << fila.Tamanho() << endl;
				break;
			case 'e': // espiar proximo
				cout << fila.Proximo() << endl;
				break;
			case 'f': // finalizar
				// vai verificar adiante
				break;
			default:
				cerr << "comando inválido\n";
		}
	} 
	while (comando != 'f'); // finalizar execução
	while (not fila.Vazia()) 
	{
		cout << fila.Remover() << ' ';
	}
	cout << endl;
	return 0;
}
