#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Estrutura:
//lista(Nohlista)
//Noharvore(lista(Nohlista))
//arvore(Noharvore(lista(Nohlista)))

typedef string Palavra;

class Nohlista {
    friend class lista;
    public:
        Nohlista (int valor);
    protected:
        int pos;
        Nohlista* prox;
};

class lista {
    friend class Nohlista;
    public:
        lista();
        void inserenofim(int dado);//insere a posicao na lista do noh
        void imprime();
    protected:
        Nohlista* inicio;
        Nohlista* fim;
};
class Noharvore {
    friend class ABB;
    public:
        Noharvore (Palavra d, int pos );
    protected:
        lista posicoes; //o noh tem a lista de posicoes
        Palavra chave; //e a palavra correspondente
        Noharvore* esq;
        Noharvore* dir;
};
class ABB {
    public:
        ABB() { raiz = NULL; }
        ~ABB();
        void insere(Palavra palavra, int cont); //insere na arvore ou na lista
        void BuscaAux(Palavra d);//achar noh e imprimir lista
        void destruirrecursivo(Noharvore* noh); //destrutor fofo
    protected:
        Noharvore* raiz;
};

using namespace std;
// === Classe Noh ==============================================================
Nohlista::Nohlista(int valor) {
    prox = NULL;
    pos = valor;
}
Noharvore::Noharvore(Palavra d, int cont){ // d tem valor default 
        chave = d;
        esq = NULL;
        dir = NULL;
        posicoes.inserenofim(cont);//controi a lista
}

// === Classe lista ==============================================================
//metodos normais, sem nehuma alteracao
lista::lista() {
    inicio = NULL;
}

void lista::inserenofim(int dado) {
    Nohlista* novo = new Nohlista(dado);
    if(inicio == NULL){
        inicio = novo;
    } else {
       fim->prox = novo;
    }
    fim = novo;
}

void lista::imprime() {
    Nohlista* aux = inicio;
    while (aux != NULL) {
        cout << aux->pos << " ";
        aux = aux->prox;
    }
}
// === Classe ABB ================================================================
ABB::~ABB(){
    destruirrecursivo(raiz);
}
void ABB::destruirrecursivo(Noharvore* noh){
    if (noh != NULL){
        destruirrecursivo(noh->esq);
        destruirrecursivo(noh->dir);
        delete(noh);
    }
}
void ABB::insere(Palavra palavra, int pos) {
    Noharvore* novo = new Noharvore(palavra, pos);
    if (raiz == NULL) { //se estiver vazia, adiciona na raiz
        raiz = novo;
    } else {
        Noharvore* atual = raiz;
        Noharvore* anterior;
        while (atual != NULL && atual->chave != palavra) { //procura a posicao na arvore
            anterior = atual;
            if (atual->chave > palavra) {
                atual = atual->esq;
            } else {
                atual = atual->dir;
            }
        }
        if (atual != NULL && palavra == atual->chave) { //se ja achou a palavra na arvore
            atual->posicoes.inserenofim(pos);//adiciona a pos na lista
            delete(novo);//e deleta o noh
        } else { //se nao, continua ate achar o adicionar um novo
            if(palavra > anterior->chave){
                anterior->dir = novo;
            }else{
                anterior->esq = novo;
            }
        }
    }
}

void ABB::BuscaAux(Palavra d){
    Noharvore* atual = raiz; //busca normal
    while(atual != NULL and atual->chave != d){
        if(atual ->chave > d){
            atual = atual -> esq;
        } else {
            atual = atual -> dir;
        }
    }
    if(atual == NULL){ //se nao achar
        cout << "-1" << endl;
    }else{ // se achar, imprime a lista correspondente ao noh
        atual->posicoes.imprime();
    }
}

// === Programa ================================================================
int main() {
   Palavra palavra;
   ifstream arq("entrada.txt"); //abre o arquivo
   int pos = 1;
   ABB arvore;
   if(arq.is_open()){ //verifica se ta aberto certo
       while ( arq >> palavra ){ //enquanto nao chegar no final do arquivo
           arvore.insere(palavra, pos);//adiciona na arvore
           pos++; //incrementa a posicao pra passar pro proximo
       }
       arq.close();
   }
   cin >> palavra;
   arvore.BuscaAux(palavra); //procur a posicao e retorna a lista
	return 0;
}
