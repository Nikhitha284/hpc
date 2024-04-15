#include <bits/stdc++.h>
using namespace std;

#define fori(i, n) for (int i = 0; i < n; i++)
#define pb push_back
#define pi pair<int, int>

int N, B, S, no_of_jobs, nodes = 0;

struct job
{
    int d;
    int c;
    vector<int> chunks;
};

pi hb(vector<pair<int, int>> R, int B, int limit)
{
    int n = R.size(), cnt = 0;
    // check working
    fori(i, n) if (R[i].first > 0) cnt++;
    if (cnt < B)
        return {-1, -1};
    vector<int> pre(n, 0);
    fori(i, n) pre[i] = R[i].first;
    for (int i = 1; i < n; i++)
        pre[i] = pre[i] + pre[i - 1];
    for (int i = n - 1; i >= 0; i--)
    {
        if (pre[i] - pre[i - B] >= limit)
        {
            return {i - B + 1, i};
            break;
        }
    }
    return {-1, -1};
}

int cred_m()
{
    return 0;
}

void schedule(vector<pair<int, int>> &R, int NTS, int l, int r)
{
    
}

int cred_s(vector<pair<int, int>> R, int d, int B, int S)
{
    int ans = 0;
    pi p = hb(R, B, S * d);
    while (p != {-1, -1})
    {
        cout << p.first << " " << p.second;
        R = schedule(R, S * d, p.first, p.second);
        ans += 1;
        pi p = hb(R, B, S * d);
    }
    return 0;
}

int main()
{
    cin >> N >> B >> S;
    vector<job> jobs;
    map<int, int> mp;
    job job1;
    cin >> no_of_jobs;
    while (no_of_jobs--)
    {
        int d, c;
        cin >> d >> c;
        vector<int> chunks(c);
        fori(i, c)
        {
            cin >> chunks[i];
            mp[chunks[i]]++;
        }
        job1.d = d, job1.c = c;
        job1.chunks = chunks;
        jobs.pb(job1);
    }
    vector<pi> R;
    for (auto x : mp)
    {
        R.pb({x.second, x.first});
    }
    sort(R.begin(), R.end());
    reverse(R.begin(), R.end());
    nodes = cred_s(R, 3, B, S);
    cout << nodes;
}