#include <iostream>
#include <exception>
#include <limits>

typedef int TChave;

class Torneio {
    friend std::ostream& operator << (std::ostream& saida, const Torneio& h);
    public:
        // Construtor sem valores
        Torneio(int tam);
        // Construtor com valores
        Torneio(TChave vet[], int tam);
        ~Torneio();
        void Inserir(TChave chave);
        // Retorna o vencedor do torneio (maior) sem removê-lo.
        TChave Raiz();
        TChave Retirar();
        bool Vazio();
    protected:
        // Atualiza uma posição, com base em seus dois filhos.
        void Atualizar(int i);
        // Atualiza um toneiro subindo a partir da posicao dada. Util para inserção.
        void ArrumarSubindo(int i);
        // Atualiza todos os nós intermediários do torneio.
        void ArrumarTudo();
        // Calcula o espaço necessário e inicializa atributos. Usado nos contrutores.
        void CalcularEspacoDeArmazenamento(int tam);
        // Calcula a posição do filho direito de um nó.
        inline int Direito(int i);
        // Calcula a posição do filho esquerdo de um nó.
        inline int Esquerdo(int i);
        // Calcula a posição do pai de um nó.
        inline int Pai(int i);

        TChave* mVetChaves; // espaço de armazenamento
        int mCapacidade; // quantidade máxima de nós
        int mTamanho; // quantidade de nós utilizados
        int mNroTorneios; // quantidade de nós intermediários
        TChave mMenor; // valor especial que indica ausência de valor
};

using namespace std;

Torneio::Torneio(int tam) {
    CalcularEspacoDeArmazenamento(tam);
    mMenor = numeric_limits<TChave>::min();
    mTamanho = mNroTorneios;
    for (int i = 0; i < mCapacidade; ++i)
        mVetChaves[i] = mMenor;
}

Torneio::Torneio(TChave vet[], int tam) {
    CalcularEspacoDeArmazenamento(tam);
    mMenor = numeric_limits<TChave>::min();
    mTamanho = mNroTorneios + tam;
    int i;
    // copiar dados do vetor para o torneio
    for (i = 0; i < tam; ++i) {
        mVetChaves[mNroTorneios+i] = vet[i];
    }
    // posicoes extras ganham a menor chave para completar nós intermediarios
    i += mNroTorneios;
    while (i < mCapacidade)
        mVetChaves[i++] = mMenor;
    ArrumarTudo();
}

Torneio::~Torneio() {
    delete[] mVetChaves;
}

void Torneio::Atualizar(int i) {
    int dir = Direito(i);
    int esq = Esquerdo(i);
    int posMaior;
    if (mVetChaves[dir] > mVetChaves[esq]){
        posMaior = dir;
	}else{
        posMaior = esq;
	}
    mVetChaves[i] = mVetChaves[posMaior];
}

void Torneio::ArrumarSubindo(int i) {
    bool terminou = false;
    while (terminou == false) {
        int p = Pai(i);
        Atualizar(p);
        if (i == 0){
            terminou = true;
		}
        else {
            i = p;
		}
    }
}

void Torneio::ArrumarTudo() {
    int i = mNroTorneios;
    while (i < mCapacidade) {
        ArrumarSubindo(i);
        i += 2;
    }
}

void Torneio::CalcularEspacoDeArmazenamento(int tam) {
    mNroTorneios = 1;
    while (mNroTorneios < tam)
        mNroTorneios *= 2;
    mCapacidade = 2 * mNroTorneios - 1;
    mNroTorneios -= 1;
    mVetChaves = new TChave[mCapacidade];
}

int Torneio::Pai(int i) {
    return (i-1)/2;
}

int Torneio::Esquerdo(int i) {
    return 2*i+1;
}

int Torneio::Direito(int i) {
    return 2*i+2;
}

TChave Torneio::Raiz() {
    if (mTamanho == mNroTorneios){
        throw runtime_error("Impossível acessar raiz de torneio vazio.");
	}
    return mVetChaves[0];
}

TChave Torneio::Retirar() {
    if (mTamanho == mNroTorneios){
        throw runtime_error("Impossível retirar de torneio vazio.");
	}
    TChave maior = mVetChaves[0];
    int pos = 0;
    for (int i = mNroTorneios; i < mCapacidade; i++) {
        if (mVetChaves[i] == maior) {
            mVetChaves[i] = mMenor;
            pos = i;
        }
    }
    ArrumarSubindo(pos);
    mTamanho--;
    return maior;
}

bool Torneio::Vazio() {
    return mTamanho == mNroTorneios;
}

void Torneio::Inserir(TChave chave) {
    if (mTamanho == mCapacidade){
        throw runtime_error("Impossível inserir em torneio cheio.");
	}
    int pos = mTamanho;
    int i = mNroTorneios;
    bool achou = false;
    while ((achou == false) && (i < mTamanho)) {
        if (mVetChaves[i] == mMenor) {
            pos = i;
            achou = true;
        }
        i++;
    }
    mVetChaves[pos] = chave;
    ArrumarSubindo(pos);
    mTamanho++;
}

std::ostream& operator << (std::ostream& saida, const Torneio& t) {
    // Operador de escrita pode ser util para depurar o torneio.
    saida << '[';
    TChave chave;
    for (int i=0; i < t.mNroTorneios; ++i) {
        chave = t.mVetChaves[i];
        if (chave == t.mMenor)
            saida << "## ";
        else
            saida << chave << ' ';
    }
    saida << '|';
    int pos = t.mNroTorneios;
    int limite = t.mTamanho;
    while (pos < limite) {
        chave = t.mVetChaves[pos];
        if (chave == t.mMenor) {
            saida << " ##";
            ++limite;
        }
        else
            saida << ' ' << chave;
        ++pos;
    }
    saida << ']';
    return saida;
}

int main() {
    int capacidade; // nro de valores que quero guardar no torneio
    cin >> capacidade;
    Torneio torneio(capacidade);
    TChave chave;
    char operacao;
    do {
        cin >> operacao;
        switch (operacao) {
            case 'i': // Inserir
                try {
                    cin >> chave;
                    torneio.Inserir(chave);
                }
                catch (runtime_error& e) {
                    cerr << e.what() << endl;
                }
                break;
            case 'r': // Remover
                try {
                    cout << torneio.Retirar() << endl;
                }
                catch (runtime_error& e) {
                    cerr << e.what() << endl;
                }
                break;
            case 'e': // Escrever
                cout << torneio << endl;
                break;
        }
    } while (operacao != 'f'); // Finalizar execução
    return 0;
}
