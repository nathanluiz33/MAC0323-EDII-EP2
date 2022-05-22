#include <bits/stdc++.h>
using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int mod = 1e9 + 7;

template<class Key, class Item>
class Treap{
    private:
    /*
     * Subclasse: Node 
     * -------------------
     * Essa subclasse guarda o tipo de nó da árvore. Os atributos 
     * Node *esq, *dir, *pai são ponteiros que apontam para o filho esquerdo, direito, e pai do nó
     * atual. Key key e Item val são a chave e item associados ao nó.
     * int Peso é o tamanho da subárvore do nó. É essencial para melhorar a complexidade de algumas funções.
     * long long prioridade é o valor associano ao nó. Nós com prioridades menor devem estar mais próximos da raiz.
     */
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
    }; 
    // Cada objeto tem sua raiz, que é uma variável global do objeto.
    Node *root;

    public:
    Treap(){
        root = new Node();
    }

    /*
     * Função: isEmpty 
     * Uso: isEmpty();
     * -------------------
     * Retorna um booleano se a tabela de símbolos está vazia. Complexidade O(1)
     */
    bool isEmpty(){
        return root->peso == 0;
    }

    /*
     * Função: balancear_peso 
     * Uso: balancear_peso(Node *raiz);
     * -------------------
     * Atualiza o peso do nó *raiz. É necessário que o peso dos filhos estejam corretos. Complexidade O(1)
     */
    void balancear_peso(Node *raiz){
        int peso_novo = 1;
        if(raiz->esq != NULL) peso_novo += raiz->esq->peso;
        if(raiz->dir != NULL) peso_novo += raiz->dir->peso;
        raiz->peso = peso_novo;
    }

    /*
     * Função rotate_left
     * Uso: rotate_left(Node *raiz);
     * --------------------
     * Rotaciona o nó para a esquerda, como na figura abaixo
     *   A               C
     *  / \       ->    / \
     * B   C           A   y
     *    / \         / \
     *   x   y       B   x
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
        // após a rotação, é preciso balancear os pesos
        balancear_peso(A);
        balancear_peso(C);
    }

    /*
     * Função rotate_right
     * Uso: rotate_right(Node *raiz);
     * --------------------
     * Rotaciona o nó para a direita, como na figura abaixo
     *     A               B
     *    / \       ->    / \
     *   B   C           x   A
     *  / \                 / \
     * x   y               y   C
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
        // após a rotação, é preciso balancear os pesos
        balancear_peso(A);
        balancear_peso(B);
    }
    
    /*
     * Função: add 
     * Uso: add(Node *raiz, Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) na subárvore de *raiz. Complexidade O(h), altura da árvore.
     */
    void add(Node *raiz, Key _key, Item _val){
        if(raiz->key == _key){ // Caso a chave esteja na árvore, apenas alterar o valor
            raiz->val = _val;
        }
        else if(_key < raiz->key){
            // Se _key for menor do que a chave do nó, então devemos ir para o filho esquerdo
            if(raiz->esq == NULL) raiz->esq = new Node(raiz, _key, _val);
            add(raiz->esq, _key, _val);
        }
        else{
            // Se _key for maior do que a chave do nó, então devemos ir para o filho direito
            if(raiz->dir == NULL) raiz->dir = new Node(raiz, _key, _val);
            add(raiz->dir, _key, _val);
        }
        
        // temos que verificar se as prioridades estao corretas
        // sendo a menor prioridade sempre em cima
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

    /*
     * Função: add 
     * Uso: add(Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) na tabela de símbolos. 
     * Basicamente, faz a chamada de add(root, _key, _val). Complexidade O(h).
     */
    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key = _key;
            root->val = _val;
            root->peso = 1;
            root->prioridade = rng();
        }
        else add(root, _key, _val);
    }

    /*
     * Função: value 
     * Uso: value(Node *raiz, Key _key);
     * -------------------
     * Procura o Item associado à chave _key na subárvore de *raiz. Caso não exista a chave _key,
     * retorna o construtor vazio Item(). Complexidade O(h).
     */
    Item value(Node *raiz, Key _key){
        if(raiz->key == _key) return raiz->val;
        if(_key < raiz->key){
            // se _key for menor do que a chave do nó, procura na subárvore esquerda
            if(raiz->esq != NULL) return value(raiz->esq, _key);
            else return Item();
        }
        else{
            // se _key for maior do que a chave do nó, procura na subárvore direita
            if(raiz->dir != NULL) return value(raiz->dir, _key);
            else return Item();
        }
    }

    /*
     * Função: value 
     * Uso: value(Key _key); 
     * -------------------
     * Procura o Item associado à chave _key na tabela de símbolos. Complexidade O(h).
     */
    Item value(Key _key){
        if(root->peso == 0) return Item();
        return value(root, _key);
    }

    /*
     * Função: rank 
     * Uso: rank(Node *raiz, Key _key);
     * -------------------
     * Retorna a quantidade de chaves estritamente menor que _key na subárvore de *raiz. Complexidade O(h)
     */
    int rank(Node *raiz, Key _key){
        if(raiz == NULL) return 0;
        if(raiz->key < _key){
            // Se _key for maior que a chave do nó, então todos os nós 
            // da subárvore esquerda são menor que key
            int quant = 1;
            if(raiz->esq != NULL) quant += raiz->esq->peso;
            return quant + rank(raiz->dir, _key);
        }
        // Caso contrário, devemos procurar na subárvore esquerda
        // porque toda a subárvore direita é maior que nó
        else return rank(raiz->esq, _key);
    }

    /*
     * Função: rank 
     * Uso: rank(Key _key);
     * -------------------
     * Retorna a quantidade de chaves estritamente menor que _key na tabela de símbolos. Complexidade O(h)
     */
    int rank(Key _key){
        return rank(root, _key);
    }

    /*
     * Função: select 
     * Uso: select(Node *raiz, Key _key);
     * -------------------
     * Retorna k-ésima chave da subávore de Node *raiz, sendo a menor chave a chave 0. Complexidade O(h).
     */
    Key select(Node *raiz, int k){
        int peso_esq = 0;
        if(raiz->esq != NULL) peso_esq = raiz->esq->peso;
        // caso tenhamos exatamente k - 1 chaves na subárvore esquerda,
        // então a resposta é o nó atual
        if(k == peso_esq) return raiz->key;

        // verificamos se não existe subárvore esquerda
        if(raiz->esq == NULL) return select(raiz->dir, k - 1);

        // vamos para a subárvore esquerda se ela tem mais de k elementos
        if(raiz->esq->peso - 1 >= k) return select(raiz->esq, k);
        // caso contrário, vamos para a subárvore direita
        else return select(raiz->dir, k - 1 - raiz->esq->peso);
    }


    /*
     * Função: select 
     * Uso: select(Key _key);
     * -------------------
     * Retorna k-ésima chave da tabela de símbolos, sendo a menor chave a chave 0. Complexidade O(h).
     */
    Key select(int k){
        if(k < 0 || k >= root->peso) return Key();
        return select(root, k);
    }
};