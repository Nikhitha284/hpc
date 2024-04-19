#include <bits/stdc++.h>
using namespace std;

// macros for loop iteration and vector functions
#define fori(i, n) for (int i = 0; i < n; i++)
#define pb push_back
#define pi pair<int, int>

vector<pair<int, vector<pi>>> active_nodes;
int N, B, S, no_of_jobs, nodes = 0, y = 0;

// struct to represent a job
struct job
{
    int deadline;
    int no_of_chunks;
    vector<int> chunks;
};

// vector to store all the jobs
vector<job> jobs;

// comparator function to sort the jobs based on deadline
bool compareByDeadline(const job &a, const job &b)
{
    return a.deadline < b.deadline;
}

// function to calculate the first set of B contiguous chunks H, from the tail of the vector R whose sum >=S*deadline(total slots in an active node)
pi calculate_H_of_size_B(vector<pair<int, int>> R, int B, int S, int d)
{
    // limit represents the maximum available slots in any VM
    int n = R.size(), cnt = 0, limit = S * d;

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
            // returns the start index and end index of H  in R
            return {i - B + 1, i};
            break;
        }
    }
    // handles the corner case where H starts from the 0 th index
    if (pre[B - 1] >= limit)
    {
        return {0, B - 1};
    }
    // if no such H is found , return {-1,-1}
    return {-1, -1};
}

// function to calculate the vector R which gives the information regarding time slots needed for every chunk
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
// function to search for the chunk in an active node
int findchunk(int chunk, vector<pi> temp)
{

    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].second == chunk)
        {
            // return the index if present
            return i;
        }
    }
    // return -1 nif not present
    return -1;
}
int sum(vector<pi> temp)
{
    int sum = 0;
    fori(i, temp.size()) sum = sum + temp[i].first;
    return sum;
}

vector<pi> schedule(vector<pair<int, int>> &R, int no_of_slots, int l, int r, int d)
{
    // l and r represent the start and end indexes of H in R

    int NTS = no_of_slots;
    vector<pi> temp;
    int initial_size = active_nodes.size();
    // iterating over array H
    for (int i = l; i <= r; i++)
    {
        int k = min(R[i].first, d);

        if (k > NTS)
        {
            // update R
            R[i].first -= NTS;
            NTS = 0;
            break;
        }
        // if available slots are more than needed slots
        else
        {

            NTS -= min(R[i].first, d);
            // update R
            R[i].first = R[i].first - min(R[i].first, d);
        }
        temp.push_back({k, R[i].second});
    }

    // updating R
    sort(R.begin(), R.end());
    reverse(R.begin(), R.end());

    while (R[R.size() - 1].first == 0)
        R.pop_back();

    active_nodes.pb({initial_size, temp});
    return R;
}
// function to calculate number of active nodes needed for the jobs which have same deadline (trying as much as possible to reduce the active nodes using our
// proposed heuristic)

int cred_s(vector<pair<int, int>> R, int d, int B, int S)
{
    int ans = 0;
    // calculate H
    pi p = calculate_H_of_size_B(R, B, S, d);
    // while such array H  of size B can be found from R , push chunks in H into a new active nodes
    while (R.size() > 0 && p.first != -1 && p.second != -1)
    {

        R = schedule(R, S * d, p.first, p.second, d);
        // increment the number of active nodes
        ans += 1;
        p = calculate_H_of_size_B(R, B, S, d);
    }
    // handling the remaining chunks when a subset H of size B  cannot be found in R
    while (R.size() > 0)
    {

        R = schedule(R, S * d, 0, min(int(R.size() - 1), B - 1), d);
        // increment the number of active nodes
        ans = ans + 1;
    }
    // output the number of active nodes
    return ans;
}

// function to calculate number of active nodes needed for the jobs which have different deadlines (trying as much as possible to
// reduce the active nodes using our  proposed heuristic)

int cred_m()
{
    // sort the jobs based on deadline in ascending order
    std::sort(jobs.begin(), jobs.end(), compareByDeadline);
    // initialising the variable which stores number of active nodes needed
    int ans = 0;
    // trying to bring all jobs with same  deadline together
    map<int, vector<int>> data_deadline;
    for (int i = 0; i < jobs.size(); i++)
    {
        for (int j = 0; j < jobs[i].chunks.size(); j++)
            data_deadline[jobs[i].deadline].push_back(jobs[i].chunks[j]);
    }
    // now iterating over all possible deadlines
    for (auto f : data_deadline)
    {
        // first calculate vector R for the jobs which have same deadline
        vector<pair<int, int>> R = calculate_R(f.second);
        // when current active nodes =0
        if (active_nodes.size() == 0)
        {
            // calculate active nodes needed till then  using cred_s
            ans = ans + cred_s(R, f.first, B, S);
        }
        else
        {
            fori(i, R.size())
            {
                // for every chunk iterate over active_nodes

                fori(j, active_nodes.size())
                {
                    // if required number of slots for an active node is equal to zero then exit from the loop
                    if (R[i].first == 0)
                    {
                        break;
                    }
                    // if the chunk is already present in the active node
                    if (findchunk(R[i].second, active_nodes[j].second) != -1)
                    {
                        // if slots are available
                        if (sum(active_nodes[j].second) < S * f.first)
                        {
                            int t_slots = f.first - active_nodes[j].second[findchunk(R[i].second, active_nodes[j].second)].first;
                            if (t_slots > 0)
                            {
                                // if time slots available
                                int a = min(t_slots, R[i].first);
                                // update R and active_nodes
                                R[i].first -= min(t_slots, R[i].first);

                                active_nodes[j].second[findchunk(R[i].second, active_nodes[j].second)].first += a;
                            }
                        }
                    }
                }
                if (R[i].first != 0)
                {
                    // check if the chunk can be placed in an already exsisting active node
                    fori(j, active_nodes.size())
                    {
                        // check if the chunk is already present or not , if not present
                        if (findchunk(R[i].second, active_nodes[j].second) == -1)
                        {
                            // if there is still space left
                            if (active_nodes[j].second.size() < B)
                            {
                                // and time slots are still available the schedule
                                if (sum(active_nodes[j].second) < S * f.first)
                                {
                                    int a = min(min(R[i].first, f.first), S * f.first - sum(active_nodes[j].second));
                                    // update R and active_nodes
                                    active_nodes[j].second.pb({a, R[i].second});
                                    R[i].first -= a;
                                }
                            }
                        }
                    }
                }
            }
            // update R
            sort(R.begin(), R.end());
            reverse(R.begin(), R.end());
            while (R[R.size() - 1].first == 0)
                R.pop_back();
            // update the number of active nodes
            ans = ans + cred_s(R, f.first, B, S);
        }
    }
    // return the number of acteive nodes needed
    return ans;
}
int main()
{
    // Input system parameters and job information
    cout << "Enter the number of physical machines " << endl;
    cin >> N;
    cout << "Enter the value of B" << endl;
    cin >> B;
    cout << "Enter the maximum number of virtual machines that can be run on a single physical machine" << endl;
    cin >> S;

    map<int, int> mp;
    job job1;
    cout << "Enter the number of jobs" << endl;
    cin >> no_of_jobs;
    // for every job take input from user
    cout << "for each job enter the deadline and chunk set size and chunk ids of all chunks needed for the job" << endl;
    while (no_of_jobs--)
    {
        int deadline, no_of_chunks;
        // take the deadline and chunk set as input from the user
        cin >> deadline >> no_of_chunks;
        vector<int> chunks(no_of_chunks);
        fori(i, no_of_chunks)
        {
            cin >> chunks[i];
            mp[chunks[i]]++;
        }
        // store the values in a struct
        job1.deadline = deadline, job1.no_of_chunks = no_of_chunks;
        job1.chunks = chunks;
        // store all the jobs in a vector
        jobs.pb(job1);
    }

    // output the number of active nodes needed
    cout << cred_m() << endl;
    for (auto f : active_nodes)
    {
        cout << "node: " << f.first << endl;
        for (auto g : f.second)
        {
            cout << "chunk :" << g.second << " " << g.first << " " << endl;
        }
        cout << endl;
    }
}