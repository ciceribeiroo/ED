#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <queue>

typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore

// === DeclaraÃ§Ãµes das classes ===================================================
class NohAVL; // declaraÃ§Ã£o avanÃ§ada

class AVL {
    public:
        AVL();
        ~AVL();
        TValor& Valor(TChave chave);
        void EscreverEmOrdem();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(TChave chave, const TValor& valor);
        bool ConferirLigacoes();
        void Remover(TChave chave);
    private:
        NohAVL* mPtRaiz;
};

class NohAVL {
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);
    friend void AVL::EscreverNivelANivel(std::ostream& saida);
    public:
        NohAVL (TChave c, const TValor& v);
        ~NohAVL();
        void DesalocarFilhosRecursivo();
        NohAVL* InserirRecursivo(NohAVL* ptNoh);
        NohAVL* MenorRecursivo();
        NohAVL* RemoverRecursivo(TChave chave);
        TValor& Valor(TChave chave);
    private:
        NohAVL* ArrumarBalanceamento();
        NohAVL* BuscaAux(TChave chave);
        void AtualizarAltura();
        int FatorBalanceamento();
        NohAVL* Remover();
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        void SubstitutirPor(NohAVL* ptNoh);
        void Transplantar(NohAVL* ptFilho);
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

// === ImplementaÃ§Ãµes das classes ================================================

using namespace std;

// === Classe NohAVL =============================================================

NohAVL::NohAVL(TChave c, const TValor& v)
    : mChave(c), mValor(v), mPtEsq(NULL), mPtDir(NULL), mPtPai(NULL), mAltura(1) {
}

NohAVL::~NohAVL() {
}

// Faz as rotaÃ§Ãµes e ajustes necessÃ¡rios inclusive do nÃ³ pai. Atualiza a altura.
// Retorna o ponteiro para o nÃ³ que ficar na posiÃ§Ã£o dele apÃ³s os ajustes.
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

// Insere um nÃ³ numa subÃ¡rvore. Retorna o ponteiro para o nÃ³ que ficou no lugar do que recebeu
// a mensagem.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {
	if (ptNoh->mChave < mChave){
        if(mPtEsq == NULL){
		    mPtEsq = ptNoh;
		    ptNoh->mPtPai = this;
        }
        else{
           mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
	   }
	} else {
		if(mPtDir == NULL){
            mPtDir = ptNoh;
            ptNoh->mPtPai = this;
        }
        else mPtDir = mPtDir->InserirRecursivo(ptNoh);
	}
    return ArrumarBalanceamento();
}

// Busca o nÃ³ que tem a menor chave. Retorna o ponteiro para ele.
NohAVL* NohAVL::MenorRecursivo() {
	if(mPtEsq != NULL) 
		return mPtEsq->MenorRecursivo();
	else 
		return this;
}

// Remove o nÃ³. Retorna o ponteiro para o nÃ³ que ficou no lugar dele.
// Confira com RemoverRecursivo(TChave).
NohAVL* NohAVL::Remover() {
	cout << "remover" << endl;
	NohAVL* nova;
	if(mPtEsq == NULL ){
		cout << "mPtEsq == NULL" << endl;
		nova = mPtDir;
		Transplantar(this);
	}
	else if(mPtDir == NULL){
		cout << "mPtDir == NULL" << endl;
		nova = mPtEsq;
		Transplantar(this);
	}
	else if (mPtDir != NULL && mPtEsq != NULL){
		cout << "tem dois filhos" << endl;
		NohAVL* sucessor = mPtDir->MenorRecursivo(); 
		nova = sucessor;
		if (sucessor->mPtPai != this){
			cout << "if" << endl;
			SubstitutirPor(sucessor);
			NohAVL* aux2 = sucessor->mPtPai;
			while (aux2 != this){
				cout << "while" << endl;
				aux2->AtualizarAltura();
				aux2 = aux2->mPtPai;
			}
			sucessor->mPtDir = mPtDir->ArrumarBalanceamento();
			sucessor->mPtDir->mPtPai = nova;
		}
		TrocarFilho(this, sucessor);
		sucessor->mPtEsq = mPtEsq;
		sucessor->mPtEsq->mPtPai= nova;
	}
	delete(this);
	if(nova == NULL) return nova;
	else return nova->ArrumarBalanceamento();
}

// Busca recursivamente um nÃ³ com dada chave e o remove. Retorna o ponteiro para o nÃ³ que ficou
// no lugar do nÃ³ que recebeu a mensagem.
// Confira com Remover().
NohAVL* NohAVL::RemoverRecursivo(TChave chave) {
	cout << "remover recursivo" << endl;
	if( mChave < chave) return mPtDir->RemoverRecursivo(chave);
	else if ( mChave > chave) return mPtEsq->RemoverRecursivo(chave);
	else
	{
		NohAVL* aux = BuscaAux(chave);
		return aux->Remover();
	}
}

// Rotaciona Ã  direita a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
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

// Rotaciona Ã  esquerda a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
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

// Atualiza todas as ligaÃ§Ãµes, para que ptNoh, fique no lugar deste nÃ³.
// Usado na remoÃ§Ã£o com dois filhos.
void NohAVL::SubstitutirPor(NohAVL* ptNoh) {
	cout << "substituir por" << endl;
    NohAVL* sucessor = mPtDir->MenorRecursivo();
    if ( sucessor->mPtPai!= ptNoh ){
		TrocarFilho(sucessor, sucessor->mPtDir);
		sucessor->mPtDir = ptNoh->mPtDir;
		sucessor->mPtDir->mPtPai = sucessor;
	}
	TrocarFilho(ptNoh, sucessor);
	sucessor->mPtEsq = ptNoh->mPtEsq;
	sucessor->mPtEsq->mPtPai = sucessor;
}

// Muda as ligaÃ§Ãµes do pai e do filho para desligar o nÃ³ atual.
// Usado para remoÃ§Ã£o na situaÃ§Ã£o em que nÃ£o existem os dois filhos.
void NohAVL::Transplantar(NohAVL* ptFilho) {
	cout << "transplantar" << endl;
    if ( ptFilho->mPtEsq == NULL ){
		TrocarFilho(ptFilho, ptFilho->mPtDir);
	}
	else if ( ptFilho->mPtDir == NULL ){
		TrocarFilho(ptFilho, ptFilho->mPtEsq);
	}
}

// Substitui um dos filhos por um novo nÃ³.
void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
	cout << "trocar filho" << endl;
    if(mPtPai == ptAntigo){
		mPtPai = ptNovo;
	}
    else if ( ptAntigo == ptAntigo->mPtPai->mPtEsq){ 
		ptAntigo->mPtPai->mPtEsq = ptNovo;
	}
    else {
		ptAntigo->mPtPai->mPtDir = ptNovo;
	}
    if ( ptNovo != NULL ){
		 ptNovo->mPtPai = ptAntigo->mPtPai;
	 }
}

// Busca recursivamente uma dada chave e retorna o valor associado a ela.
// Levanta exceÃ§Ã£o se nÃ£o encontrar a chave.
TValor& NohAVL::Valor(TChave chave) {
	if(mChave == chave) return mValor;
	else if (chave > mChave) return mPtDir->Valor(chave);
	else return mPtEsq->Valor(chave);
	throw runtime_error("Nao existe esse valor na arvore");
}
NohAVL* NohAVL::BuscaAux(TChave chave){
	if(mChave == chave) return this;
	else if (chave > mChave) return mPtDir->BuscaAux(chave);
	else return mPtEsq->BuscaAux(chave);
	throw runtime_error("Nao existe esse valor na arvore");
}
// Escreve o conteÃºdo de um nÃ³ no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
ostream& operator<<(ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() : mPtRaiz(NULL) {
}

AVL::~AVL() {
    if (mPtRaiz != NULL) {
        mPtRaiz->DesalocarFilhosRecursivo();
        delete mPtRaiz;
    }
}

// Retorna o valor associado a uma dada chave.
TValor& AVL::Valor(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de buscar valor numa arvore vazia.");
    return mPtRaiz->Valor(chave);
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

// Remove da Ã¡rvore o nÃ³ que tiver uma dada chave.
void AVL::Remover(TChave chave) {
	cout << "remover" << endl;
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de remover elemento de uma Ã¡rvore vazia.");
    else{
		mPtRaiz= mPtRaiz->RemoverRecursivo(chave);
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
            case 'r': // Remover
                try {
                    cin >> chave;
                    minhaArvore.Remover(chave);
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
                break;
            case 'b': // Buscar
                try {
                    cin >> chave;
                    valor = minhaArvore.Valor(chave);
                    cout << valor << endl;
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
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
