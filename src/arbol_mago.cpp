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