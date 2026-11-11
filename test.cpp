#include<bits/stdc++.h>
using namespace std;

int n;
string s;

int rec(int i, char last){

    if(i==n){
        return 1;
    }

    int ans = 0;
    if(s[i]=='?'){
        for(char ch = 'A'; ch<='D';ch++){
            if(ch==last) continue;
            ans += rec(i+1,ch);
        }
    }else if(s[i]!=last) ans = rec(i+1,s[i]);

    return ans;
}

int main(){
    cin>>s;
    n = s.length();
    cout<<rec(0,0);
}