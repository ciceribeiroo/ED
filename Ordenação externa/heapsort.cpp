#include <iostream>
#include <algorithm> 
using namespace std;
void HeapSort(int a[], int n) {
   int i = n / 2, pai, filho, t;
   while(true) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n == 0) return;
          t = a[n];
          a[n] = a[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < n) {
          if ((filho + 1 < n)  &&  (a[filho + 1] > a[filho]))
              filho++;
          if (a[filho] > t) {
             a[pai] = a[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      a[pai] = t;
   }
}
int main()
{
	int tam = 10;
   int vetor[tam] = {4,1,3,2,16,9,10,14,8,7};
	HeapSort(vetor, tam);
	for ( int i = 0; i < tam; i++ )
	{
		cout << vetor[i] << " ";
	}
    return 0;
}
