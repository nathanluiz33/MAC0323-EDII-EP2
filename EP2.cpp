#include <bits/stdc++.h>
using namespace std;

#define dbg(x)  cout << #x << " = " << x << "\n"

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
const int mod = 1e9 + 7;

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
        if(k < 0 or k >= v.size()) return Key();
        return v[k].first;
    }
};


template<class Key, class Item>
class ABB{
    /*
     * Subclasse: Node 
     * -------------------
     * Essa subclasse guarda o tipo de nó da árvore. Os atributos 
     * Node *esq, *dir, *pai são ponteiros que apontam para o filho esquerdo, direito, e pai do nó
     * atual. Key key e Item val são a chave e item associados ao nó.
     * int Peso é o tamanho da subárvore do nó. É essencial para melhorar a complexidade de algumas funções.
     */
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
    }; 
    // Cada objeto tem sua raiz, que é uma variável global do objeto.
    Node *root;

    public:
    // construtor vazio
    ABB(){
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
            if(raiz->esq == NULL) raiz->esq = new Node(raiz, _key, _val); // Caso não tenha filho esquerdo, criamos um novo nó
            add(raiz->esq, _key, _val);
        }
        else{
            // Se _key for maior do que a chave do nó, então devemos ir para o filho direito
            if(raiz->dir == NULL) raiz->dir = new Node(raiz, _key, _val); // Caso não tenha filho esquerdo, criamos um novo nó
            add(raiz->dir, _key, _val);
        }

        // Após adicionar a chave, devemos balancear o peso de Node *raiz.
        balancear_peso(raiz);
    }

    /*
     * Função: add 
     * Uso: add(Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) no objeto ABB. 
     * Basicamente, faz a chamada de add(root, _key, _val). Complexidade O(h).
     */
    void add(Key _key, Item _val){
        if(root->peso == 0){
            // primeiro verificamos se o nosso objeto ainda não tem nenhuma chave.
            root->key = _key;
            root->val = _val;
            root->peso = 1;
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

template<class Key, class Item>
class ARN{
    private:
    /*
     * Subclasse: Node 
     * -------------------
     * Essa subclasse guarda o tipo de nó da árvore. Os atributos 
     * Node *esq, *dir, *pai são ponteiros que apontam para o filho esquerdo, direito, e pai do nó
     * atual. Key key e Item val são a chave e item associados ao nó.
     * int Peso é o tamanho da subárvore do nó. É essencial para melhorar a complexidade de algumas funções.
     * int cor é cor do nó. 1->preto, 0->vermelho.
     */
    class Node{
        public:
        Node *esq, *dir, *pai;
        Key key;
        Item val;
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
    }; 
    // Cada objeto tem sua raiz, que é uma variável global do objeto.
    // NULO é um nó de cor preta que representa o NULL
    Node *root, *NULO;

    public:
    ARN(){
        root = new Node();
        NULO = new Node();
        root->pai = NULO;
        NULO->cor = 1;
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
        if(raiz->esq != NULO) peso_novo += raiz->esq->peso;
        if(raiz->dir != NULO) peso_novo += raiz->dir->peso;
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
        // após a rotação, é preciso balancear os pesos
        balancear_peso(A);
        balancear_peso(B);
    }

    /*
     * Função: add 
     * Uso: add(Node *raiz, Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) na subárvore de *raiz. Complexidade O(logN), altura da árvore.
     */
    void add(Node *raiz, Key _key, Item _val){
        if(raiz->key == _key){
            raiz->val = _val;
        }
        else if(_key < raiz->key){
            // Se _key for menor do que a chave do nó, então devemos ir para o filho esquerdo
            if(raiz->esq == NULO){
                raiz->esq = new Node(raiz, _key, _val);
                // sempre que criamos um novo nó, devemos fazer seus
                // filhos serem NULO, a variável global
                raiz->esq->esq = raiz->esq->dir = NULO;
            }
            add(raiz->esq, _key, _val);
        }
        else{
            // Se _key for menor do que a chave do nó, então devemos ir para o filho esquerdo
            if(raiz->dir == NULO){
                raiz->dir = new Node(raiz, _key, _val);
                raiz->dir->esq = raiz->dir->dir = NULO;
            }
            add(raiz->dir, _key, _val);
        }

        // temos que preservar as propriedades da árvore rubro-negra, em que vértices de cor vermelha
        // só tem filhos de cor preta
        
        Node* p = raiz->pai;
        if(raiz->cor == 0 and p->cor == 0){
            // se o nó atual e o pai forem vermelhos
            assert(p->pai != NULO and p != NULO); // temos que garantir que root e NULO sao sempre pretos!

            // caso o pai seja filho esquerdo do avô 
            if(p == p->pai->esq){
                Node* y = p->pai->dir; // tio do nó atual
                // se y for vermelho, podemos pintar p e y de pretos, e p->pai vermelho
                if(y->cor == 0){
                    p->cor = 1;
                    y->cor = 1;
                    p->pai->cor = 0;
                }
                else{
                    if(raiz == p->dir){
                        // se o nó é filho direito do pai, devemos rotacionar para esquerda
                        raiz = p;
                        rotate_left(raiz);
                    }
                    // pintamos o pai de preto e o avo de vermelho
                    raiz->pai->cor = 1;
                    raiz->pai->pai->cor = 0;
                    // rotacinamos para direita
                    rotate_right(raiz->pai->pai);
                } 
            } 
            else{ // caso o pai seja filho direito do avô.
                Node* y = p->pai->esq; // tio do nó atual
                if(y->cor == 0){
                    // se y for vermelho, podemos pintar p e y de pretos, e p->pai vermelho
                    p->cor = 1;
                    y->cor = 1;
                    p->pai->cor = 0;
                }
                else{
                    if(raiz == p->esq){
                        // se o nó é filho esquerdo do pai, devemos rotacionar para direita
                        raiz = p;
                        rotate_right(raiz);
                    }
                    // pintamos o pai de preto e o avo de vermelho
                    raiz->pai->cor = 1;
                    raiz->pai->pai->cor = 0;
                    rotate_left(raiz->pai->pai);
                }
            }
        }
        balancear_peso(raiz);
    }

    /*
     * Função: add 
     * Uso: add(Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) na tabela de símbolos. 
     * Basicamente, faz a chamada de add(root, _key, _val). Complexidade O(logN).
     */
    void add(Key _key, Item _val){
        if(root->peso == 0){
            root->key = _key;
            root->val = _val;
            root->peso = 1;
            root->cor = 1;
            root->esq = root->dir = NULO;
        }
        else add(root, _key, _val);
        // utilizamos a convencao de a raiz ter sempre cor preta
        if(root->cor == 0) root->cor = 1;
        assert(root->cor == 1);
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
            if(raiz->esq != NULO) return value(raiz->esq, _key);
            else return Item();
        }
        else{
            // se _key for maior do que a chave do nó, procura na subárvore direita
            if(raiz->dir != NULO) return value(raiz->dir, _key);
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
        if(raiz == NULO) return 0;
        if(raiz->key < _key){
            // Se _key for maior que a chave do nó, então todos os nós 
            // da subárvore esquerda são menor que key
            int quant = 1;
            if(raiz->esq != NULO) quant += raiz->esq->peso;
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
        if(raiz->esq != NULO) peso_esq = raiz->esq->peso;
        // caso tenhamos exatamente k - 1 chaves na subárvore esquerda,
        // então a resposta é o nó atual
        if(k == peso_esq) return raiz->key;
        
        // verificamos se não existe subárvore esquerda
        if(raiz->esq == NULO) return select(raiz->dir, k - 1);

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

template<class Key, class Item>
class A23{
    private:
    /*
     * Subclasse: Node 
     * -------------------
     * Essa subclasse guarda o tipo de nó da árvore. Os atributos 
     * Node *esq, *meio, *dir, *pai são ponteiros que apontam para o filho esquerdo, meio, direito, e pai do nó
     * atual. Key key e Item val são a chave e item associados ao nó.
     * int Peso é o tamanho da subárvore do nó. É essencial para melhorar a complexidade de algumas funções.
     * bool node_2 é uma variável que identifica se o nó tem 2 ou 3 filhos. Caso tenha somente 2 filhos,
     * o ponteiro *dir será sempre NULL.
     */
    class Node{
        public:
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
    }; 
    // Cada objeto tem sua raiz, que é uma variável global do objeto.
    // O ponteiro subiu é uma variável que ajuda na inserção do elemento.
    // Quando um nó fica saturado, então ele sobe por esse ponteiro.
    Node *root, *subiu;

    public:
    A23(){
        root = new Node();
        subiu = new Node();
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
            }
        }
        // se vem do meio
        else if(tipo == 1){
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
};

int n, q;
string estrutura;
vector<string> texto;

signed main(){
    cin >> estrutura >> n;
    for(int i = 0; i < n; i++){
        string s; cin >> s;
        texto.push_back(s);
    }
    cin >> q;
    int pont = 0;
    if(estrutura == "VO"){
        Vetor_Ordenado<string, int> VO;
        for(int i = 1; i <= q; i++){
            int op; cin >> op;

            if(op == 1){
                int x; cin >> x;
                while(x){
                    int ant = VO.value(texto[pont]);
                    VO.add(texto[pont], ant + 1);
                    pont++;
                    x--;
                }
            }
            else if(op == 2){
                string s; cin >> s;
                cout << VO.value(s) << "\n";
            }
            else if(op == 3){
                string s; cin >> s;
                cout << VO.rank(s) << "\n";
            }
            else{
                int k; cin >> k;
                cout << VO.select(k) << "\n";
            }
        }
    }
    else if(estrutura == "ABB"){
        ABB<string, int> abb;
        for(int i = 1; i <= q; i++){
            int op; cin >> op;

            if(op == 1){
                int x; cin >> x;
                while(x){
                    int ant = abb.value(texto[pont]);
                    abb.add(texto[pont], ant + 1);
                    pont++;
                    x--;
                }
            }
            else if(op == 2){
                string s; cin >> s;
                cout << abb.value(s) << "\n";
            }
            else if(op == 3){
                string s; cin >> s;
                cout << abb.rank(s) << "\n";
            }
            else{
                int k; cin >> k;
                cout << abb.select(k) << "\n";
            }
        }
    }
    else if(estrutura == "TR"){
        Treap<string, int> treap;
        for(int i = 1; i <= q; i++){
            int op; cin >> op;

            if(op == 1){
                int x; cin >> x;
                while(x){
                    int ant = treap.value(texto[pont]);
                    treap.add(texto[pont], ant + 1);
                    pont++;
                    x--;
                }
            }
            else if(op == 2){
                string s; cin >> s;
                cout << treap.value(s) << "\n";
            }
            else if(op == 3){
                string s; cin >> s;
                cout << treap.rank(s) << "\n";
            }
            else{
                int k; cin >> k;
                cout << treap.select(k) << "\n";
            }
        }
    }
    else if(estrutura == "A23"){
        A23<string, int> a23;
        for(int i = 1; i <= q; i++){
            int op; cin >> op;

            if(op == 1){
                int x; cin >> x;
                while(x){
                    int ant = a23.value(texto[pont]);
                    a23.add(texto[pont], ant + 1);
                    pont++;
                    x--;
                }
            }
            else if(op == 2){
                string s; cin >> s;
                cout << a23.value(s) << "\n";
            }
            else if(op == 3){
                string s; cin >> s;
                cout << a23.rank(s) << "\n";
            }
            else{
                int k; cin >> k;
                cout << a23.select(k) << "\n";
            }
        }
    } 
    else if(estrutura == "ARN"){
        ARN<string, int> arn;
        for(int i = 1; i <= q; i++){
            int op; cin >> op;

            if(op == 1){
                int x; cin >> x;
                while(x){
                    int ant = arn.value(texto[pont]);
                    arn.add(texto[pont], ant + 1);
                    pont++;
                    x--;
                }
            }
            else if(op == 2){
                string s; cin >> s;
                cout << arn.value(s) << "\n";
            }
            else if(op == 3){
                string s; cin >> s;
                cout << arn.rank(s) << "\n";
            }
            else{
                int k; cin >> k;
                cout << arn.select(k) << "\n";
            }
        }
    }
}