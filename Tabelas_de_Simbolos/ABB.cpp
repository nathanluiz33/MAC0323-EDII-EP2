#include <bits/stdc++.h>
using namespace std;

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class ABB{
    private:
    class Node{
        public:
        Node *esq, *dir, *pai;
        Key key;
        Item val;
        int peso;

        Node(){
            esq = dir = pai = NULL;
            key = Key();
            val = Item();
            peso = 0;
        }

        Node(Node* _pai, Key _key, Item _val){
            key = _key;
            val = _val;
            esq = dir = NULL;
            pai = _pai;
            peso = 1;
        }

        void print(){
            dbg(key);
            dbg(val);
            dbg(peso);
            cout << "\n";
        }
    }; 
    Node *root;

    public:

    ABB(){
        root = new Node();
    }

    bool isEmpty(){
        return root->peso == 0;
    }

    void balancear_peso(Node *raiz){
        int peso_novo = 1;
        if(raiz->esq != NULL) peso_novo += raiz->esq->peso;
        if(raiz->dir != NULL) peso_novo += raiz->dir->peso;
        raiz->peso = peso_novo;
    }

    void add(Node *raiz, Key _key, Item _val){
        if(raiz->key == _key){
            raiz->val = _val;
        }
        else if(_key < raiz->key){
            // vamos para esquerda
            if(raiz->esq == NULL) raiz->esq = new Node(raiz, _key, _val);
            add(raiz->esq, _key, _val);
        }
        else{
            // vamos para direita
            if(raiz->dir == NULL) raiz->dir = new Node(raiz, _key, _val);
            add(raiz->dir, _key, _val);
        }

        balancear_peso(raiz);
    }

    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key = _key;
            root->val = _val;
            root->peso = 1;
        }
        else add(root, _key, _val);
    }

    Item value(Node *raiz, Key _key){
        if(raiz->key == _key) return raiz->val;
        if(_key < raiz->key){
            // vamos para esquerda
            if(raiz->esq != NULL) return value(raiz->esq, _key);
            else return Item();
        }
        else{
            // vamos para direita
            if(raiz->dir != NULL) return value(raiz->dir, _key);
            else return Item();
        }
    }

    Item value(Key _key){
        if(root->peso == 0) return Item();
        return value(root, _key);
    }

    int rank(Node *raiz, Key _key){
        if(raiz == NULL) return 0;
        if(raiz->key < _key){
            int quant = 1;
            if(raiz->esq != NULL) quant += raiz->esq->peso;
            return quant + rank(raiz->dir, _key);
        }
        else return rank(raiz->esq, _key);
    }

    int rank(Key _key){
        return rank(root, _key);
    }

    Key select(Node *raiz, int k){
        int peso_esq = 0;
        if(raiz->esq != NULL) peso_esq = raiz->esq->peso;
        if(k == peso_esq) return raiz->key;
        
        if(raiz->esq == NULL) return select(raiz->dir, k - 1);

        if(raiz->esq->peso - 1 >= k) return select(raiz->esq, k);
        else return select(raiz->dir, k - 1 - raiz->esq->peso);
    }


    Key select(int k){
        if(k < 0 || k >= root->peso) return Key();
        return select(root, k);
    }

    void print(Node *raiz){
        if(raiz == NULL) return;

        raiz->print();
        print(raiz->esq);
        print(raiz->dir);
    }

    void print(){
        print(root);
    }
};

#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds;

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

int n;

signed main(){
    cin >> n;

    ABB<int, int> abb;
    map<int, int> M;
    ordered_set S;
    for(int i = 1; i <= n; i++){
        // dbg(i);
        int x; cin >> x;

        S.insert(x);
        M[x]++;
        
        int ant = abb.value(x);
        if(ant == 0) ant = 0;
        abb.add(x, ant + 1);

        int pos = abb.rank(x);
        assert(abb.select(pos) == *S.find_by_order(pos));
        assert(M[x] == abb.value(x));
        assert(abb.rank(x) == S.order_of_key(x));
    }

    for(auto u : S){
        // cout << abb.value(u) << " ";
        assert(M[u] == abb.value(u));
        assert(abb.rank(u) == S.order_of_key(u));
    }
    dbg("Okay!");
}