#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

struct Dado {
    int chave1;
    int chave2;
    float valor1;
    char campo1[8];
    char campo2[20];
};

using namespace std;

ostream& operator << (ostream& saida, const Dado& registro) {
    saida << registro.chave1 << ' ' << registro.chave2 << ' '
          << registro.valor1 << ' ' << registro.campo1 << ' ' << registro.campo2;
    return saida;
}

char MaiusculaAleatoria() {
    return char('A' + rand() % 26);
}

float RealAleatorio() {
    return float(rand()) / 1.0e10f * float(rand());
}

void PreencherCampo1(char campo[8]) {
    int i = 0;
    while (i < 3)
        campo[i++] = MaiusculaAleatoria();
    while (i < 7)
        campo[i++] = char('0' + rand() % 10);
    campo[7] = '\0';
}

void PreencherCampo2(char campo[20]) {
    static char caracteres[] = " abcdefghijklmnopqrstuvwxyz";
    campo[0] = MaiusculaAleatoria();
    for (int i = 1; i < 19; ++i)
        if (campo[i-1] == ' ')
            campo[i] = MaiusculaAleatoria();
        else
            campo[i] = caracteres[rand() % 28];
    campo[19] = '\0';
}

int main(int argc, char* argv[]) {
    bool erroChamada = false;
    string modoChamada;
    unsigned nroRegistros = 0;
    if (argc < 2)
        erroChamada = true;
    else {
        modoChamada = argv[1];
        if (modoChamada == "--gerar") {
            if (argc < 4)
                erroChamada = true;
            else {
                istringstream iss(argv[2]);
                if (not (iss >> nroRegistros))
                    erroChamada = true;
            }
        }
        else if (modoChamada != "-v")
            erroChamada = true;
    }
    if (erroChamada) {
        cerr << "Uso: " << argv[0] << " opÃ§Ãµes\n"
             << "OpÃ§Ãµes disponÃ­veis:\n"
             << "    --gerar n nome: gera n registros aleatÃ³rio no arquivo nome.\n"
             << "    -v nome: verifica os registros no arquivo nome estÃ£o ordenados.\n"
             << "Exemplo: " << argv[0] << " --gerar 600000 arquivo-original\n";
        return 1;
    }
    srand(unsigned(time(0)));

    Dado registro;
    if (modoChamada == "--gerar") {
        cout << "Gerando " << nroRegistros << " registros no arquivo " << argv[3] << ".\n";
        ofstream arquivo(argv[3]);
        for (unsigned i = 0; i < nroRegistros; ++i) {
            registro.chave1 = rand();
            registro.chave2 = rand();
            registro.valor1 = RealAleatorio();
            PreencherCampo1(registro.campo1);
            PreencherCampo2(registro.campo2);
            arquivo.write(reinterpret_cast<char*>(&registro), sizeof(Dado));
        }
    }
    else {
        // verificar se o arquivo estÃ¡ ordenado
        Dado r1, r2;
        Dado* ptAnterior = &r1;
        Dado* ptAtual = &r2;
        ifstream arquivo(argv[2]);
        arquivo.seekg(0, ios::end);
        nroRegistros = unsigned(arquivo.tellg() / sizeof(Dado));
        cout << "Verificando ordem de " << nroRegistros << " registros.\n";
        arquivo.seekg(0);
        arquivo.read(reinterpret_cast<char*>(ptAnterior), sizeof(Dado));
        for (unsigned i = 1; i < nroRegistros; ++i) {
            arquivo.read(reinterpret_cast<char*>(ptAtual), sizeof(Dado));
            // requer a existencia do campo "chave1"
            if (ptAnterior->chave1 > ptAtual->chave1) {
                cerr << "Erro de ordenaÃ§Ã£o na posiÃ§Ã£o " << i << endl;
                return 0;
            }
        }
        cout << "Os registros estÃ£o em ordem.\n";
    }
    return 0;
}
