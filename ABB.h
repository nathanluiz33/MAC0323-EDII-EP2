#include <bits/stdc++.h>
using namespace std;

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
