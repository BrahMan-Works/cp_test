#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

#define int int64_t

//--------------Pacific_Hues--------------\\

void solve() 
{
    int n; 
    std::cin >> n;

    auto a = std::vector<int>(n + 2, 0);
    for(int i = 1; i <= n; ++i) 
    {
        std::cin >> a[i];
    }

    auto dp = std::vector<int>(n + 2, 0);
    for(int i = n; i >= 1; --i) 
    {
        if(i & 1) 
        {
            int b = a[i];
            if(i > 1) 
            {
                b = std::min(b, a[i - 1]);
            }
            if(i < n) 
            {
                b = std::min(b, a[i + 1] - dp[i + 2]);
            }
            dp[i] = std::max<int>(0, b);
        }
    }

    int ans = 0;
    for(int i = 1; i <= n; i += 2) 
    {
        ans += a[i] - dp[i];
    }

    std::cout << ans << '\n';
}

signed main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t; 
    std::cin >> t;
    while(t--) solve();
}
