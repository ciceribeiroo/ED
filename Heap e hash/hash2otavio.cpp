// Tabela Hash com tratamento de colisÃ£o por endereÃ§amento aberto
#include <iostream>
#include <exception>

typedef int TValor; // tipo do valor armazenado

class InfoHash {
    friend class Hash; // essa classe sÃ³ deve ser usada na classe Hash
    InfoHash();
    InfoHash(const std::string& chave, const TValor& valor);
    std::string mChave;
    TValor mValor;
};

class Hash {
    public:
        Hash(unsigned capacidade = 50);
        ~Hash();
        // Mostra todos as posiÃ§Ãµes de armazenamento da hash.
        void EscreverEstrutura(std::ostream& saida) const;
        // Insere uma cÃ³pia do valor. NÃ£o permite inserÃ§Ã£o de chave repetida.
        void Inserir(const std::string& chave, const TValor& valor);
        // Remove um item da hash associado com a chave dada.
        void Remover(const std::string& chave);
        // Retorna o valor associado a uma chave.
        TValor Valor(const std::string& chave) const;
    protected:
        // Retorna a posicao em que uma chave estÃ¡ armazenada na estrutura.
        unsigned Buscar(const std::string& chave) const;
        // Retorna a posicao em que uma chave deveria ficar na estrutura.
        unsigned Posicao(const std::string& chave) const;

        InfoHash** mVetPtDados;
        InfoHash* REMOVIDO; // ponteiro especial a ser usado para marcar posiÃ§Ãµes de elemento removido
        unsigned mCapacidade;
        unsigned mTamanho;
};

using namespace std;

InfoHash::InfoHash()
    : mChave(), mValor() {
}

InfoHash::InfoHash(const std::string& chave, const TValor& valor)
    : mChave(chave), mValor(valor) {
}

Hash::Hash(unsigned capacidade) // capacidade tem valor default
    : mVetPtDados(new InfoHash*[capacidade]), REMOVIDO(new InfoHash()), mCapacidade(capacidade), mTamanho(0) {
    // FALTA FAZER:
    // inicializar todas as posiÃ§Ãµes de armazenamento com NULL indicando posiÃ§Ã£o VAZIA
    for (unsigned i = 0; i < capacidade; i++){
        mVetPtDados[i] = NULL;
    }
}

Hash::~Hash() {
    // FALTA FAZER:
    // desalocar memÃ³ria de cada item (InfoHash) armazenado
    // desalocar o ponteiro especial REMOVIDO
    // desalocar o vetor de ponteiros
    for (unsigned i = 0; i < mTamanho; i++){
        mVetPtDados[i] = NULL;
    }
    delete REMOVIDO;
    delete[] mVetPtDados;
}

unsigned Hash::Buscar(const std::string& chave) const {
    // Retorna a posicao em que uma chave estÃ¡ armazenada na estrutura. Protegido.
    unsigned pos = Posicao(chave);
    unsigned cont = 0;
    bool encontrado = false;
    while(cont < mCapacidade and encontrado == false){
        if((mVetPtDados[pos] != NULL) and (mVetPtDados[pos] != REMOVIDO)){
            if (mVetPtDados[pos]->mChave == chave){
                encontrado = true;
            } else {
                if (pos+1 == mCapacidade){
                pos = 0;
                } else {
                    pos++;
                }   
                cont++;
            }
        } else {
            if (pos+1 == mCapacidade){
                pos = 0;
            } else {
                pos++;
            }
            cont++;
        }
    }
    if ((mVetPtDados[pos] != NULL) and (mVetPtDados[pos]->mChave == chave)){
        return pos;
    } else {
        throw invalid_argument("chave nao encontrada");
    }
}

void Hash::EscreverEstrutura(std::ostream& saida) const {
    // Mostra todos as posiÃ§Ãµes de armazenamento da hash. Ajuda a debugar.
    for (unsigned i = 0; i < mCapacidade; ++i) {
        saida << '[' << i;
        if (mVetPtDados[i] != NULL) {
            if (mVetPtDados[i] == REMOVIDO)
                saida << "/removido";
            else
                saida << '/' << mVetPtDados[i]->mChave << '/' << mVetPtDados[i]->mValor;
        }
        saida << "] ";
    }
}

void Hash::Inserir(const string& chave, const TValor& valor) {
    if (mTamanho < mCapacidade){
        InfoHash* novo = new InfoHash(chave,valor);
        int pos = Posicao(chave);
        while (mVetPtDados[pos] != NULL and mVetPtDados[pos]->mChave != chave and mVetPtDados[pos] != REMOVIDO){
            pos++;
            pos %= mCapacidade;
        }
        if(mVetPtDados[pos] == NULL or mVetPtDados[pos] == REMOVIDO) {
            mTamanho++; 
            mVetPtDados[pos] = novo; 
        } else {
            throw invalid_argument("chave repetida");
        }
    } else {
        throw runtime_error("tabela hash lotada");
    }
}

unsigned Hash::Posicao(const string& chave) const {
    // Retorna a posiÃ§Ã£o de armazenamento de uma chave, 0 <= posicao < mCapacidade. Protegido.
    unsigned pos = 1;
    unsigned tamanho = unsigned(chave.length());
    for (unsigned i = 0; i < tamanho; ++i)
        pos = 7 * pos + chave[i];
    return pos % mCapacidade;
}

void Hash::Remover(const std::string& chave) {
    // Remove um item da hash associado com a chave dada.
    unsigned pos = Buscar(chave);
    delete mVetPtDados[pos];
    mVetPtDados[pos] = REMOVIDO;
    mTamanho--;
}

TValor Hash::Valor(const std::string& chave) const {
    // Retorna o valor associado a uma chave.
    unsigned pos = Buscar(chave);
    return mVetPtDados[pos]->mValor;
}

int main() {
    unsigned capacidade;
    cin >> capacidade;
    Hash tabela(capacidade);
    char operacao;
    string chave;
    TValor valor;
    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // inserir
                    cin >> chave >> valor;
                    tabela.Inserir(chave, valor);
                    break;
                case 'r': // remover
                    cin >> chave;
                    tabela.Remover(chave);
                    break;
                case 'c': // consultar
                    cin >> chave;
                    valor = tabela.Valor(chave);
                    cout << valor << endl;
                    break;
                case 'd': // debug (mostrar estrutura)
                    tabela.EscreverEstrutura(cout);
                    cout << endl;
                    break;
                case 'f': // finalizar
                    // vai testar depois
                    break;
                default:
                    cerr << "operaÃ§Ã£o invÃ¡lida\n";
            }
        }
        catch (exception& e) {
            // ignorar a mensagem que explica o erro e sÃ³ escrever ERRO.
            cout << "ERRO" << endl;
        }
    } while (operacao != 'f');
    return 0;
}
