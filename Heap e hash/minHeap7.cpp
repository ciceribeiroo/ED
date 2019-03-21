#include <iostream>
#include <algorithm> 
using namespace std;

void redimensiona(){

}

class MinHeap
{
	private:
		int* dados;
		int tam;
		int cap;
		int tamInicial;
	public:
		MinHeap( int* vet);
		~MinHeap();
		void corrigeDescendo( int i );
        void insere(int valor);
        void corrigeSubindo(int i);
        void imprime();
        void HeapSort();
};
void MinHeap:: HeapSort() {
   int i = tam / 2, pai, filho, t;
   while(true) {
      if (i > 0) {
          i--;
          t = dados[i];
      } else {
          tam--;
          if (tam == 0) return;
          t = dados[tam];
          dados[tam] = dados[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < tam) {
          if ((filho + 1 < tam)  &&  (dados[filho + 1] < dados[filho]))
              filho++;
          if (dados[filho] < t) {
			dados[pai] = dados[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      dados[pai] = t;
   }
}
MinHeap::MinHeap( int* vetor){
	tam = 15;
	cap = 20;
	tamInicial = tam;
	dados= new int[cap];
	for (int i = 0; i < tam; i++){
		dados[i] = vetor[i];
	}
	for ( int i = (tam/2) - 1; i >= 0; i-- ){
		corrigeDescendo(i);
	}
}
MinHeap::~MinHeap(){
	delete(dados);
}

void MinHeap:: corrigeDescendo(int i){
	int esq = 2*i+1;
	int dir = 2*i+2;
	int menor = i;
	if (( esq < tam) && (dados[esq] > dados[i])){
		menor = esq;
	}
	if (( dir < tam) && ( dados[dir] > dados[menor] )){
		menor = dir;
	}
	if (menor != i){
		swap(dados[i], dados[menor]);
		corrigeDescendo(menor);
	}
}
void MinHeap::insere(int valor){
		if ( tam > cap ){
			cout << "ERRO" << endl;
		}
		else{
			dados[tam] = valor;
			corrigeSubindo(tam);
			tam++;
		}
		tamInicial= tam;
	}

void MinHeap:: corrigeSubindo(int i){
    int p =(i - 1)/2;
    if (dados[i]>dados[p]){
        swap(dados[i], dados[p]);
        corrigeSubindo(p);
    }
}
void MinHeap::imprime(){
	for ( int j = 0; j < tamInicial; j++ ){
		cout << dados[j] << " ";
	}
}

int main()
{
    int* vet = new int [15];
    for ( int i = 0 ; i < 15; i++ ){
		cin >> vet[i];
	}
	MinHeap *minh = new MinHeap ( vet );
	minh-> imprime();
	delete(vet);
	cout << endl;
    int auxi;
    for ( int i = 0; i < 5; i++ ){
        cin >> auxi;
        minh->insere(auxi);
    }
	minh->imprime();
	cout << endl;
	minh->HeapSort();
	minh->imprime();
	
	delete(minh);
	
    return 0;
}
