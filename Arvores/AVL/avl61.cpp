#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>
 
class NohAVL; // declaração avançada
 
typedef std::string TChave; // chave da árvore
typedef float TValor; // valor da árvore
 
// Declarações das classes ===================================================
 
class NohAVL {
    friend class AVL;
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);
    private:
        NohAVL (TChave c, const TValor& v);
        ~NohAVL();
        NohAVL* ArrumarBalanceamento();
        void AtualizarAltura();
        void DesalocarFilhosRecursivo();
        int FatorBalanceamento();
        NohAVL* InserirRecursivo(NohAVL* ptNoh);
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        //~ void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas têm altura 1
        NohAVL* Remover_Recursivo(TChave chave);
        NohAVL* Deletar();
        NohAVL* Menor_Recursivo();
        NohAVL* Transplanta(NohAVL* b);
        TValor Valor_Recursivo(TChave chave);
};
 
class AVL {
    public:
        AVL();
        ~AVL();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(TChave chave, const TValor& valor);
        TValor Valor(TChave chave);
        void Remover(TChave chave);
    private:
        NohAVL* mPtRaiz;
};
 
// === Classe NohAVL ==============================================================================
 
using namespace std;
 
NohAVL::NohAVL(TChave c, const TValor& v) : mChave(c), mValor(v) {
    mPtEsq = NULL;
    mPtDir = NULL;
    mPtPai = NULL;
    mAltura = 1;
}
 
NohAVL::~NohAVL() {
    DesalocarFilhosRecursivo();
}
 
// Faz as rotações e ajustes necessários inclusive do nó pai. Atualiza a altura.
// Retorna o nó que ficar na posição dele após os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
    AtualizarAltura();
   
    int bal = FatorBalanceamento();
   
    if (bal > 1 and mPtEsq->FatorBalanceamento() >= 0)
        return RotacionarDireita();
   
    if (bal > 1 and mPtEsq->FatorBalanceamento() < 0){
        mPtEsq = mPtEsq->RotacionarEsquerda();
        return RotacionarDireita();
    }
   
    if (bal < -1 and mPtDir->FatorBalanceamento() <= 0)
        return RotacionarEsquerda();
   
    if (bal < -1 and mPtDir->FatorBalanceamento() > 0){
        mPtDir = mPtDir->RotacionarDireita();
        return RotacionarEsquerda();
    }
   
    return this;
}
 
// Calcula e atualiza a altura de um nó.
void NohAVL::AtualizarAltura() {
    int hEsq;
    int hDir;
   
    if(mPtEsq != NULL)
        hEsq = mPtEsq->mAltura;
    else
        hEsq = 0;
   
    if(mPtDir != NULL)
        hDir = mPtDir->mAltura;
    else
        hDir = 0;
   
    if(hEsq >= hDir)
        mAltura = hEsq + 1;
    else
        mAltura = hDir + 1;
}
 
// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
    if (mPtEsq != NULL)
        delete mPtEsq;
   
    if (mPtDir != NULL)
        delete mPtDir;
}
 
// Calcula e retorna o fator de balanceamento do nó.
int NohAVL::FatorBalanceamento() {
    int hEsq;
    int hDir;
   
    if(mPtEsq != NULL)
        hEsq = mPtEsq->mAltura;
    else
        hEsq = 0;
   
    if(mPtDir != NULL)
        hDir = mPtDir->mAltura;
    else
        hDir = 0;
   
    return hEsq - hDir;
}
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh){
    if (ptNoh->mChave < mChave){
        if (mPtEsq == NULL){
            mPtEsq = ptNoh;
            ptNoh->mPtPai = this;
        }
        else
            mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
    }
    else {
        if (mPtDir == NULL){
            mPtDir = ptNoh;
            ptNoh->mPtPai = this;
        }
        else
            mPtDir = mPtDir->InserirRecursivo(ptNoh);
    }
    return ArrumarBalanceamento();
}
 
// Rotaciona a subárvore à direita. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarDireita() {
    NohAVL* aux = mPtEsq;
   
    mPtEsq = aux->mPtDir;
   
    if (aux->mPtDir != NULL)
        aux->mPtDir->mPtPai = this;
       
    aux->mPtPai = mPtPai;
    aux->mPtDir = this;
    mPtPai = aux;
   
    AtualizarAltura();
    aux->AtualizarAltura();
    return aux;
}
 
// Rotaciona a subárvore à esquerda. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarEsquerda() {
    NohAVL* aux = mPtDir;
   
    mPtDir = aux->mPtEsq;
   
    if (aux->mPtEsq != NULL)
        aux->mPtEsq->mPtPai = this;
       
    aux->mPtPai = mPtPai;
    aux->mPtEsq = this;
    mPtPai = aux;
   
    AtualizarAltura();
    aux->AtualizarAltura();
    return aux;
}
 
// Substitui um dos filhos por um novo nó.
//~ void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    //~ #warning NohAVL::TrocarFilho não implementado.
//~ }
 
// Escreve o conteúdo de um nó no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}
 
NohAVL* NohAVL::Remover_Recursivo(TChave chave){
    if (chave < mChave){
        if(mPtEsq == NULL){
            throw runtime_error("chave nao encontrada");
        }
        else if (chave == mPtEsq->mChave){
            mPtEsq->Deletar();
        }else{
            mPtEsq = mPtEsq->Remover_Recursivo(chave);
        }
    }else if(chave > mChave){
        if(mPtDir == NULL){
            throw runtime_error("chave nao encontrada");
        }
        else if (chave == mPtDir->mChave){
            mPtDir->Deletar();
        }else{
            mPtDir = mPtDir->Remover_Recursivo(chave);
        }
    }
    return ArrumarBalanceamento();
}
 
NohAVL* NohAVL::Transplanta( NohAVL* b){
    if (mPtPai == NULL){
    }
    else if (this == mPtPai->mPtEsq)
        mPtPai->mPtEsq = b;
    else
        mPtPai->mPtDir = b;
    if (b != NULL)
        b->mPtPai = mPtPai;
    return b;
}
 
NohAVL* NohAVL::Deletar(){
    NohAVL* removido = this;
    NohAVL* sucessor = NULL;
    if (mPtEsq == NULL)
        sucessor = Transplanta(mPtDir);
    else if (mPtDir == NULL)
        sucessor = Transplanta(mPtEsq);
    else {
        sucessor = mPtDir->Menor_Recursivo();
        if (sucessor->mPtPai != this){
            sucessor->Transplanta (sucessor->mPtDir);
            sucessor->mPtDir = mPtDir;
            sucessor->mPtDir->mPtPai = sucessor;
        }
        Transplanta(sucessor);
        sucessor->mPtEsq = mPtEsq;
        sucessor->mPtEsq->mPtPai = sucessor;
    }
    removido->mPtEsq = NULL;
    removido->mPtDir = NULL;
    delete removido;
    return sucessor;
}
 
TValor NohAVL::Valor_Recursivo(TChave chave){
    if(chave < mChave){
        if(mPtEsq == NULL){
            throw runtime_error("chave nao encontrada");
        }
        return mPtEsq->Valor_Recursivo(chave);
    }
    if(chave > mChave){
        if(mPtDir == NULL){
            throw runtime_error("chave nao encontrada");
        }
        return mPtDir->Valor_Recursivo(chave);
    }
    return mValor;
}
 
// === Classe AVL =================================================================================
AVL::AVL() {
    mPtRaiz = NULL;
}
 
AVL::~AVL() {
    delete mPtRaiz;
}
 
// Escreve o conteúdo da árvore nível a nível, na saída de dados informada.
// Usado para conferir se a estrutra da árvore está correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nó especial para marcar fim de um nível
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
 
// Insere um par chave/valor na árvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
    NohAVL* novo = new NohAVL(chave, valor);
    if(mPtRaiz == NULL)
        mPtRaiz = novo;
    else
        mPtRaiz = mPtRaiz->InserirRecursivo(novo);
}
 
TValor AVL::Valor(TChave chave){
    if (mPtRaiz == NULL){
        throw runtime_error("Tentativa de buscar valor numa arvore vazia.");
    }
    return mPtRaiz->Valor_Recursivo(chave);
}
 
void AVL::Remover(TChave chave){
    if(mPtRaiz == NULL){
        throw runtime_error("Tentativa de Remover em arvore vazia");
    }else if (mPtRaiz->mChave == chave){
        mPtRaiz = mPtRaiz->Deletar();
    }else{
        mPtRaiz = mPtRaiz->Remover_Recursivo(chave);
    }
}
 
NohAVL* NohAVL::Menor_Recursivo(){
    NohAVL* menor = this;
    while(menor->mPtEsq != NULL){
        menor = menor->mPtEsq;
    }
    return menor;
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
