#include <bits/stdc++.h>
using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class A23{
    private:
    class Node{
        public:
        // se dir == NULL, entao o no tem 1 elemento somente
        Node *esq, *dir, *meio, *pai;
        Key key_l, key_r;
        Item val_l, val_r;
        bool node_2;
        int peso;

        Node(){
            esq = dir = meio = pai = NULL;
            key_l = key_r = Key();
            val_l = val_r = Item();
            node_2 = 1;
            peso = 0;
        }

        Node(Node* _pai, Key _key, Item _val){
            key_l = _key;
            key_r = Key();
            val_l = _val;
            val_r = Item();
            esq = dir = meio = NULL;
            pai = _pai;
            peso = 1;
            node_2 = 1;
        }

        void print(){
            dbg(key_l);
            dbg(val_l);
            dbg(key_r);
            dbg(val_r);
            dbg(peso);
            cout << "\n";
        }
    }; 
    
    Node *root, *subiu;

    public:
    A23(){
        root = new Node();
        subiu = new Node();
    }

    bool isEmpty(){
        return root->peso == 0;
    }

    void balancear_peso(Node *raiz){
        int peso_novo = 1;
        if(!raiz->node_2) peso_novo++;
        if(raiz->esq != NULL) peso_novo += raiz->esq->peso;
        if(raiz->meio != NULL) peso_novo += raiz->meio->peso;
        // se o nó tem 2 elementos, devemos somar 1 no peso
        if(raiz->dir != NULL) peso_novo += raiz->dir->peso;
        raiz->peso = peso_novo;
    }

    void fix_node(Node *raiz, int tipo){
        // tipo 0 -> vem da esquerda
        if(tipo == 0){
            // se o nó estiver livre
            // dbg("tipo 0");
            if(raiz->node_2){
                raiz->key_r = raiz->key_l;
                raiz->val_r = raiz->val_l;
                raiz->dir = raiz->meio;

                raiz->meio = subiu->meio;
                raiz->esq = subiu->esq;
                raiz->key_l = subiu->key_l;
                raiz->val_l = subiu->val_l;
                raiz->node_2 = false;
                balancear_peso(raiz);
                subiu = NULL;
            }
            else{
                Node *esq = new Node(subiu, subiu->key_l, subiu->val_l), *dir = new Node(subiu, raiz->key_r, raiz->val_r);

                esq->esq = subiu->esq;
                if(esq->esq != NULL) esq->esq->pai = esq;
                esq->meio = subiu->meio;
                if(esq->meio != NULL) esq->meio->pai = esq;

                dir->esq = raiz->meio;
                if(dir->esq != NULL) dir->esq->pai = dir;
                dir->meio = raiz->dir;
                if(dir->meio != NULL) dir->meio->pai = dir;
                
                subiu->esq = esq;
                subiu->meio = dir;
                subiu->key_l = raiz->key_l;
                subiu->val_l = raiz->val_l;
                balancear_peso(esq);
                balancear_peso(dir);
                balancear_peso(subiu);
                // dbg("AQUI");
            }
        }
        // se vem do meio
        else if(tipo == 1){
            // dbg("tipo 1");
            if(raiz->node_2){
                raiz->meio = subiu->esq;
                raiz->dir = subiu->meio;
                raiz->key_r = subiu->key_l;
                raiz->val_r = subiu->val_l;
                raiz->node_2 = false;
                balancear_peso(raiz);
                subiu = NULL;
            }
            else{
                Node *esq = new Node(subiu, raiz->key_l, raiz->val_l), *dir = new Node(subiu, raiz->key_r, raiz->val_r);

                esq->esq = raiz->esq;
                if(esq->esq != NULL) esq->esq->pai = esq;
                esq->meio = subiu->esq;
                if(esq->meio != NULL) esq->meio->pai = esq;

                dir->esq = subiu->meio;
                if(dir->esq != NULL) dir->esq->pai = dir;
                dir->meio = raiz->dir;
                if(dir->meio != NULL) dir->meio->pai = dir;
                
                subiu->esq = esq;
                subiu->meio = dir;
                balancear_peso(esq);
                balancear_peso(dir);
                balancear_peso(subiu);
            }
        }
        // se vem de dir
        else{
            // dbg("tipo 2");
            assert(!raiz->node_2);
            Node *esq = new Node(subiu, raiz->key_l, raiz->val_l), *dir = new Node(subiu, subiu->key_l, subiu->val_l);

            esq->esq = raiz->esq;
            if(esq->esq != NULL) esq->esq->pai = esq;
            esq->meio = raiz->meio;
            if(esq->meio != NULL) esq->meio->pai = esq;

            dir->esq = subiu->esq;
            if(dir->esq != NULL) dir->esq->pai = dir;
            dir->meio = subiu->meio;
            if(dir->meio != NULL) dir->meio->pai = dir;
            
            subiu->esq = esq;
            subiu->meio = dir;
            subiu->key_l = raiz->key_r;
            subiu->val_l = raiz->val_r;
            balancear_peso(esq);
            balancear_peso(dir);
            balancear_peso(subiu);
        }
    }

    void add(Node *raiz, Key _key, Item _val){
        // dbg(raiz->key_l);
        // dbg(raiz->key_r);
        if(raiz->key_l == _key)
            raiz->val_l = _val;
        else if(!(raiz->node_2) && raiz->key_r == _key)
            raiz->val_r = _val;
        else if(_key < raiz->key_l){
            // vamos para esquerda
            if(raiz->esq == NULL){
                // se estiver cheio, devemos subir
                subiu = new Node(raiz->pai, _key, _val);
            }
            else{
                add(raiz->esq, _key, _val);
            }
            if(subiu != NULL)
                fix_node(raiz, 0);
        }
        else if(!(raiz->node_2) && raiz->key_r < _key){
            // vamos para direita
            if(raiz->dir == NULL)
                subiu = new Node(raiz->pai, _key, _val);
            else add(raiz->dir, _key, _val);
            // dbg("AQUI");
            if(subiu != NULL)
                fix_node(raiz, 2);
        }
        else{
            if(raiz->meio == NULL){
                subiu = new Node(raiz->pai, _key, _val);
            }
            else
                add(raiz->meio, _key, _val);
            if(subiu != NULL)
                fix_node(raiz, 1);
        }

        balancear_peso(raiz);
        // temos que preservar as propriedades da árvore 23
    }

    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key_l = _key;
            root->val_l = _val;
            root->peso = 1;
            root->esq = root->meio = NULL;
        }
        else{
            add(root, _key, _val);
            if(subiu != NULL){
                root = subiu;
                root->pai = NULL;
                subiu = NULL;
            }
            balancear_peso(root);
            // dbg(root->key_l);
        }
    }
    
    Item value(Node *raiz, Key _key){
        if(raiz->key_l == _key) return raiz->val_l;
        if(!raiz->node_2 && raiz->key_r == _key) return raiz->val_r;
        if(_key < raiz->key_l){
            // vamos para esquerda
            if(raiz->esq != NULL) return value(raiz->esq, _key);
            else return Item();
        }
        else if(raiz->key_r != Key() && raiz->key_r < _key) {
            // vamos para direita
            if(raiz->dir != NULL) return value(raiz->dir, _key);
            else return Item();
        }
        else{
            if(raiz->meio != NULL) return value(raiz->meio, _key);
            else return Item();
        }
    }

    Item value(Key _key){
        if(root->peso == 0) return Item();
        return value(root, _key);
    }

    int rank(Node *raiz, Key _key){
        if(raiz == NULL) return 0;
        if(_key <= raiz->key_l){
            return rank(raiz->esq, _key);
        }
        if((!raiz->node_2 && _key <= raiz->key_r) or raiz->node_2){
            int quant = 1;
            if(raiz->esq != NULL) quant += raiz->esq->peso;
            return quant + rank(raiz->meio, _key);
        }
        else{
            int quant = 2;
            if(raiz->esq != NULL) quant += raiz->esq->peso;
            if(raiz->meio != NULL) quant += raiz->meio->peso;
            return quant + rank(raiz->dir, _key);
        }
    }

    int rank(Key _key){
        return rank(root, _key);
    }

    Key select(Node *raiz, int k){
        if(raiz->esq == NULL){
            if(k == 0) return raiz->key_l;
            else return raiz->key_r;
        }
        if(k == raiz->esq->peso) return raiz->key_l;
        if(k == raiz->esq->peso + raiz->meio->peso + 1) return raiz->key_r;

        if(k < raiz->esq->peso) return select(raiz->esq, k);
        else if(k < raiz->meio->peso + raiz->esq->peso + 2) return select(raiz->meio, k - raiz->esq->peso - 1);
        else return select(raiz->dir, k - 2 - raiz->esq->peso - raiz->meio->peso);
    }

    Key select(int k){
        if(k < 0 || k >= root->peso) return Key();
        return select(root, k);
    }

    void print(Node *raiz){
        if(raiz == NULL) return;

        raiz->print();
        // if(raiz->esq != NULL) assert(raiz->esq->key < raiz->key);
        // if(raiz->dir != NULL) assert(raiz->dir->key > raiz->key);
        cout << "ESQ\n";
        print(raiz->esq);
        cout << "MEIO\n";
        print(raiz->meio);
        cout << "DIR\n";
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

    A23<int, int> a23;
    map<int, int> M;
    ordered_set S;
    for(int i = 1; i <= n; i++){
        int x; cin >> x;

        // dbg(i);
        // dbg(x);
        S.insert(x);
        M[x]++;
        
        int ant = a23.value(x);
        if(ant == 0) ant = 0;
        a23.add(x, ant + 1);
        
        // dbg("ACABOU");
        // a23.print();
        // dbg(a23.rank(x));
        // dbg((int) S.order_of_key(x));
        int pos = a23.rank(x);
        assert(a23.select(pos) == *S.find_by_order(pos));
        // dbg(1);
        assert(M[x] == a23.value(x));
        assert(a23.rank(x) == (int) S.order_of_key(x));
    }

    for(auto u : S){
        // cout << a23.value(u) << " ";
        assert(M[u] == a23.value(u));
        assert(a23.rank(u) == (int)S.order_of_key(u));
    }
    dbg("Ökay!"); 
} 