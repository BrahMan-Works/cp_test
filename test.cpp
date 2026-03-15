#include<bits/stdc++.h>
#define int long long
using namespace std;
int solve(){
	int n;
	cin >> n;
	int a[n];
	int c1=0,c2=0,flag=0;
	for(int i=0;i<n;i++){
		cin >> a[i];
		if(a[i]==25){
			c1++;
		}
		else if(a[i]==50){
			if(c1>0){
				c1--;
				c2++;
			}
			else{
				flag=1;
			}
		}
		else{
			int cur=0;
			if(c2>0){
				c2--;
				cur = 1;
			}
			if(cur){
				if(c1>0){
					c1--;
				}
				else{
					flag=1;
				}
			}
			else{
				if(c1>2){
					c1 -= 3;
				}
				else{
					flag=1;
				}
			}
		}
	}
	if(flag){
		cout << "NO\n";
	}
	else{
		cout << "YES\n";
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
