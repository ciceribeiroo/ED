//NÃO TENHO CERTEZA SE ESTÁ TUDO CERTO
#include <iostream>
#include <queue>
 
typedef int TChave;
 
class Noh {
    friend class Arvore23;
    friend std::ostream& operator<<(std::ostream& saida, Noh* ptNoh);
    protected:
        // Métodos
        Noh(const TChave& chave, Noh* ptPai);
        ~Noh();
        void DesalocarRecursivo();
        void DividirSeNecessario(Noh** ptPtRaiz);
        void InserirLocal(const TChave& chave, Noh* ptFilho);
        void InserirRecursivo(const TChave& chave, Noh** ptPtRaiz);
        // Atributos
        TChave mChaves[3]; // 3 chaves só temporariamente
        Noh* mFilhos[4];   // 4 filhos só temporariamente
        Noh* mPtPai;
        short int mQtdChaves;
        int mID; // identificador, útil para debugar o programa
};
 
class Arvore23 {
    public:
        Arvore23();
        ~Arvore23();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(const TChave& chave);
    protected:
        Noh* mPtRaiz;
};
 
using namespace std;
 
// === Classe Noh ==============================================================
Noh::Noh(const TChave& chave, Noh* ptPai) {
    mChaves[0] = chave;
    mQtdChaves = 1;
    mFilhos[0] = NULL; // usamos o primeiro filho para identificar folha
    //adicionei essa parte
    for (int i = 1; i < 4; i++)
    {
        mFilhos[i] = NULL;
    }
    mPtPai = ptPai;
    static int contador = 1;
    mID = contador++;
}
 
Noh::~Noh() {
}
 
void Noh::DesalocarRecursivo() {
    if (mFilhos[0] != NULL)
        for (int i = 0; i <= mQtdChaves; ++i)
            mFilhos[i]->DesalocarRecursivo();
    delete this;
}
 
// Divide um nó se for necessário, subindo a chave do meio e repetindo o processo.
// Recebe um ponteiro para ponteiro para a raiz, de forma a permitir a mudança da raiz da árvore
// quando necessário.
void Noh::DividirSeNecessario(Noh** ptPtRaiz) {//-----------------------certo?
    //~ #warning Noh::DividirSeNecessario não foi implementado.
    // Se o nó está acima do limite de chaves:
    // -> Criar novo nó com a chave da direita e seus filhos.
    // -> Se o nó tem pai, recomeçar nele; senão, criar um novo pai e atualizar a raiz.
    if(mQtdChaves > 2){
        //NOTA: pode ocorrer de mPtPai ser NULL
        //mQtdChaves-1 == 2 (terceira chave que ocasionou a necessidade de divisão)
        //divisão direita
        Noh* novo = new Noh(mChaves[mQtdChaves-1], mPtPai);
        if(mFilhos[2] != NULL){
            novo->mFilhos[0] = mFilhos[2];
            novo->mFilhos[0]->mPtPai = novo;
            mFilhos[2] = NULL;
        }
        if(mFilhos[3] != NULL){
            novo->mFilhos[1] = mFilhos[3];
            novo->mFilhos[1]->mPtPai = novo;
            mFilhos[3] = NULL;
        }
        mQtdChaves = 1;//esse noh precisa ficar com apenas 1 chave, de qualquer jeito
        if(mPtPai != NULL){
            mPtPai->InserirLocal(mChaves[1], novo);
            mPtPai->DividirSeNecessario(ptPtRaiz);
        }else{
            Noh* novo_pai = new Noh(mChaves[1], NULL);
            mPtPai = novo_pai;
            novo->mPtPai = novo_pai;
            novo_pai->mFilhos[0] = this;
            novo_pai->mFilhos[1] = novo;
            *ptPtRaiz = novo_pai;
        }
    }
}
 
// Adiciona uma chave e um filho ao nó, na posição certa dos vetores.
void Noh::InserirLocal(const TChave& chave, Noh* ptFilho) {
    //~ #warning Noh::InserirLocal não foi implementado.
    // Deslocar chaves e filhos à direita até abrir espaço para a nova chave e filho.
    // Atualizar a quantidade de chaves no nó.
    int pos = mQtdChaves-1;
    //~ while(pos > 0 && mChaves[pos] > chave){
        //~ pos--;
        //~ if(i > pos){
            //~ mChaves[i] = mChaves[i-1];
            //~ i--;
        //~ }
    //~ }
    //NOTA: Se pos for 2, significa que não deve deslocar as chaves, certo(?)
    for (int i = 2; i > pos; i--)
    {
        if(pos >= 0 && mChaves[pos] > chave){
            pos--;
        }
        mChaves[i] = mChaves[i-1];
        mFilhos[i+1] = mFilhos[i];
    }
   
    mChaves[pos+1] = chave;
    mFilhos[pos+2] = ptFilho;
    mQtdChaves++;
}
 
// Desce recursivamente até achar uma folha para inserção. Faz as arrumações necessárias.
void Noh::InserirRecursivo(const TChave& chave, Noh** ptPtRaiz) {//-----certo?
    //~ #warning Noh::InserirRecursivo não foi implementado.
    // Se o nó é folha, inserir aqui mesmo e fazer ajustes necessários.
    // Senão, recomeçar a inserção no filho apropriado.
    int pos = mQtdChaves;//pos é o numero da primeira posição vazia de mChaves
    if(mFilhos[0] == NULL){//se é folha
        while(pos > 0 && mChaves[pos-1] > chave){
            mChaves[pos] = mChaves[pos-1];
            pos--;
        }
        mChaves[pos] = chave;
        mQtdChaves++;
    }else{
        while(pos > 0 && mChaves[pos-1] > chave){
            pos--;
        }
        mFilhos[pos]->InserirRecursivo(chave, ptPtRaiz);
    }
    cout << mQtdChaves << endl;
    DividirSeNecessario(ptPtRaiz);
    cout << mQtdChaves << endl;
}
 
// Escreve um nó no formato [ID|chaves|pai;filhos]
ostream& operator<<(ostream& saida, Noh* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else {
        saida << '[' << ptNoh->mID << "|" << ptNoh->mChaves[0];
        for (int i = 1; i < ptNoh->mQtdChaves; ++i)
            saida << ',' << ptNoh->mChaves[i];
        int pai = (ptNoh->mPtPai == NULL) ? 0 : ptNoh->mPtPai->mID;
        saida << "|" << pai << ';';
        if (ptNoh->mFilhos[0] != NULL) {
            saida << ptNoh->mFilhos[0]->mID;
            for (int i = 1; i <= ptNoh->mQtdChaves; ++i)
                saida << ',' << ptNoh->mFilhos[i]->mID;
        }
        saida << ']';
    }
    return saida;
}
 
// === Classe Arvore23 =========================================================
Arvore23::Arvore23() : mPtRaiz(NULL) {
}
 
Arvore23::~Arvore23() {
    if (mPtRaiz != NULL)
        mPtRaiz->DesalocarRecursivo();
}
 
// Escreve a árvore nível a nível para facilitar a depuração.
void Arvore23::EscreverNivelANivel(ostream& saida) {
    if (mPtRaiz == NULL)
        cout << "[]\n";
    else {
        queue<Noh*> filhos;
        filhos.push(mPtRaiz);
        while (not filhos.empty()) {
            unsigned nroNohsNesteNivel = unsigned(filhos.size());
            for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
                Noh* ptNoh = filhos.front();
                filhos.pop();
                saida << ptNoh << ' ';
                if (ptNoh->mFilhos[0] != NULL)
                    for (short int i = 0; i <= ptNoh->mQtdChaves; ++i)
                        filhos.push(ptNoh->mFilhos[i]);
            }
            cout << "\n";
        }
    }
}
 
// Insere uma chave na árvore.
void Arvore23::Inserir(const TChave& chave) {
    if (mPtRaiz == NULL){
        mPtRaiz = new Noh(chave, NULL);
    }else{
        mPtRaiz->InserirRecursivo(chave, &mPtRaiz);
    }
}
 
// === Programa ================================================================
int main() {
    Arvore23 arvore;
    char opcao;
    TChave chave;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave;
                arvore.Inserir(chave);
                break;
            case 'e': // Escrever nós nível a nível
                arvore.EscreverNivelANivel(cout);
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
