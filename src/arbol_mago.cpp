#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Mago {
    int id;
    string name;
    string last_name;
    char gender; // 'H' o 'M'
    int age;
    int id_father;
    bool is_dead;
    string type_magic; // elemental, unique, mixed, no_magic
    bool is_owner;
    Mago* left;  // primer discípulo
    Mago* right; // segundo discípulo
    Mago* father; // apuntador al padre para facilitar búsquedas

    Mago(int _id, string _name, string _last_name, char _gender, int _age, int _id_father,
         bool _is_dead, string _type_magic, bool _is_owner)
        : id(_id), name(_name), last_name(_last_name), gender(_gender), age(_age),
          id_father(_id_father), is_dead(_is_dead), type_magic(_type_magic), is_owner(_is_owner),
          left(nullptr), right(nullptr), father(nullptr) {}
};

class ArbolMagos {
private:
    Mago* root;

    Mago* buscarMago(Mago* nodo, int id) {
        if (nodo == nullptr) return nullptr;
        if (nodo->id == id) return nodo;
        Mago* encontrado = buscarMago(nodo->left, id);
        if (encontrado) return encontrado;
        return buscarMago(nodo->right, id);
    }

    void insertarDiscipulo(Mago* padre, Mago* discipulo) {
        if (padre->left == nullptr) {
            padre->left = discipulo;
        } else if (padre->right == nullptr) {
            padre->right = discipulo;
        } else {
            cout << "El mago con id " << padre->id << " ya tiene dos discipulos." << endl;
        }
        discipulo->father = padre;
    }

    void mostrarSucesion(Mago* nodo) {
        if (nodo == nullptr) return;
        if (!nodo->is_dead) {
            cout << nodo->name << " " << nodo->last_name << " (Edad:" << nodo->age << ") ID:" << nodo->id << " Magia:" << nodo->type_magic << (nodo->is_owner ? " [Dueno]" : "") << "\n";
        }
        mostrarSucesion(nodo->left);
        mostrarSucesion(nodo->right);
    }

    Mago* buscarDuenio(Mago* nodo) {
        if (!nodo) return nullptr;
        if (nodo->is_owner) return nodo;
        Mago* l = buscarDuenio(nodo->left);
        if (l) return l;
        return buscarDuenio(nodo->right);
    }

    // Función para buscar discípulo vivo con alguna de las magias dadas en arreglo
    Mago* buscarDiscipuloConMagia(Mago* nodo, const string magias[], int n) {
        if (!nodo) return nullptr;
        if (!nodo->is_dead) {
            for (int i = 0; i < n; i++) {
                if (nodo->type_magic == magias[i]) return nodo;
            }
        }
        Mago* res = buscarDiscipuloConMagia(nodo->left, magias, n);
        if (res) return res;
        return buscarDiscipuloConMagia(nodo->right, magias, n);
    }

    // Buscar primer hombre vivo
    Mago* buscarPrimerHombreVivo(Mago* nodo) {
        if (!nodo) return nullptr;
        if (!nodo->is_dead && nodo->gender == 'H') return nodo;
        Mago* res = buscarPrimerHombreVivo(nodo->left);
        if (res) return res;
        return buscarPrimerHombreVivo(nodo->right);
    }

    // Buscar compañero discípulo (hermano)
    Mago* buscarCompanero(Mago* mago) {
        if (!mago || !mago->father) return nullptr;
        Mago* padre = mago->father;
        if (padre->left && padre->left != mago) return padre->left;
        if (padre->right && padre->right != mago) return padre->right;
        return nullptr;
    }

    // Verifica si dos magos comparten la misma magia
    bool compartenMagia(Mago* a, Mago* b) {
        if (!a || !b) return false;
        return a->type_magic == b->type_magic;
    }

    // Busqueda para la regla sobre primero elemental/unique, luego mixed, luego primer hombre vivo
    Mago* buscarDiscipuloCondicion(Mago* nodo) {
        if (!nodo) return nullptr;
        const string magiasPrimarias[] = {"elemental", "unique"};
        Mago* res = buscarDiscipuloConMagia(nodo->left, magiasPrimarias, 2);
        if (res) return res;
        res = buscarDiscipuloConMagia(nodo->right, magiasPrimarias, 2);
        if (res) return res;
        const string magiasMixed[] = {"mixed"};
        res = buscarDiscipuloConMagia(nodo->left, magiasMixed, 1);
        if (res) return res;
        res = buscarDiscipuloConMagia(nodo->right, magiasMixed, 1);
        if (res) return res;
        res = buscarPrimerHombreVivo(nodo->left);
        if (res) return res;
        return buscarPrimerHombreVivo(nodo->right);
    }

    Mago* buscarEnArbolCompanero(Mago* companero) {
        if (!companero) return nullptr;
        Mago* nuevo = buscarDiscipuloCondicion(companero);
        if (nuevo) return nuevo;
        return nullptr;
    }

    Mago* companeroDeMaestro(Mago* mago) {
        if (!mago || !mago->father || !mago->father->father) return nullptr;
        Mago* maestro = mago->father;
        Mago* abuelo = maestro->father;
        if (abuelo->left && abuelo->left != maestro) return abuelo->left;
        if (abuelo->right && abuelo->right != maestro) return abuelo->right;
        return nullptr;
    }

    void buscarMujerJovenConDiscipulosYNecesaria(Mago* nodo, Mago*& candidata, int& edadCandidata) {
        if (!nodo) return;
        if (nodo->gender == 'M' && !nodo->is_dead && (nodo->left || nodo->right)) {
            Mago* maestro = nodo->father;
            if (maestro && maestro->is_owner && maestro->type_magic == "mixed") {
                if (nodo->age < edadCandidata) {
                    candidata = nodo;
                    edadCandidata = nodo->age;
                }
            }
        }
        buscarMujerJovenConDiscipulosYNecesaria(nodo->left, candidata, edadCandidata);
        buscarMujerJovenConDiscipulosYNecesaria(nodo->right, candidata, edadCandidata);
    }

    void buscarMujerJoven(Mago* nodo, Mago*& candidata, int& edadCandidata) {
        if (!nodo) return;
        if (nodo->gender == 'M' && !nodo->is_dead) {
            if (nodo->age < edadCandidata) {
                candidata = nodo;
                edadCandidata = nodo->age;
            }
        }
        buscarMujerJoven(nodo->left, candidata, edadCandidata);
        buscarMujerJoven(nodo->right, candidata, edadCandidata);
    }

    // FUNCIONES RECURSIVAS PARA LA REGLA DE MAGIA IGUAL o MAS VIEJO

    Mago* buscarDiscipuloMagia(Mago* nodo, const string& magia) {
        if (!nodo) return nullptr;
        if (!nodo->is_dead && nodo->type_magic == magia) return nodo;
        Mago* res = buscarDiscipuloMagia(nodo->left, magia);
        if (res) return res;
        return buscarDiscipuloMagia(nodo->right, magia);
    }

    void buscarMasViejo(Mago* nodo, Mago*& masViejo, int& edadMax) {
        if (!nodo) return;
        if (!nodo->is_dead && nodo->age > edadMax) {
            masViejo = nodo;
            edadMax = nodo->age;
        }
        buscarMasViejo(nodo->left, masViejo, edadMax);
        buscarMasViejo(nodo->right, masViejo, edadMax);
    }
