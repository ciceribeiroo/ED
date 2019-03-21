#include <iostream>

using namespace std;

// constantes para verificação
const int MAXITENS = 3;
const int MINITENS = 1;
const int MAXFILHOS = 4;

// posição do elemento do meio (o que sobe para página pai em caso de estouro
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
    Noh* insereAux(Noh* umNoh, Dado umItem, Noh* promovido);
    void insereEmNohFolhaNaoCheio(Noh* umNoh, Dado umItem);
    Noh* divideNoh(Noh* umNoh, Dado umItem, Noh* promovido);
    void insereEmNohIntermediarioNaoCheio(Noh* umNoh, Noh* novo, Noh* promovido);
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
		Noh* promovido = NULL;
        Noh *novo = insereAux(raiz, umItem, promovido);
        if (novo){
            Noh* antiga=raiz;
            raiz->itens[0]=promovido->itens[0];
            raiz->num=1;
            raiz->filhos[0] = antiga;
            raiz->filhos[1]= novo;
        }
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
Noh* Arvore234::divideNoh(Noh* umNoh, Dado umItem, Noh* promovido){
	cout << "divide noh" << endl;
   if(umNoh->num > 2){
	   cout << "if" << endl;
		Noh* novo = new Noh();
		cout << "novo" << endl;
		if (umNoh->filhos[2] != NULL){
			cout << "segundo if" << endl;
			promovido = umNoh->filhos[2];
			novo->filhos[0] = umNoh->filhos[2];
			novo->filhos[0]->pai = novo;
			umNoh->filhos[2] = NULL;
		}
		if(umNoh->filhos[3] != NULL){
			cout << "segundo if" << endl;
			promovido = umNoh->filhos[3];
			novo->filhos[1] = umNoh->filhos[3];
			novo->filhos[1]->pai = novo;
			umNoh->filhos[3] = NULL;
		}
		umNoh->num = 1;
		if(umNoh->pai != NULL){
			cout << "segundo if" << endl;
			insereAux( umNoh->pai,umNoh->itens[1], promovido);
			divideNoh(umNoh->pai, umItem, promovido);
		}else{
			promovido = umNoh->filhos[0];
			Noh* npai = new Noh();
			umNoh->pai = npai;
			novo->pai = npai;
			npai->filhos[0] = umNoh;
			npai->filhos[1] = novo;
			raiz = npai;
		}
	}
	if (promovido == 0) return NULL;
	else return promovido;
}
void Arvore234::insereEmNohIntermediarioNaoCheio(Noh* umNoh, Noh* novo, Noh* promovido){
    int pos = umNoh->num - 1;
    while(pos>=0 && umNoh->itens[pos] > promovido->itens[0]){
        umNoh->itens[pos+1]= umNoh->itens[pos];
        umNoh->filhos[pos+2] = umNoh->filhos[pos+1];
        pos--;
    }
    umNoh->itens[pos+1]=promovido->itens[0];
    umNoh->filhos[pos+2]= novo;
    umNoh->num++;
}
Noh* Arvore234::insereAux(Noh* umNoh, Dado umItem, Noh* promovido){
    if(umNoh->folha){
        if(umNoh->num < MAXITENS){
            insereEmNohFolhaNaoCheio(umNoh, umItem);
            promovido = NULL;
            return NULL;
        }else{
            Noh* novo = divideNoh(umNoh, umItem, promovido);
            cout << "voltou" << endl;
            if(umItem <= umNoh->itens[1]){
                insereEmNohFolhaNaoCheio(umNoh, umItem);
            }else{
                insereEmNohFolhaNaoCheio(novo, umItem);
            }
        }
    }else{
        int i = umNoh->num -1;
        while((i >=0)&&(umNoh->itens[i]>umItem)){
            i--;
        }
        Noh *Nohaux = insereAux(umNoh, umItem, promovido);
        if(Nohaux){
            if(umNoh->num < MAXITENS){
                insereEmNohIntermediarioNaoCheio(umNoh,Nohaux,promovido);
                return NULL;
            }else{
                Noh* provfilho = promovido;
                Noh* novo = divideNoh(umNoh, umItem, promovido);
                 cout << "voltou" << endl;
                insereEmNohIntermediarioNaoCheio(umNoh, Nohaux, provfilho);
                return novo;
            }
        }
    }return NULL;
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
