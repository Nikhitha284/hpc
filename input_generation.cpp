#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include<algorithm>

std::vector<int> generateDistinctRandomNumbers(int n, int k) {
    std::vector<int> distinctNumbers;
    std::mt19937 rng(static_cast<unsigned>(std::time(0))); // Seed the random number generator
    std::uniform_int_distribution<int> distribution(1, n); // Define the range of random numbers

    // Generate k distinct random numbers
    while (distinctNumbers.size() < static_cast<size_t>(k)) {
        int randomNumber = distribution(rng);
        if (std::find(distinctNumbers.begin(), distinctNumbers.end(), randomNumber) == distinctNumbers.end()) {
            distinctNumbers.push_back(randomNumber);
        }
    }

    // If n > k, shuffle the numbers to randomize the order
    if (n > k) {
        std::shuffle(distinctNumbers.begin(), distinctNumbers.end(), rng);
    }

    return distinctNumbers;
}

int main() {
    
    
   
    int c = 10;
    std::mt19937 rng(static_cast<unsigned>(std::time(0))); // Seed the random number generator
    std::uniform_int_distribution<int> numJobsDistribution(1, 50);
    std::uniform_int_distribution<int> dDistribution(1, 4);
    std::uniform_int_distribution<int> numChunksDistribution(1, 5);
  std::uniform_int_distribution<int> distribution(1, c); 
    std::cout << "200 64 4" << std::endl;
    int num_of_jobs = numJobsDistribution(rng);
    std::cout << num_of_jobs << std::endl;
    int d = dDistribution(rng);
   
    
    

    while (num_of_jobs--) {
        std::cout << dDistribution(rng)  << " ";
        int num_of_chunks = numChunksDistribution(rng);
        std::cout << num_of_chunks << " ";
        std::vector<int> chunks = generateDistinctRandomNumbers(c, num_of_chunks);
        for (int i = 0; i < num_of_chunks; i++) {
            std::cout << chunks[i] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
