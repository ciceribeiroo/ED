#include <iostream>
#include <exception>
using namespace std;
typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore
class Noh{
    friend class AVL;
    private:
        Data chave;
        Lista lista;
        Noh* esq;
        Noh* dir;
        Noh* pai;
        unsigned int altura;
    public:
        Noh(int d);
        ~Noh();
};

Noh::Noh(Data d){
    chave = d;
    esq = NULL;
    dir = NULL;
    pai = NULL;
};

Noh::~Noh(){
    delete esq;
    delete dir;
}

class AVL {
    friend class Noh;
    private:
        Noh* raiz;
    public:
        AVL();
        ~AVL();
        int informaAltura(Noh* umNoh);
        void atualizarAltura(Noh* umNoh);
        int maximo(int a, int b);
        int fatorBalanceamento(Noh* umNoh);
        Noh* rotacaoEsquerda(Noh* umNoh);
        Noh* rotacaDireita(Noh* umNoh);
        Noh* rotacaoEsquerdaDireita (Noh* umNoh);
        Noh* rotacaoDireitaEsquerda (Noh* umNoh);
        void inserirRec (int d);
        Noh* inserirRecAux (Noh* umNoh, int d);
        void EscreverNivelANivel(ostream& saida)
};

AVL::AVL(){
    raiz = NULL;
}

AVL::~AVL(){
    delete raiz;
}

int AVL::informaAltura(Noh* umNoh){
    if (umNoh == NULL){
        return 0;
    } else {
        return umNoh->altura;
    }
}

void AVL::atualizarAltura(Noh* umNoh){
    int altEsq = informaAltura(umNoh->esq);
    int altDir = informaAltura(umNoh->dir);
    
    umNoh->altura = 1 + maximo(altEsq, altDir);
}

int AVL::maximo(int a, int b){
    if (a > b){
        return a;
    } else {
        return b;
    }
}

int AVL::fatorBalanceamento(Noh* umNoh){
    int altEsq = informaAltura(umNoh->esq);
    int altDir = informaAltura(umNoh->dir);
    
    return altEsq - altDir;
}

Noh* AVL::rotacaoEsquerda(Noh* umNoh){
    Noh* aux = umNoh->dir;
    umNoh->dir = aux->esq;
    
    if (aux->esq != NULL){
        aux->esq->pai = umNoh;
    }
    aux->pai = umNoh->pai;
    
    if (umNoh == raiz){
        raiz = aux;
    } else if (umNoh == umNoh->pai->esq){
        umNoh->pai->esq = aux;
    } else {
        umNoh->pai->dir = aux;
    }
    
    aux->esq = umNoh;
    umNoh->pai = aux;
    
    atualizarAltura(aux);
    atualizarAltura(umNoh);
    
    return aux;
}

Noh* AVL::rotacaDireita(Noh* umNoh){
    Noh* aux = umNoh->esq;
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
    
    atualizarAltura(aux);
    atualizarAltura(umNoh);
    
    return aux;
}

Noh* AVL::rotacaoEsquerdaDireita (Noh* umNoh){
    umNoh->esq = rotacaoEsquerda(umNoh->esq);
    return rotacaDireita(umNoh);
}

Noh* AVL:: rotacaoDireitaEsquerda (Noh* umNoh){
    umNoh->dir = rotacaDireita(umNoh->dir);
    return rotacaoEsquerda(umNoh);
}

void AVL::inserirRec(int d){
    raiz = inserirRecAux(raiz, d);
}

Noh* AVL::inserirRecAux (Noh* umNoh, int d){
    if (umNoh == NULL){
        Noh* novo = new Noh(d);
        return novo;
    } else if (d < umNoh->chave){
        umNoh->esq = inserirRecAux(umNoh->esq, d);
        umNoh->esq->pai = umNoh;
    } else {
        umNoh->dir = inserirRecAux(umNoh->dir, d);
        umNoh->dir->pai = umNoh;
    }
    return arrumarBalanciamento(umNoh);
}
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nÃ³ especial para marcar fim de um nÃ­vel
    filhos.push(mPtRaiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        NohAVL* ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mPtEsq);
                filhos.push(ptNoh->mPtDir);
            }
        }
    }
    delete fimDeNivel;
}
Noh* AVL::arrumarBalanciamento(Noh* umNoh){
    atualizarAltura(umNoh);
    int bal = fatorBalanceamento(umNoh);
    
    if ((bal >= -1) and (bal <= 1)){
        return umNoh;
    }
    
    if ((bal > 1) and (fatorBalanceamento(umNoh->esq) >= 0)){
        return rotacaDireita(umNoh);
    }
    if ((bal > 1) and (fatorBalanceamento(umNoh->esq) < 0)){
        umNoh->esq = rotacaoEsquerda(umNoh);
        return rotacaDireita(umNoh);
    }
    if ((bal < -1) and (fatorBalanceamento(umNoh->dir) <= 0)){
        return rotacaoEsquerda(umNoh);
    }
    if ((bal < -1) and (fatorBalanceamento(umNoh->dir) > 0)){
        umNoh->dir = rotacaDireita(umNoh->dir);
        return rotacaoEsquerda(umNoh);
    }
    return umNoh;
}

// === Programa ===================================================================================
int main() {
    AVL minhaArvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                minhaArvore.inserirRec(chave, valor);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                minhaArvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
}

