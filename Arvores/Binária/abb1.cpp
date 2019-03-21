#include <iostream>
#include <queue>

typedef int Dado;

enum Posicao {DIR, ESQ};

class Noh {
    friend class ABB;
    public:
        Noh (Dado d = Dado());
        ~Noh();
    protected:
        Dado valor;
        Noh* esq;
        Noh* dir;
        Noh* pai;
};

class ABB {
    public:
        ABB() { raiz = NULL; }
        ~ABB();
        // Insere um dado na Ã¡rvore.
        void Inserir(Dado d);
        // Verifica se um dado tem sucessor e o retorna.
        void Remover (Dado d);
        Noh* RemoveAtual ( Noh* aux);
        void EmOrdem();
        void EOAux( Noh* aux);
        void PreOrdem();
        void POAux( Noh* aux);
        Noh* BuscaAux(Dado d);
    protected:
        Noh* raiz;
        Dado Nivel( Dado d );
};

using namespace std;
// === Classe Noh ==============================================================
Noh::Noh(Dado d) // d tem valor default
    : valor(d), esq(NULL), dir(NULL), pai(NULL) {
}

Noh::~Noh() {
    delete esq;
    delete dir;
}

// === Classe ABB ==============================================================
ABB::~ABB(){
    delete raiz;
}

void ABB::Inserir(Dado d) {
    Noh* novo = new Noh(d);
    if (raiz == NULL) {
        raiz = novo;
    } else {
        Posicao posInsercao;
        Noh* atual = raiz;
        Noh* anterior = NULL;
        while (atual != NULL) {
            anterior = atual;
            if (atual->valor > d) {
                atual = atual->esq;
                posInsercao = ESQ;
            } else {
                atual = atual->dir;
                posInsercao = DIR;
            }
        }
        novo->pai = anterior;
        if (posInsercao == DIR) {
            anterior->dir = novo;
        } else {
            anterior->esq = novo;
        }
    }
}
void ABB ::Remover(Dado d){
	if ( raiz == NULL ){
		cout << "ERRO" << endl;
	}
	else{
		// Noh* ant = NULL;
		Noh* atual = raiz;
		if (atual->valor == d){
			raiz = RemoveAtual(atual);
		} else {
			while ( atual != NULL && d != atual->valor){
				// ant = atual;
				if ( d > atual->valor ){
					atual = atual->dir;
				}else{
					atual = atual->esq;
				}
				
				if ( d == atual->valor ){
					atual = RemoveAtual(atual);
				}
			}
		}
	}
}

Noh* ABB::BuscaAux( Dado d){
	cout << "busca" << endl;
	Noh* atual = raiz;
	cout << "atual" << endl;
	while(atual != NULL ){
		cout << "while" << endl;
		if ( atual->valor == d){
			cout << "CHAVE ATUAL: " << atual->valor << endl;
			return atual;
		}
		else if (atual->valor > d){
			cout << "menor" << endl;
			atual = atual->esq;
		}
		else{
			cout << "maior" << endl;
			atual = atual->dir;
		}
	}
	
	return atual;
}

Noh* ABB:: RemoveAtual(Noh* aux){
	cout << "remove atual" << endl;
	Noh* n1= NULL;
	Noh* n2 = NULL;
	if ( aux->esq == NULL ){
		cout << "esquerda e null" << endl;
		n2 = aux->dir;
		cout << "n2:"<< n2->valor << endl;
		delete (aux);
		return n2;
	}
	else{
		n1 = aux;
		cout << "n1 = aux" << endl;
		n2 = aux->esq;
		cout << "n2 = aux->esq" << endl;
		while ( n2->dir != NULL ){
			cout << "while dir" << endl;
			n1 = n2;
			n2 = n2->dir;
		}
		if ( n1 != aux ){
			cout << "entrou no while" << endl;
			n1->dir = n2->esq;
			n2->esq = aux ->esq;
		}
		
		cout << "aux:"<< aux->valor << endl;
		cout << "n1:" <<n1->valor << endl;
		cout << "n2:"<< n2->valor << endl;
		delete(aux);
		return n2;
	}
}
void ABB::EmOrdem(){
	EOAux(raiz);
}
void ABB::EOAux( Noh* aux){
	if ( aux != NULL){
		EOAux(aux->esq);
		cout << aux->valor << "/" << Nivel(aux->valor) << " ";
		EOAux(aux->dir);
	}
}
void ABB::PreOrdem(){
	POAux(raiz);
}
void ABB::POAux( Noh* aux){
	if ( aux != NULL ){
		cout << aux->valor << "/" << Nivel(aux->valor) << " ";
		POAux(aux->esq);
		POAux(aux->dir);
	}
}
Dado ABB::Nivel(Dado d){
	Noh* atual = raiz;
	Dado cont = 0;
	while ( atual->valor != d && atual != NULL ){
		cont++;
		if ( atual->valor > d){
			atual = atual->esq;
		}
		else{
			atual = atual->dir;
		}
	}
	return cont;
}
// === Programa ================================================================
int main() {
    ABB arvore;
    Dado chave;
    char operacao;
    do {
        cin >> operacao;
        switch (operacao) {
            case 'i': // Inserir
                cin >> chave;
                arvore.Inserir(chave);
                break;
            case 'r': // Escrever
				cin >> chave;
				// arvore.RemoveAtual(arvore.BuscaAux(chave));
                arvore.Remover(chave);
                break;
            case 'o':  // Sucessor
                arvore.EmOrdem();
                cout << endl;
                break;
            case 'p':  // Sucessor
                arvore.PreOrdem();
                cout << endl;
                break;
        }
    } while (operacao != 'q');
    return 0;
}
