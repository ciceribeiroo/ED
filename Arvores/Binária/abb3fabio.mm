#include <iostream>
#include <fstream>

using namespace std;

typedef string Dado;

class NohAux
{
    friend class Lista;
    private:
        NohAux* proximo;
        int valor;
    public:
        NohAux(int valor);
};

NohAux::NohAux(int v){
    //construtor listaNoh
    proximo = NULL;
    valor = v;
}

class Lista
{
    private:
        NohAux* mPrimeiro;
    public:
        Lista();
        ~Lista();
        void InsereLista(int v);
        void ImprimeLista();
};

Lista::Lista(){
    mPrimeiro = NULL;
}

Lista::~Lista(){
    NohAux* atual = mPrimeiro;
    NohAux* ant = NULL;
    while(atual != NULL){
        ant = atual;
        atual = atual -> proximo;
        delete ant;
    }
}

void Lista::InsereLista(int v){
    NohAux* novo = new NohAux(v);
    if(mPrimeiro == NULL){
        mPrimeiro = novo;
    } else {
        NohAux* atual = mPrimeiro;
        NohAux* ant = NULL;
        while(atual != NULL){
            ant = atual;
            atual = atual -> proximo;
        }
        ant -> proximo = novo;
        novo -> proximo = NULL;
    }
}

void Lista::ImprimeLista(){
    NohAux* atual = mPrimeiro;
    while(atual != NULL){
        cout << atual -> valor << " ";
        atual = atual -> proximo;
    }
}

class Noh
{
    friend class ArvoreABB;
    private:
        Noh* mEsquerdo;
        Noh* mDireito;
        Noh* mPai;
        Dado palavra;
        Lista conexao;
    public:
        Noh(Dado d);
};

Noh::Noh(Dado d){
    //Construtor
    mEsquerdo = NULL;
    mDireito = NULL;
    mPai = NULL;
    palavra = d;
}

class ArvoreABB
{
    private:
        Noh* mRaiz;
    public:
        ArvoreABB();
        ~ArvoreABB();
        void insere(Dado d);
        void inserePosi(int posi);
        void buscaAux(Dado d);
        void buscaInsere(Dado d, int posi);
        void D(){ destruct(mRaiz);}
        void destruct(Noh* atual);
};

ArvoreABB::ArvoreABB(){
    //Contructor
    mRaiz = NULL;
}

ArvoreABB::~ArvoreABB(){
    //Destructor
    D();
}

void ArvoreABB::insere(Dado d){
    //Insere interativamente
    Noh* novo = new Noh(d);
    if(mRaiz == NULL){
        mRaiz = novo;
    } else {
        Noh* atual = mRaiz;
        Noh* ant = NULL;
        
        while(atual != NULL){
            ant = atual;
            if(atual -> palavra > d){
                atual = atual -> mEsquerdo;
            } else {
                atual = atual -> mDireito;
            }
        }
        novo -> mPai = ant;
        if(ant -> palavra > novo -> palavra){
            ant -> mEsquerdo = novo;
        } else {
            ant -> mDireito = novo;
        }
    }
}

void ArvoreABB::buscaInsere(Dado d, int posi){
    //Faz a busca na estrutura e retorna
    Noh* atual = mRaiz;
    bool achei = false;
    while(atual != NULL and achei == false){
        if(atual -> palavra == d){
            achei = true;
            atual -> conexao.InsereLista(posi);
        } else if(atual -> palavra > d){
            atual = atual -> mEsquerdo;
        } else {
            atual = atual -> mDireito;
        }
    }
}

void ArvoreABB::destruct(Noh* atual){
    
    if(atual != NULL){
        destruct(atual->mEsquerdo);
        destruct(atual->mDireito);
        delete atual;
    }
}

void ArvoreABB::buscaAux(Dado d){
    //Faz a busca na estrutura e retorna
    Noh* atual = mRaiz;
    bool achei = false;
    while(atual != NULL and achei == false){
        if(atual -> palavra == d){
            achei = true;
            atual -> conexao.ImprimeLista();
        } else if(atual -> palavra > d){
            atual = atual -> mEsquerdo;
        } else {
            atual = atual -> mDireito;
        }
    }
    if(achei == false){
        cout << "-1" << endl;
    }
}

int main()
{
    ArvoreABB MyTree;
    ifstream ArquivoEnt("entrada.txt");
    int tam = 0;
    string palavra[100];
    
    while(ArquivoEnt >> palavra[tam++]){}
    
    int teste[tam];
    for (int i = 0; i < tam; i++){
        teste[i] = 1;
    }
    
    int i = 0;
    while(i < tam - 1){
        int j = i + 1;
        while(j < tam - 1){
            if(palavra[j] == palavra[i]){
                teste[j] = 0;
            }
            j++;
        }
        i++;
    }
    
    for (int i = 0; i < tam - 1; i++){
        if(teste[i] == 1){
            MyTree.insere(palavra[i]);
        }
    }
    string corrigido[100];
    int contaux = 0;
    for (int i = 0; i < tam - 1; i++){
        if(teste[i] == 1){
            corrigido[contaux] = palavra[i];
            contaux++;
        }
    }
    
    int contVez = 0;
    for (int i = 0; i < contaux; i++){
        contVez = 1;
        for (int j = 0; j < tam; j++){
            if(corrigido[i] == palavra[j]){
                MyTree.buscaInsere(palavra[i], contVez);
            }
            contVez++;
        }
    }
        
    Dado procura;
    cin >> procura;
    MyTree.buscaAux(procura);
    
	return 0;
}