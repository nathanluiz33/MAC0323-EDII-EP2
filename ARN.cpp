#include <bits/stdc++.h>
using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int mod = 1e9 + 7;

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class ARN{
    private:
    class Node{
        public:
        Node *esq, *dir, *pai;
        Key key;
        Item val;
        // cor: 1->preto, 0->vermelho
        int peso, cor;

        Node(){
            esq = dir = pai = NULL;
            key = Key();
            val = Item();
            peso = cor = 0;
        }

        Node(Node* _pai, Key _key, Item _val){
            key = _key;
            val = _val;
            esq = dir = NULL;
            pai = _pai;
            peso = 1;
            cor = 0;
        }

        void print(){
            dbg(key);
            dbg(val);
            dbg(peso);
            dbg(cor);
            cout << "\n";
        }
    }; 
    
    Node *root, *NULO;

    public:
    ARN(){
        root = new Node();
        NULO = new Node();
        root->pai = NULO;
        NULO->cor = 1;
    }

    bool isEmpty(){
        return root->peso == 0;
    }

    void balancear_peso(Node *raiz){
        int peso_novo = 1;
        if(raiz->esq != NULO) peso_novo += raiz->esq->peso;
        if(raiz->dir != NULO) peso_novo += raiz->dir->peso;
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
        if(C->esq != NULO)
            C->esq->pai = A;
        C->pai = A->pai;

        if(A->pai != NULO){
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
        // return A;
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
        if(B->dir != NULO)
            B->dir->pai = A;
        B->pai = A->pai;

        if(A->pai != NULO){
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
            if(raiz->esq == NULO){
                raiz->esq = new Node(raiz, _key, _val);
                raiz->esq->esq = raiz->esq->dir = NULO;
            }
            add(raiz->esq, _key, _val);
        }
        else{
            // vamos para direita
            if(raiz->dir == NULO){
                raiz->dir = new Node(raiz, _key, _val);
                raiz->dir->esq = raiz->dir->dir = NULO;
            }
            add(raiz->dir, _key, _val);
        }

        // temos que preservar as propriedades da árvore rubro-negra
        
        Node* p = raiz->pai;

        if(raiz->cor == 0 and p->cor == 0){
            // se o nó atual e o pai forem vermelhos
            // temos que garantir que root e NULO sao sempre pretos!
            assert(p->pai != NULO and p != NULO);

            if(p == p->pai->esq){
                Node* y = p->pai->dir;
                // se y for vermelho, podemos pintar p e y de pretos, e p->pai vermelho
                if(y->cor == 0){
                    p->cor = 1;
                    y->cor = 1;
                    p->pai->cor = 0;
                }
                else{
                    if(raiz == p->dir){
                        // devemos rotacionar para esquerda
                        raiz = p;
                        rotate_left(raiz);
                    }
                    raiz->pai->cor = 1;
                    raiz->pai->pai->cor = 0;
                    rotate_right(raiz->pai->pai);
                } 
            } 
            else{
                Node* y = p->pai->esq;
                if(y->cor == 0){
                    p->cor = 1;
                    y->cor = 1;
                    p->pai->cor = 0;
                }
                else{
                    if(raiz == p->esq){
                        raiz = p;
                        rotate_right(raiz);
                    }
                    raiz->pai->cor = 1;
                    raiz->pai->pai->cor = 0;
                    rotate_left(raiz->pai->pai);
                }
            }
        }
        balancear_peso(raiz);
    }

    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key = _key;
            root->val = _val;
            root->peso = 1;
            root->cor = 1;
            root->esq = root->dir = NULO;
        }
        else add(root, _key, _val);
        if(root->cor == 0) root->cor = 1;
        assert(root->cor == 1);
    }
    
    Item value(Node *raiz, Key _key){
        if(raiz->key == _key) return raiz->val;
        if(_key < raiz->key){
            // vamos para esquerda
            if(raiz->esq != NULO) return value(raiz->esq, _key);
            else return Item();
        }
        else{
            // vamos para direita
            if(raiz->dir != NULO) return value(raiz->dir, _key);
            else return Item();
        }
    }

    Item value(Key _key){
        if(root->peso == 0) return Item();
        return value(root, _key);
    }

    int rank(Node *raiz, Key _key){
        if(raiz == NULO) return 0;
        if(raiz->key < _key){
            int quant = 1;
            if(raiz->esq != NULO) quant += raiz->esq->peso;
            return quant + rank(raiz->dir, _key);
        }
        else return rank(raiz->esq, _key);
    }

    int rank(Key _key){
        return rank(root, _key);
    }

    Key select(Node *raiz, int k){
        int peso_esq = 0;
        if(raiz->esq != NULO) peso_esq = raiz->esq->peso;
        if(k == peso_esq) return raiz->key;
        
        if(raiz->esq == NULO) return select(raiz->dir, k - 1);

        if(raiz->esq->peso - 1 >= k) return select(raiz->esq, k);
        else return select(raiz->dir, k - 1 - raiz->esq->peso);
    }

    Key select(int k){
        if(k < 0 || k >= root->peso) return Key();
        return select(root, k);
    }

    void print(Node *raiz){
        if(raiz == NULO) return;

        raiz->print();
        assert(raiz->cor == 1 || raiz->pai->cor == 1);
        if(raiz->esq != NULO) assert(raiz->esq->key < raiz->key);
        if(raiz->dir != NULO) assert(raiz->dir->key > raiz->key);
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

    ARN<int, int> arn;
    map<int, int> M;
    ordered_set S;
    for(int i = 1; i <= n; i++){
        int x; cin >> x;

        // dbg(i);
        // dbg(x);
        S.insert(x);
        M[x]++;
        
        int ant = arn.value(x);
        if(ant == 0) ant = 0;
        arn.add(x, ant + 1);
        
        int pos = arn.rank(x);
        assert(arn.select(pos) == *S.find_by_order(pos));
        assert(M[x] == arn.value(x));
        assert(arn.rank(x) == S.order_of_key(x));
    }

    for(auto u : S){
        // cout << arn.value(u) << " ";
        assert(M[u] == arn.value(u));
        assert(arn.rank(u) == S.order_of_key(u));
    }
    dbg("Ökay!");
}