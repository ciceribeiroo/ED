#include <iostream>
#include <stdexcept>
#include <queue>

using namespace std;

typedef std::string TValor;
typedef unsigned TChave;
enum Cor {PRETO, VERMELHO};

class noh {
    friend class arvoreRN;
	friend std::ostream& operator<<(std::ostream& saida, noh* ptNoh);
	public:
        noh(unsigned chave, TValor valor);
  private:
	   TValor mValor;
	   noh* Tio();
		TChave mChave;
		noh* pai;
		noh* esq;
		noh* dir;
		Cor cor;
};

noh::noh(TChave c, TValor v){
	mChave = c;
	mValor = v;
	cor = VERMELHO;
	pai = NULL;
	esq = NULL;
	dir = NULL;
}
noh* noh::Tio() {
	if((pai != NULL)&&(pai->pai!=NULL)){
		if(pai == pai->pai->dir){
			if(pai->pai->esq != NULL){
				return pai->pai->esq;
			}
			else {
				return NULL;
			}
		 }
		else{
			if ( pai->pai->dir!= NULL){
				return pai->pai->dir; 
			}
			else
			{
				 return NULL;
			 }
		}
	}
	else{
		return NULL;
	}
}
class arvoreRN {
  private:
    noh* raiz;
    bool insereAux(noh* umNoh);
    bool arrumaInsercao(noh* umNoh);
    void rotacaoEsquerda(noh* umNoh);
    void rotacaoDireita(noh* umNoh);

  public:
    arvoreRN();
    ~arvoreRN();
    bool insere(TChave c, TValor v);
     void EscreverNivelANivel(std::ostream& saida);
};

arvoreRN ::arvoreRN() {
    raiz = NULL;
}

arvoreRN ::~arvoreRN() {
    delete raiz;
}

bool arvoreRN ::insere(TChave c, TValor v) {
    noh* novo = new noh(c,v);
    novo->mChave = c;
    novo->esq = NULL;
    novo->dir = NULL;

    if (raiz == NULL) {
        novo->cor = PRETO;
        novo->pai = NULL;
        raiz = novo;
        return true;
    }
    noh* atual = raiz;
    noh* anterior;
    while (atual != NULL) {
        anterior = atual;
        if (c > atual->mChave) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }
    novo->pai = anterior;
    novo->cor = VERMELHO;
    if (anterior->mChave < novo->mChave) {
        anterior->dir = novo;
    } else {
        anterior->esq = novo;
    }

    return arrumaInsercao(novo);
}

bool arvoreRN ::arrumaInsercao(noh* umNoh) {
    while ((umNoh != raiz) and (umNoh->pai->cor == VERMELHO)) {
		noh* tio = umNoh->Tio();
        if ((tio != NULL) and (tio->cor == VERMELHO)) {
			cout << "tio e null" << endl;
            // alterar as cores do pai, tio e avô
            umNoh->pai->cor = PRETO;
            umNoh->pai->pai->cor = VERMELHO;
            tio->cor = PRETO;
            // mover umNoh para o avô para continuar a verificação
            umNoh = umNoh->pai->pai;
        } else {
            // tio preto e pai é vermelho
            // CASO 1 : Rotação a esquerda simples (LL)
            if ((umNoh == umNoh->pai->esq) and (umNoh->pai == umNoh->pai->pai->esq)) {
				cout << "caso 1" << endl;
                tio->cor = PRETO;
				umNoh->pai->cor = PRETO;
                umNoh->pai->pai->cor = VERMELHO;
                rotacaoDireita(umNoh->pai->pai);
                continue;
            }
            // CASO 2 : Rotação Esquerda-Direita (LR)
            if ((umNoh->pai == umNoh->pai->pai->esq) and (umNoh == umNoh->pai->dir)) {
				cout << "caso 2" << endl;
                umNoh = umNoh->pai;
				rotacaoEsquerda(umNoh);
				umNoh->pai->cor = PRETO;
                umNoh->pai->pai->cor = VERMELHO;
                rotacaoDireita(umNoh->pai->pai);
                continue;
            }

            // CASO 3 : Rotação a direita simples (RR)
            if ((umNoh->pai == umNoh->pai->pai->dir) and (umNoh == umNoh->pai->dir)) {
				cout << "caso 3" << endl;
                umNoh->pai->cor = PRETO;
                umNoh->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(umNoh->pai->pai);
                continue;
            }
            // CASO 4 : Rotação Direita-Esquerda (RL)
            if ((umNoh->pai = umNoh->pai->pai->dir) and (umNoh == umNoh->pai->esq)) {
				cout << "caso 4" << endl;
                umNoh = umNoh->pai;
                rotacaoDireita(umNoh);
                umNoh->pai->cor = PRETO;
                umNoh->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(umNoh->pai->pai);
                continue;
            }
        }
    }
    raiz->cor = PRETO;
    return true;
}

void arvoreRN::EscreverNivelANivel(ostream& saida) {
    queue<noh*> filhos;
    filhos.push(raiz);
    while (not filhos.empty()) {
        unsigned nroNohsNesteNivel = unsigned(filhos.size());
        for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
            noh* ptNoh = filhos.front();
            filhos.pop();
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->esq);
                filhos.push(ptNoh->dir);
            }
        }
        cout << "\n";
    }
}

// Escreve um nÃ³ (ou NIL).
ostream& operator<<(ostream& saida, noh* ptNoh) {
    static char vetLetrasCores[] = "PV";
    if (ptNoh == NULL)
        saida << "NIL";
    else
        saida << '[' << vetLetrasCores[ptNoh->cor] << ':' << ptNoh->mChave << '/'
              << ptNoh->mValor << ']';
    return saida;
}


void arvoreRN ::rotacaoDireita(noh* umNoh) {
	cout << "rotacao a direita" << endl;
    noh* aux = umNoh->esq;
    umNoh->dir = aux->dir;
    
    if (aux->dir != NULL){
        aux->dir->pai = umNoh;
    }
    aux->pai = umNoh->pai;
    
    if (umNoh == raiz){
        raiz = aux;
    } else if (umNoh == umNoh->pai->esq){
        umNoh->pai->esq = aux;
    } else {
        umNoh->pai->dir = aux;
    }
    
    aux->dir = umNoh;
    umNoh->pai = aux;
    
}

void arvoreRN ::rotacaoEsquerda(noh* umNoh) {
	cout << "rotacao a esquerda" << endl;
    noh* aux = umNoh->dir;
    umNoh->dir = aux->esq;
    if (aux->esq != NULL) {
        aux->esq->pai = umNoh;
    }
    aux->esq = umNoh;
    if (umNoh->pai == NULL) {
        raiz = aux;
    } else if (umNoh->pai != NULL) {
        if (umNoh == umNoh->pai->esq) {
            umNoh->pai->esq = aux;
        } else {
            umNoh->pai->dir = aux;
        }
    }
    aux->pai = umNoh->pai;
    umNoh->pai = aux;
}

int main() {
    arvoreRN arvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                arvore.insere(chave, valor);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                arvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
    return 0;
}
