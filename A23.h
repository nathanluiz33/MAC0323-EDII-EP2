#include <bits/stdc++.h>
using namespace std;

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
        // se o nó tem 2 elementos, devemos somar 1 no peso
        if(!raiz->node_2) peso_novo++;
        if(raiz->esq != NULL) peso_novo += raiz->esq->peso;
        if(raiz->meio != NULL) peso_novo += raiz->meio->peso;
        if(raiz->dir != NULL) peso_novo += raiz->dir->peso;
        raiz->peso = peso_novo;
    }
    /*
     * Função: fix_node 
     * Uso: fix_node(Node *raiz, int tipo);
     * -------------------
     * Esta função merge o nó raiz com o nó subiu. Se o nó que sobe vem 
     * da esquerda, tipo = 0. Se vem do meio, tipo = 1. E se vem da direita,
     * tipo = 2. Caso o nó atual esteja saturado também, a função atualiza o nó
     * atual e o nó subiu. Essa função funciona em O(1) e é essencial para manter
     * as propriedades da árvore 23.
     */
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
            // se o nó não estiver livre
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
        // tipo 1 -> vem do meio
        else if(tipo == 1){
            // se o nó estiver livre
            if(raiz->node_2){
                raiz->meio = subiu->esq;
                raiz->dir = subiu->meio;
                raiz->key_r = subiu->key_l;
                raiz->val_r = subiu->val_l;
                raiz->node_2 = false;
                balancear_peso(raiz);
                subiu = NULL;
            }
            // se o nó não estiver livre
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
        // tipo 2 -> vem da direita
        else{
            // se vem da direita, o nó atual com certeza tem 2 elementos.
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

    /*
     * Função: add 
     * Uso: add(Node *raiz, Key _key, Item _val);
     * -------------------
     * Adiciona o par (_key, _val) na subárvore de *raiz. Complexidade O(logN), altura da árvore.
     */
    void add(Node *raiz, Key _key, Item _val){
        if(raiz->key_l == _key)
            raiz->val_l = _val;
        else if(!(raiz->node_2) && raiz->key_r == _key)
            raiz->val_r = _val;
        else if(_key < raiz->key_l){
            // Se _key for menor do que a chave esquerda do nó, então devemos ir para o filho esquerdo
            if(raiz->esq == NULL){
                // se for uma folha, devemos atualizar subiu
                subiu = new Node(raiz->pai, _key, _val);
            }
            else{
                add(raiz->esq, _key, _val);
            }
            // caso o nó que subiu não seja NULL, devemos mergir com o atual
            if(subiu != NULL)
                fix_node(raiz, 0);
        }
        else if(!(raiz->node_2) && raiz->key_r < _key){
            if(raiz->dir == NULL)
                subiu = new Node(raiz->pai, _key, _val);
            else add(raiz->dir, _key, _val);
            // caso o nó que subiu não seja NULL, devemos mergir com o atual
            if(subiu != NULL)
                fix_node(raiz, 2);
        }
        else{
            if(raiz->meio == NULL){
                subiu = new Node(raiz->pai, _key, _val);
            }
            else
                add(raiz->meio, _key, _val);
            // caso o nó que subiu não seja NULL, devemos mergir com o atual
            if(subiu != NULL)
                fix_node(raiz, 1);
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
            root->key_l = _key;
            root->val_l = _val;
            root->peso = 1;
            root->esq = root->meio = NULL;
        }
        else{
            add(root, _key, _val);
            // se o nó que subiu não é NULL, devemos atualizar a raiz
            if(subiu != NULL){
                root = subiu;
                root->pai = NULL;
                subiu = NULL;
            }
            balancear_peso(root);
        }
    }
    
    /*
     * Função: value 
     * Uso: value(Node *raiz, Key _key);
     * -------------------
     * Procura o Item associado à chave _key na subárvore de *raiz. Caso não exista a chave _key,
     * retorna o construtor vazio Item(). Complexidade O(h).
     */
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
            // vamos pro meio
            if(raiz->meio != NULL) return value(raiz->meio, _key);
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