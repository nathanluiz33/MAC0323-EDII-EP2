#include <bits/stdc++.h>
using namespace std;

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class Vetor_Ordenado{
    private:
    vector< pair<Key, Item> > v;

    /*
     * Função: find_first_bigger 
     * Uso: find_first_bigger(Key key);
     * -------------------
     * Esta função acha o menor índice que contém uma chave menor ou igual
     * a Key key. Caso não tenha um elemento maior ou igual, retorna -1. 
     * É feita uma busca binária pois o vetor está sempre ordenado, portanto
     * funciona em O(logN).
     */
    int find_first_bigger(Key key){
        if(!v.size()) return -1;
        if(v.back().first < key) return -1;
        if(key <= v[0].first) return 0;

        int ini = 0, fim = (int) v.size() - 1;
        while(ini <= fim){
            int meio = (ini + fim) / 2;
            if(v[meio].first < key) ini = meio + 1;
            else fim = meio - 1;
        }
        return ini;
    }

    public:
    Vetor_Ordenado(){}

    /*
     * Função: isEmpty 
     * Uso: isEmpty(Key key);
     * -------------------
     * Retorna um booleano se a estrutura está vazia
     */
    bool isEmpty(){
        return !(v.size());
    }
    
    /*
     * Função: contais 
     * Uso: contais(Key key);
     * -------------------
     * Retorna um booleano se a tem a Key key na tabela de símbolos.
     * Funciona em O(logN).
     */
    bool contains(Key key){
        int k = find_first_bigger(key);
        if(k == -1) return false;
        return v[k].first == key;
    }

    /*
     * Função: add 
     * Uso: add(Key key, Item val);
     * -------------------
     * Adiciona o par (key, val) na estrutura. Se já tiver key na tabela,
     * a função acha o índice da chave e altera o valor - O(logN). Se não tiver,
     * adicina o par no final do vetor e usa swaps para deixar ordenado. No pior caso
     * funciona em O(N).
     */
    void add(Key key, Item val){
        if(!contains(key)){
            v.push_back(make_pair(key, val));
            for(int i = (int) v.size() - 2; i >= 0; i--){
                if(v[i].first < key) break;
                swap(v[i], v[i + 1]);
            }
        }
        else{
            int k = find_first_bigger(key);
            v[k].second = val;
        }
    }

    /*
     * Função: value 
     * Uso: value(Key key);
     * -------------------
     * Acha o Item associado à chave key. Caso a chave não esteja na tabela,
     * retorna o construtor vazio. Funciona em O(logN).
     */
    Item value(Key key){
        if(!contains(key)) return Item();
        int k = find_first_bigger(key);
        return v[k].second;
    }

    /*
     * Função: rank 
     * Uso: rank(Key key);
     * -------------------
     * Retorna a quantidade de chaves estritamente menor do que key. Utiliza busca
     * binária, portanto funciona em O(logN). 
     */
    int rank(Key key){
        if(!v.size() || key < v[0].first) return 0;
        if(v.back().first < key) return v.size();
        return find_first_bigger(key);
    }

    /*
     * Função: select 
     * Uso: select(int k);
     * -------------------
     * Retorna a chave na posição k. Caso não tenha nenhuma chave nesta posição,
     * retorna o construtor vazio da chave. Como basta acessar uma posição do vetor,
     * é em O(1).
     */
    Key select(int k){
        if(k < 0 or k >= (int) v.size()) return Key();
        return v[k].first;
    }
};
