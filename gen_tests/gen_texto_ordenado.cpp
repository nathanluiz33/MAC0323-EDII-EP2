#include <bits/stdc++.h>
using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main(int argc, char** argv){
    string s;
    int n = 0;
    map<string, int> freq;
    vector<string> texto;
    while(cin >> s){
        if(freq[s]) continue;
        freq[s]++;
        texto.push_back(s);
        n++;
    }
    sort(texto.begin(), texto.end());
    reverse(texto.begin(), texto.end());
    cout << argv[1] << "\n";
    cout << n << "\n";
    for(auto u : texto) cout << u << " ";
    int q = 3e5;
    cout << q << "\n";
    cout << 1 << " " << n / 2 << "\n";
    for(int i = 1; i <= q / 2; i++){
        long long op = rng() % 3 + 2;
        long long pos = rng() % n;
        if(op == 4) cout << op << " " << pos << "\n";
        else cout << op << " " << texto[pos] << "\n";
    }
    cout << 1 << " " << n - n / 2 << "\n";
    for(int i = 1; i <= q - q / 2; i++){
        long long op = rng() % 3 + 2;
        long long pos = rng() % n;
        if(op == 4) cout << op << " " << pos << "\n";
        else cout << op << " " << texto[pos] << "\n";
    }
}