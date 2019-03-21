#include <iostream>
#include <cstdlib>

using namespace std;

typedef int Dado;

class pilha {
	private: 
		int capacidade;
		int posTopo;//aponta para uma caixa
		Dado *dados;
		int tamanho;
	public:
		pilha( int cap = 20 );//construtor
		~pilha();//destrutor
		void Empilha( Dado valor );
		int Desempilha ();
		void depura ();	
		void info();
		int espia();	
};
pilha::pilha( int cap  ){ //constroi a pilha
	capacidade = cap;
	tamanho = 0;
	posTopo = 0;
	dados = new int[cap];
}
pilha::~pilha(){ //destroi a pilha
	while(tamanho != 0){
		Desempilha();
	}
}
int pilha::espia()
{
	return dados[posTopo];
}
void pilha::info()
{
	cout << tamanho << " " << --posTopo << endl;
}
void pilha::depura(){
	for ( int i = 0; i < tamanho; i++ )
	{
		cout << dados[i] << " ";
	}
	cout << endl;
}
void pilha:: Empilha (int valor){
	this->dados[posTopo] = valor;
	posTopo++;
	tamanho++;
}
int pilha:: Desempilha (){
	int guarda = 7;
	tamanho--;
	posTopo--;
	guarda = dados[posTopo];
	dados[posTopo] = Dado();
	return guarda;
}
	
int main(){
	pilha pi;
	Dado valor;
	for ( int i = 0; i < 5; i++ )
	{
		cin >> v[i];
	}
	for ( int i = 0; i < 4; i++ )
	{
		cin >> vet[i];
	}
	for ( int i = 0; i < 5; i++ )
	{
		pi.Empilha(v[i]);
	}
	
	for ( int i = 0; i < 3; i++ )
	{
		cout << pi.Desempilha() << " ";
	}
	cout << endl;
	for ( int i = 0; i < 4; i++ )
	{
		pi.Empilha(vet[i]);
	} 
	for ( int i = 0; i < 3; i++ )
	{
		cout << pi.Desempilha() << " ";
	}
	cout << endl;
	cout << pi.espia() << endl;
	pi.depura();
	pi.info();
	return 0;		
}
