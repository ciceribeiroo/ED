#include <iostream>

using namespace std;

// constantes para verificação
const int MAXITENS = 3;
const int MINITENS = 1;
const int MAXFILHOS = 4;
const int METADE = 1;
const int MEIO = 1; 

typedef float Dado;

class Noh {
friend class Arvore234;
private:
    bool folha;
    int num; // número de itens armazenadas no nó
    Dado itens[MAXITENS];
    Noh* filhos[MAXFILHOS];
    Noh* pai;
public:
    Noh();
    ~Noh();
};

Noh::Noh() {
    num = 0;
    folha = false;
    for (int i = 0; i < MAXFILHOS; i++) {
        filhos[i] = NULL;
    }
}

Noh::~Noh() {
    for (int i = 0; i < num+1; i++) {
        delete filhos[i];
    }
}

class Arvore234 {
private:
    Noh* raiz;
    void percorreEmOrdemAux(Noh* atual, int nivel);   
public:
    Arvore234() { raiz = NULL; }
    ~Arvore234() { delete raiz; }
    // código não implementado, sua tarefa é implementá-lo!
    void insere(Dado umItem);
    Noh* insereAux(Noh* umNoh, Dado umItem, Dado& promovido);
    void insereEmNohFolhaNaoCheio(Noh* umNoh, Dado umItem);
    void insereEmNohIntermediarioNaoCheio(Noh* umNoh, Noh* novo, Dado& promovido);
    Noh* divideNoh(Noh* umNoh, Dado& promovido);
    // percorrimento (impressão)
    void percorreEmOrdem();
};

void Arvore234::insere(Dado umItem) {
    if(raiz==NULL){
        raiz = new Noh();
        raiz->folha = true;
        raiz->itens[0] = umItem;
        raiz->num = 1;
    }else{
		Dado promovido = 0;
        Noh *novo = insereAux(raiz, umItem, promovido);
        if (novo != NULL){
            Noh* antiga=raiz;
            raiz = new Noh();
            raiz->itens[0]=promovido;
            raiz->num=1;
            raiz->filhos[0] = antiga;
            raiz->filhos[1]= novo;
        }
    }
}

Noh* Arvore234::insereAux(Noh* umNoh, Dado umItem, Dado& promovido){
    if(umNoh->folha){
        if(umNoh->num < MAXITENS){
            insereEmNohFolhaNaoCheio(umNoh, umItem);
            promovido = 0;
            return NULL;
        }else{
            Noh* novo = divideNoh(umNoh, promovido);
            if(umItem <= umNoh->itens[MEIO]){
                insereEmNohFolhaNaoCheio(umNoh, umItem);
            }else{
                insereEmNohFolhaNaoCheio(novo, umItem);
            }
            return novo;
        }
    }else{
        int i = umNoh->num -1;
        while((i >=0)&&(umNoh->itens[i]>umItem)){
            i--;
        }
        Noh *Nohaux = insereAux(umNoh->filhos[i+1], umItem, promovido);
        if(Nohaux != NULL){
            if(umNoh->num < MAXITENS){
                insereEmNohIntermediarioNaoCheio(umNoh,Nohaux,promovido);
                return NULL;
            }else{
                Dado provfilho = promovido;
                Noh* novo = divideNoh(umNoh, promovido);
               if(umItem <= umNoh->itens[MEIO]){
                   insereEmNohIntermediarioNaoCheio(umNoh, Nohaux, provfilho);
               }else{
                   insereEmNohIntermediarioNaoCheio(umNoh, Nohaux, provfilho);
               }
                return novo;
            }
        }return NULL;
    }
}

void Arvore234::insereEmNohFolhaNaoCheio(Noh* umNoh, Dado umItem){
    int pos = umNoh->num - 1;
    while(pos>=0 && umNoh->itens[pos] > umItem){
        umNoh->itens[pos+1] = umNoh->itens[pos];
        pos--;
    }
    umNoh->itens[pos+1] = umItem;
    umNoh->num++;
}

void Arvore234::insereEmNohIntermediarioNaoCheio(Noh* umNoh, Noh* novo, Dado& promovido){
    int pos = umNoh->num - 1;
    while(pos>=0 && umNoh->itens[pos] > promovido){
        umNoh->itens[pos+1]= umNoh->itens[pos];
        umNoh->filhos[pos+2] = umNoh->filhos[pos+1];
        pos--;
    }
    umNoh->itens[pos+1]=promovido;
    umNoh->filhos[pos+2]= novo;
    umNoh->num++;
}

Noh* Arvore234::divideNoh(Noh* umNoh, Dado& promovido){
    promovido = umNoh->itens[MEIO];
    Noh* novo = new Noh();
    novo->folha = umNoh->folha;
    novo->itens[0] = umNoh->itens[MEIO+1];
    novo->num = METADE;
    umNoh->num = METADE;
    if(not(umNoh->folha)){
        for(int i=0; i <= MEIO+1;i++){
            novo->filhos[i]=umNoh->filhos[MEIO+1+i];
        }
    }
    return novo;
}

void Arvore234::percorreEmOrdem() {
    percorreEmOrdemAux(raiz,0);        
    cout<< endl;
}

void Arvore234::percorreEmOrdemAux(Noh* umNoh, int nivel) {
    int i;
    for (i = 0; i < umNoh->num; i++) {
        // se nó não é folha, imprima os dados do filho i
        // antes de imprimir o item i
        if (not umNoh->folha) {
            percorreEmOrdemAux(umNoh->filhos[i], nivel+1); 
        }
        cout << umNoh->itens[i] << '/' << nivel << ' ';
    }
 
    // Imprima os dados do último filho
    if (not umNoh->folha) {
        percorreEmOrdemAux(umNoh->filhos[i], nivel+1);   
    }
}


// =========================== Programa ================================
int main() {
    Arvore234* B = new Arvore234;
    
    char opcao;
    float valor;
    
    do {
        cin >> opcao;
        if (opcao == 'i') {
            cin >> valor;
            B->insere(valor);
        } else if(opcao == 'e') {
            B->percorreEmOrdem();
        }
    } while(opcao != 'q');
    
    
    delete B;    
    
    return 0;
}
