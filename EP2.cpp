#include <bits/stdc++.h>
#include "VO.h"
#include "ABB.h"
#include "TR.h"
#include "ARN.h"
#include "A23.h"
using namespace std;

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