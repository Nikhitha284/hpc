#include <bits/stdc++.h>
using namespace std;

#define fori(i, n) for (int i = 0; i < n; i++)
#define pb push_back
#define pi pair<int, int>

int N, B, S, no_of_jobs, nodes = 0, y = 0;

struct job
{
    int d;
    int c;
    vector<int> chunks;
};

pi hb(vector<pair<int, int>> R, int B, int S, int d)
{
    int n = R.size(), cnt = 0, limit = S * d;
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

vector<pi> schedule(vector<pair<int, int>> &R, int no_of_slots, int l, int r, int d)
{
    int NTS = no_of_slots;
    for (int i = l; i <= r; i++)
    {
        int k = min(R[i].first, d);
        if (k > NTS)
        {
            R[i].first -= NTS;
            NTS = 0;
            break;
        }
        else
        {
            NTS -= min(R[i].first, d);
            R[i].first = R[i].first - min(R[i].first, d);
        }
    }
    sort(R.begin(), R.end());
    reverse(R.begin(), R.end());
    cout << y << endl;
    for (auto x : R)
    {
        cout << x.first << " ";
        y++;
    }
    cout << endl;
    while (R[R.size() - 1].first == 0)
        R.pop_back();

    return R;
}

int cred_s(vector<pair<int, int>> R, int d, int B, int S)
{
    int ans = 0;
    pi p = hb(R, B, S, d);
    while (R.size() > 0 && p.first != -1 && p.second != -1)
    {
        cout << p.first << " " << p.second << endl;
        R = schedule(R, S * d, p.first, p.second, d);
        ans += 1;
        p = hb(R, B, S, d);
    }
    // cout << "-";
    // for (auto x : R)
    //     cout << x.first << " ";
    // cout << "-";
    while (R.size() > 0)
    {

        R = schedule(R, S * d, 0, min(int(R.size() - 1), B), d);
        ans = ans + 1;
    }

    return ans;
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