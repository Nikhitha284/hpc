#include <bits/stdc++.h>
using namespace std;

#define fori(i, n) for (int i = 0; i < n; i++)
#define pb push_back
#define pi pair<int, int>

map<int, vector<pi>> active_nodes;
int N, B, S, no_of_jobs, nodes = 0, y = 0;
struct job
{
    int d;
    int c;
    vector<int> chunks;
};
vector<job> jobs;

bool compareByDeadline(const job &a, const job &b)
{
    return a.d < b.d;
}
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
    if (pre[B - 1] >= limit)
    {
        return {0, B - 1};
    }
    return {-1, -1};
}
vector<pair<int, int>> calculate_R(vector<int> chunk_hist)
{
    map<int, int> l;
    vector<pair<int, int>> R;
    for (auto y : chunk_hist)
    {
        l[y]++;
    }
    for (auto y : l)
    {
        R.pb({y.second, y.first});
    }
    return R;
}
int findchunk(int chunk, vector<pi> temp)
{

    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].second == chunk)
        {
            return i;
        }
    }
    return -1;
}

int cred_m()
{
    std::sort(jobs.begin(), jobs.end(), compareByDeadline);
    int ans = 0;
    map<int, vector<int>> data_deadline;
    for (int i = 0; i < jobs.size(); i++)
    {
        for (int j = 0; j < jobs[i].chunks.size(); j++)
            data_deadline[jobs[i].d].push_back(jobs[i].chunks[j]);
    }
    for (auto f : data_deadline)
    {
        vector<pair<int, int>> R = calculate_R(f.second);
        if (active_nodes.size() == 0)
        {
            ans = ans + cred_s(R, f.first, B, S);
        }
        else
        {
            for (auto y : R)
            {

                for (auto g : active_nodes)
                {
                    if (findchunk(y.second, g.second) != -1)
                    {
                        int t_slots = f.first - g[findchunk(y.second, g.second)].first;
                        if (t_slots > 0)
                        {
                            y.first -= min(t_slots, y.first);
                            g[findchunk(y.second, g.second)].first += min(t_slots, y.first);
                        }
                    }
                }
            }
        }
    }
}

vector<pi> schedule(vector<pair<int, int>> &R, int no_of_slots, int l, int r, int d)
{
    int NTS = no_of_slots;
    vector<pi> temp;
    int initial_size = active_nodes.size();
    for (int i = l; i <= r; i++)
    {
        int k = min(R[i].first, d);
        temp.push_back({k, R[i].second});
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

    active_nodes[initial_size + 1] = temp;
    return R;
}

int cred_s(vector<pair<int, int>> R, int d, int B, int S)
{
    int ans = 0;
    pi p = hb(R, B, S, d);
    while (R.size() > 0 && p.first != -1 && p.second != -1)
    {
        cout << p.first << "***" << p.second << endl;
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

        R = schedule(R, S * d, 0, min(int(R.size() - 1), B - 1), d);
        ans = ans + 1;
    }

    return ans;
}

int main()
{
    cin >> N >> B >> S;

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
    // nodes = cred_s(R, 3, B, S);
    // cout << nodes<<endl;
    // cout<<"?????????"<<endl;
    // for(auto f:active_nodes){
    //     cout<<f.first<<endl;
    //     for(auto g:f.second){
    //         cout<<g.second<<" "<<g.first<<" "<<endl;
    //     }
    //     cout<<endl;
    // }
    cred_m();
}