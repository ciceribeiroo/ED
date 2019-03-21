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
void Noh::DividirSeNecessario(Noh** ptPtRaiz) {
	cout << "dividir se necessario" << endl;
	if (mQtdChaves == 3){
		cout << "mQtdChaves == 3" << endl;
		mQtdChaves = 1;
		Noh* filho = new Noh(mChaves[2], mPtPai);
		cout << "cria noh filho" << endl;
		
		filho->mFilhos[0] = mFilhos[2];
		filho->mFilhos[1] = mFilhos[3];
		cout << "filhos" << endl;

		if(mFilhos[0] != NULL){
			cout << "se nao for o primeiro" << endl;
			mFilhos[2]->mPtPai = filho;
			cout << "liga pai e filho" << endl;
			mFilhos[3]->mPtPai = filho;
			cout << "liga pai e filho 2" << endl;
		}

		if (mPtPai != NULL) {
			cout << "se tiver pai" << endl;
			mPtPai->InserirLocal(mChaves[1], filho);
			cout << "volta insere local" << endl;
			mPtPai->DividirSeNecessario(ptPtRaiz);
			cout << "volta dividir" << endl;
		}
		else {
			cout << 
			Noh* novaRaiz = new Noh(mChaves[1], NULL);
			mPtPai = novaRaiz;
			filho->mPtPai = novaRaiz;
			novaRaiz->mFilhos[0] = this;
			novaRaiz->mFilhos[1] = filho;
			*ptPtRaiz = novaRaiz;
		}
	}
	
	//~ #warning Noh::DividirSeNecessario não foi implementado.
	//~ // Se o nó está acima do limite de chaves:
	//~ // -> Criar novo nó com a chave da direita e seus filhos.
	//~ // -> Se o nó tem pai, recomeçar nele; senão, criar um novo pai e atualizar a raiz.
}

// Adiciona uma chave e um filho ao nó, na posição certa dos vetores.
void Noh::InserirLocal(const TChave& chave, Noh* ptFilho) { //inserir auxiliar
	int i = 0;
	while (i < mQtdChaves and mChaves[i] < chave)
		i++;
	
	for (int j = mQtdChaves; j > i; j--)
		mChaves[j] = mChaves[j-1];
	
	mChaves[i] = chave;
	mQtdChaves++;
	if (mFilhos[0] != NULL){
		i++;
		for (int j = mQtdChaves+1; j > i; j--)
			mFilhos[j] = mFilhos[j-1];
		mFilhos[i] = ptFilho;
	}
	
	//~ #warning Noh::InserirLocal não foi implementado.
	//~ // Deslocar chaves e filhos à direita até abrir espaço para a nova chave e filho.
	//~ // Atualizar a quantidade de chaves no nó.
}

// Desce recursivamente até achar uma folha para inserção. Faz as arrumações necessárias.
void Noh::InserirRecursivo(const TChave& chave, Noh** ptPtRaiz) {
	if (mFilhos[0] == NULL){
		InserirLocal(chave, NULL);
		DividirSeNecessario(ptPtRaiz);
		cout << "volta no dividir necessario" << endl;
	}
	else {
		cout << "nao e o primeiro no noh" << endl;
		int i = 0;
		while (mChaves[i] < chave and i < mQtdChaves){
			cout << "while" << endl;
			i++;
		}
		cout << "saiu while" << endl;
		mFilhos[i]->InserirRecursivo(chave, ptPtRaiz);
		cout << "insere na posicao recursao" << endl;
	}
	
	//~ #warning Noh::InserirRecursivo não foi implementado.
	//~ // Se o nó é folha, inserir aqui mesmo e fazer ajustes necessários.
	//~ // Senão, recomeçar a inserção no filho apropriado.
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
	cout << "inserir arvore" << endl;
	if (mPtRaiz == NULL){
		cout << "na raiz" << endl;
		mPtRaiz = new Noh(chave, NULL);
		mPtRaiz->mQtdChaves = 1;
	}else{
		cout << "raiz esta preenchida" << endl;
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
