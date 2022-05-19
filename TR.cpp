#include <bits/stdc++.h>
using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int mod = 1e9 + 7;

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class Treap{
    private:
    class Node{
        public:
        Node *esq, *dir, *pai;
        Key key;
        Item val;
        int peso;
        long long prioridade;

        Node(){
            esq = dir = pai = NULL;
            key = Key();
            val = Item();
            peso = prioridade = 0;
        }

        Node(Node* _pai, Key _key, Item _val){
            key = _key;
            val = _val;
            esq = dir = NULL;
            pai = _pai;
            peso = 1;
            prioridade = rng() % mod;
        }

        void print(){
            dbg(key);
            dbg(val);
            dbg(peso);
            dbg(prioridade);
            cout << "\n";
        }
    }; 
    
    Node *root;

    public:
    Treap(){
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

    /*
        A               C
       / \       ->    / \
      B   C           A   y
         / \         / \
        x   y       B   x
    */
    void rotate_left(Node *A){
        Node* C = A->dir;
        A->dir = C->esq;
        if(C->esq != NULL)
            C->esq->pai = A;
        C->pai = A->pai;

        if(A->pai != NULL){
            if(A == A->pai->esq)
                A->pai->esq = C;
            else
                A->pai->dir = C;
        }
        else{
            root = C;
        }
        C->esq = A;
        A->pai = C;
        balancear_peso(A);
        balancear_peso(C);
    }

    /*
        A               B
       / \       ->    / \
      B   C           x   A
     / \                 / \
    x   y               y   C
    */
    void rotate_right(Node *A){
        Node* B = A->esq;
        A->esq = B->dir;
        if(B->dir != NULL)
            B->dir->pai = A;
        B->pai = A->pai;

        if(A->pai != NULL){
            if(A == A->pai->esq)
                A->pai->esq = B;
            else
                A->pai->dir = B;
        }
        else root = B;
        B->dir = A;
        A->pai = B;
        balancear_peso(A);
        balancear_peso(B);
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
        
        // temos que verificar se as prioridades estao corretas
        if(raiz->esq != NULL){
            if(raiz->esq->prioridade < raiz->prioridade)
                rotate_right(raiz);
        }
        if(raiz->dir != NULL){
            if(raiz->dir->prioridade < raiz->prioridade)
                rotate_left(raiz);
        }
        balancear_peso(raiz);
    }

    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key = _key;
            root->val = _val;
            root->peso = 1;
            root->prioridade = rng();
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
        if(raiz->esq != NULL) assert(raiz->esq->key < raiz->key);
        if(raiz->dir != NULL) assert(raiz->dir->key > raiz->key);
        cout << "ESQ\n";
        print(raiz->esq);
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

    Treap<int, int> treap;
    map<int, int> M;
    ordered_set S;
    for(int i = 1; i <= n; i++){
        int x; cin >> x;

        S.insert(x);
        M[x]++;
        
        int ant = treap.value(x);
        if(ant == 0) ant = 0;
        treap.add(x, ant + 1);
        
        int pos = treap.rank(x);
        assert(treap.select(pos) == *S.find_by_order(pos));
        assert(M[x] == treap.value(x));
        assert(treap.rank(x) == S.order_of_key(x));
    }

    for(auto u : S){
        // cout << treap.value(u) << " ";
        assert(M[u] == treap.value(u));
        assert(treap.rank(u) == S.order_of_key(u));
    }
    dbg("ÖKAY!");
}