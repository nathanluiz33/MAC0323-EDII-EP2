#include <bits/stdc++.h>
using namespace std;

#define dbg(x)  cout << #x << " = " << x << "\n"

template<class Key, class Item>
class Vetor_Ordenado{
    private:
    vector< pair<Key, Item> > v;

    int find_first_bigger(Key key){
        if(!v.size()) return -1;
        if(key < v[0].first or v.back().first < key) return -1;

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

    bool isEmpty(){
        return !(v.size());
    }

    bool contains(Key key){
        int k = find_first_bigger(key);
        if(k == -1) return false;
        return v[k].first == key;
    }

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

    Item value(Key key){
        if(!contains(key)) return Item();
        int k = find_first_bigger(key);
        return v[k].second;
    }

    int rank(Key key){
        if(!v.size() || key < v[0].first) return 0;
        if(v.back().first < key) return v.size();
        return find_first_bigger(key);
    }

    Key select(int k){
        if(k < 0 or k >= v.size()) return Key();
        return v[k].first;
    }
};

#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds;

typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

int n;

signed main(){
    cin >> n;

    Vetor_Ordenado<int, int> VO;
    map<int, int> M;
    ordered_set S;
    for(int i = 1; i <= n; i++){
        int x; cin >> x;

        S.insert(x);
        M[x]++;
        
        int ant = VO.value(x);
        if(ant == 0) ant = 0;
        VO.add(x, ant + 1);

        int pos = VO.rank(x);
        assert(VO.select(pos) == *S.find_by_order(pos));
        assert(M[x] == VO.value(x));
        assert(VO.rank(x) == S.order_of_key(x));
    }

    for(auto u : S){
        // cout << VO.value(u) << " ";
        assert(M[u] == VO.value(u));
        assert(VO.rank(u) == S.order_of_key(u));
    }
    dbg("Okay!");
}