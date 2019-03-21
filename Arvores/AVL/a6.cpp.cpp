#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

class NohAVL; // declaraÃ§Ã£o avanÃ§ada

typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore

// DeclaraÃ§Ãµes das classes ===================================================

class NohAVL {
    friend class AVL;
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);
    private:
        NohAVL (TChave c, const TValor& v);
        ~NohAVL();
        NohAVL* ArrumarBalanceamento(); //ok
        void AtualizarAltura(); //ok
        void DesalocarFilhosRecursivo(); //ok
        int FatorBalanceamento(); //ok
        NohAVL* InserirRecursivo(NohAVL* ptNoh); //ok
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

class AVL {
    public:
        AVL();
        ~AVL();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(TChave chave, const TValor& valor);
    private:
        NohAVL* mPtRaiz;
};

// === Classe NohAVL ==============================================================================

using namespace std;

NohAVL::NohAVL(TChave c, const TValor& v) {
    mChave = c;
    mValor = v;
    mPtPai = NULL;
    mPtEsq = NULL;
    mPtDir = NULL;
    mAltura = 1;
}

NohAVL::~NohAVL() {
    DesalocarFilhosRecursivo();
}

// Faz as rotaÃ§Ãµes e ajustes necessÃ¡rios inclusive do nÃ³ pai. Atualiza a altura.
// Retorna o nÃ³ que ficar na posiÃ§Ã£o dele apÃ³s os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
	AtualizarAltura();
    int fb = FatorBalanceamento();
    
    if ((fb > 1) and (mPtEsq->FatorBalanceamento() >= 0)){ //rotacao simples ~> mesmo sinal
        return RotacionarDireita();                         //direita ~> positivo
    }
    if ((fb > 1) and (mPtEsq->FatorBalanceamento() < 0)){ //rotacao dupla ~> sinais contrarios
        mPtEsq = mPtEsq->RotacionarEsquerda();                  //fb positivo~>esquerda-direita
        return RotacionarDireita();
    }
    if ((fb < -1) and (mPtDir->FatorBalanceamento() <= 0)){//rotacao simples ~> mesmo sinal
        return RotacionarEsquerda();                     //esquerda ~> negativo
    }
    if ((fb < -1) and (mPtDir->FatorBalanceamento() > 0)){ //rotacao dupla ~> sinais contrarios
        mPtDir = mPtDir->RotacionarDireita();                //fb negativo ~>direita-esquerda
        return RotacionarEsquerda();
    }
    return this;
}

// Calcula e atualiza a altura de um nÃ³.
void NohAVL::AtualizarAltura() {
    int hEsq;
    if(mPtEsq != NULL) hEsq = mPtEsq->mAltura;
    else hEsq = 0;
    int hDir;
    if (mPtDir != NULL) hDir= mPtDir->mAltura;
    else hDir = 0;
    
    if ( hEsq > hDir) mAltura = hEsq+1;
    else mAltura = hDir+1;    
    //aux->altura = 1 + maximo(hEsq, hDir);
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
	if(mPtEsq != NULL) delete mPtEsq;
    if(mPtDir != NULL) delete mPtDir;
}

// Calcula e retorna o fator de balanceamento do nÃ³.
int NohAVL::FatorBalanceamento() {
	int altArvEsq;
    int altArvDir;
    if(mPtEsq != NULL){
        altArvEsq = mPtEsq->mAltura;
    }else{
        altArvEsq = 0;
    }
    if(mPtDir != NULL){
        altArvDir = mPtDir->mAltura;
    }else{
        altArvDir = 0;
    }
    int fator_balanceamento = altArvEsq - altArvDir;
    return fator_balanceamento;
}

// Insere um nÃ³ numa subÃ¡rvore.
NohAVL* NohAVL::InserirRecursivo(NohAVL* atual) {
	if (atual->mChave < mChave){
        if(mPtEsq == NULL){
		    mPtEsq = atual;
		    atual->mPtPai = this;
        }
        else{
           mPtEsq = mPtEsq->InserirRecursivo(atual);
	   }
	} else {
		if(mPtDir == NULL){
            mPtDir = atual;
            atual->mPtPai = this;
        }
        else mPtDir = mPtDir->InserirRecursivo(atual);
	}
    return ArrumarBalanceamento();
}

// Rotaciona a subÃ¡rvore Ã  direita. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarDireita() {
	NohAVL* aux = mPtEsq;
    this->mPtEsq = aux->mPtDir;
    
    if (aux->mPtDir != NULL){
        aux->mPtDir->mPtPai = this;
    }
    aux->mPtDir = this;
    this->mPtPai = aux;
    
    this->AtualizarAltura();
    aux->AtualizarAltura();
    
    return aux;
}

// Rotaciona a subÃ¡rvore Ã  esquerda. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarEsquerda() {
	NohAVL* aux = this->mPtDir;
    this->mPtDir = aux->mPtEsq;
    
    if (aux->mPtEsq != NULL){
        aux->mPtEsq->mPtPai = this;
    }
    aux->mPtPai = this->mPtPai;
    
    aux->mPtEsq = this;
    this->mPtPai = aux;
    
    this->AtualizarAltura();
    aux->AtualizarAltura();
    
    return aux;
}

// Substitui um dos filhos por um novo nÃ³.
// void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
// 	NohAVL* aux = this;
//     if ( aux->mPtPai == ptAntigo ) 
	// 	aux->mPtPai = ptNovo;
 //    else if ( ptAntigo == ptAntigo->mPtPai->mPtEsq) 
// 		ptAntigo->mPtPai->mPtEsq = ptNovo;
//     else 
// 		ptAntigo->mPtPai->mPtDir = ptNovo;
//     if ( ptNovo != NULL ) 
	// 	ptNovo->mPtPai = ptAntigo->mPtPai;
// }

// Escreve o conteÃºdo de um nÃ³ no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() {
    mPtRaiz = NULL;
}

AVL::~AVL() {
   delete mPtRaiz;
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel, na saÃ­da de dados informada.
// Usado para conferir se a estrutra da Ã¡rvore estÃ¡ correta.
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

// Insere um par chave/valor na Ã¡rvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
    NohAVL* novo = new NohAVL(chave, valor);
	if ( mPtRaiz == NULL ){
		mPtRaiz = novo;
	}
	else{
		mPtRaiz = mPtRaiz->InserirRecursivo(novo);
	}
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
                minhaArvore.Inserir(chave, valor);
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
