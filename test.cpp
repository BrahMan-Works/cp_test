#include<bits/stdc++.h>
#define int long long
using namespace std;
int solve(){
	int n;
	cin >> n;
	int val=1,sum=0;
	while(sum+5*val < n){
		sum += 5*val;
		val*=2;
	}
	int temp = n-sum;
	int cur=0,ind=0;
	while(cur+val < temp){
		cur += val;
		ind++;
	}
	if(ind == 0){
		cout << "Sheldon\n";
	}
	else if(ind == 1){
		cout << "Leonard\n";
	}
	else if(ind==2){
		cout << "Penny\n";
	}
	else if(ind==3){
		cout << "Rajesh\n";
	}
	else{
		cout << "Howard\n";
	}
	return 0;
}
signed main(){
	int t=1;
	//cin >> t;
	while(t--){
		solve();
	}
}
