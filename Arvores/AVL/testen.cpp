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
        /*NÃO USEI AINDA, MAS PODERIA AJUDAR
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        */
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas têm altura 1
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

NohAVL::NohAVL(TChave c, const TValor& v) : mChave(c), mValor(v) {
    mPtEsq = NULL;
    mPtDir = NULL;
    mPtPai = NULL;
    mAltura = 1;
    //~ #warning NohAVL::NohAVL não implementado.
}

NohAVL::~NohAVL() {
    //o destrutor da AVL já manda deletar a raiz, então, a partir dela, só precisa deletar os filhos
    DesalocarFilhosRecursivo();
}

// Faz as rotações e ajustes necessários inclusive do nó pai. Atualiza a altura.
// Retorna o nó que ficar na posição dele após os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
    //inicialmente atualiza a altura do noh (para caso seja em remoção(?))
    AtualizarAltura();
    //checa o balanceamento do noh
    int balanceamento = FatorBalanceamento();
   
    if(balanceamento > 1 && mPtEsq->FatorBalanceamento() >= 0){//se fator de balanceamento do noh e de seu filho esquerdo são positivos
        return RotacionarDireita();
    }
    //2. Desbalanceamento Esquerda Direita
    if(balanceamento > 1 && mPtEsq->FatorBalanceamento() < 0){//se o fator de balanceamento do noh é positivo mas de seu filho é negativo
        mPtEsq = mPtEsq->RotacionarEsquerda();
        return RotacionarDireita();
    }
    //3. Desbalanceamento Direita Direita
    if(balanceamento < -1 && mPtDir->FatorBalanceamento() <= 0){//se fator de balanceamento do noh e de seu filho dirito são negativos
        return RotacionarEsquerda();
    }
    //4. Desbalanceamento Direita Esquerda
    if(balanceamento < -1 && mPtDir->FatorBalanceamento() > 0){//se o fator de balanceamento do noh é negativo mas de seu filho é positivo
        mPtDir = mPtDir->RotacionarDireita();
        return RotacionarEsquerda();
    }
    return this;
}

// Calcula e atualiza a altura de um nó.
void NohAVL::AtualizarAltura() {
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
    if(altArvEsq >= altArvDir){
        mAltura = altArvEsq + 1;
    }else{
        mAltura = altArvDir + 1;
    }
    //~ #warning NohAVL::AtualizarAltura não implementado.
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {//-----------------------------certo?
    //o delete chama o destrutor e o destrutor chama o DesalocarFilhosRecursivo
    if(mPtEsq != NULL){
        delete mPtEsq;
    }
    if(mPtDir != NULL){
        delete mPtDir;
    }
    //~ delete this; não precisa (?)
    //~ #warning NohAVL::DesalocarFilhosRecursivo não implementado.
}

// Calcula e retorna o fator de balanceamento do nó.
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

// Insere um nó numa subárvore.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {
    if(ptNoh->mChave < mChave){
        if(mPtEsq == NULL){
            mPtEsq = ptNoh;
            ptNoh->mPtPai = this;
        }else{
            mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
        }
    }else{//se ptNoh->mChave >= mChave
        if(mPtDir == NULL){
            mPtDir = ptNoh;
            ptNoh->mPtPai = this;
        }else{
            mPtDir = mPtDir->InserirRecursivo(ptNoh);
        }
    }
    return ArrumarBalanceamento();
}

// Rotaciona a subárvore à direita. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarDireita() {
    NohAVL* aux = mPtEsq;
    //armazena subárvore à direita de aux na direita da raiz atual(se aux virar a nova raiz, o filho direito dele precis ser
    //a antiga raiz, certo? então seu antigo filho direito precisa ir para algum lugar, caso ele exista. Esse lugar é na posição
    //de filho esquerdo da antiga raiz
    this->mPtEsq = aux->mPtDir;
    //atualiza o pai do noh que agora é filho esquerdo da antiga raiz, se ele existir
    if(aux->mPtDir != NULL){
        aux->mPtDir->mPtPai = this;
    }
    //faz a antiga raiz da subárvore como filho direito de aux;
    aux->mPtDir = this;
    this->mPtPai = aux;
    
    //atualiza alturas
    this->AtualizarAltura();
    aux->AtualizarAltura();
    
    //retorna a nova raiz da subárvore
    return aux;
}

// Rotaciona a subárvore à esquerda. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarEsquerda() {//--------------------------------certo?
    NohAVL* aux = this->mPtDir;
    this->mPtDir =  aux->mPtEsq;
    //atualiza o pai do noh que agora é filho direito da antiga raiz, se ele existir
    if(aux->mPtEsq != NULL){
        aux->mPtEsq->mPtPai = this;
    }
    //atualiza o pai de aux. Como ele é a nova raiz, seu pai passa a ser o pai da antiga raiz
    aux->mPtPai = this->mPtPai;
    if(this == mPtRaiz){
        mPtRaiz = aux;
    }else if(this == this->mPtPai->mPtEsq){//caso contrário, coloque aux no lugar da raiz atual
        this->mPtPai->mPtEsq = aux;
    }else{
        this->mPtPai->mPtDir = aux;
    }  
    */
    //faz a antiga raiz da subárvore como filho esquerdo de aux
    aux->mPtEsq = this;
    this->mPtPai = aux;
    
    //atualiza alturas
    this->AtualizarAltura();
    aux->AtualizarAltura();
    
    //retorna a nova raiz da subárvore
    return aux;
}
/*NÃO USEI AINDA
// Substitui um dos filhos por um novo nó.
void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    
    #warning NohAVL::TrocarFilho não implementado.
}
*/

// Escreve o conteúdo de um nó no formato [altura:chave/valor].
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
    //~ #warning AVL::AVL não implementado.
}

AVL::~AVL() {
    delete mPtRaiz;
    //~ #warning AVL::~AVL não implementado.
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
    if(mPtRaiz == NULL){
        mPtRaiz = novo;
    }else{
        mPtRaiz = mPtRaiz->InserirRecursivo(novo);
    }
    //~ #warning AVL::Inserir não implementado.
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
            case 'e': // Escrever nós nível a nível
                minhaArvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "Opção inválida\n";
        }
    } while (opcao != 'f');
    return 0;
}
