#include <iostream>
#include <cstdlib>
using namespace std;
typedef string word;
class noh {
	public:
		word palavra;
		noh *dir;
		noh *esq;
};
class fila{
	private: 
		int tamanho;
		int capacidade;
		noh *inicio;//aponta para uma caixa
		noh *fim;//aponta para uma caixa
	public:
		fila(int cap);//construtor
		~fila();//destrutor
		bool vazia();
		void enfilar(word palav);
		word desenfilar();
		word undo();
};
fila::fila(int cap){ //constroi a fila
	tamanho=0;
	capacidade= cap;
	inicio=NULL;
	fim=NULL;
}
fila::~fila(){ //destroi a fila
	while(!vazia()){
		desenfilar();
	}
}
bool fila::vazia(){//true pra vazia
	return (tamanho==0);
}

void fila::enfilar(word palav){
	if ( tamanho <= capacidade )
	{
		noh *aux = new noh();//cria um apontador do tipo caixa que aponta uma nova caixa
		aux->palavra=palav;
		aux->dir=NULL;
		if(vazia()){
			inicio=aux;
			fim=aux;
		}
		else{
			fim->dir=aux;
			fim->esq = fim;
			fim=aux;
		}
		tamanho++;
	}
	else
	{
		cout << "esqueci: ";
		cout << desenfilar() << endl;
		enfilar(palav);
	}
}
word fila::desenfilar(){
	noh *aux=inicio;
	if(tamanho==1){
		inicio=NULL;
		fim=NULL;
	}
	else{
		inicio=aux->dir;
		aux->esq = NULL;
	}
	word guarda = aux->palavra;
	delete aux;
	tamanho--;
	return guarda;
}	
word fila::undo()
{
	noh *aux= fim;
	if(tamanho==1){
		inicio=NULL;
		fim=NULL;
	}
	else{
		fim = aux->esq;
	}
	word guarda = aux->palavra;
	delete aux;
	tamanho--;
	return guarda;
}	
int main(){
	int cap;
	cin >> cap;
	fila deque(cap);
	word comando;
	do{
		getline(cin, comando);
		if ( comando == "undo" ){
			if ( !deque.vazia() ){
				string auxiliar = deque.undo();
				cout << "desfazer: ";
				cout << auxiliar << endl;
			}
			else{
				cout << "ERRO" << endl;
			}
		}else {
			deque.enfilar(comando);
		}
	}while( comando != "sair" );
	return 0;
}
