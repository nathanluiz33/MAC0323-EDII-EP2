#include <bits/stdc++.h>
using namespace std;

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
            // Se _key for maior do que a chave do nó, então devemos ir para o filho direito
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