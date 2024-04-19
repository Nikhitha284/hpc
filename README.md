# To run on sample test cases
g++ diff_deadline.cpp -std=gnu++11 -o main
./main.exe

# To run on randomly generated test cases
g++ input.cpp -o input -std=c++11
g++ diff_deadline.cpp -std=gnu++11 -o main
./input.exe | ./main.exe

# Input Format
Our test cases follow the following format:
- N (nodes), B (maximum number of chunks that can be placed on each node), S (number of virtual machines on each physical node)
- Number of jobs to be scheduled
- For each job in jobs:
  - Deadline, number of chunks to be placed followed by chunk IDs
