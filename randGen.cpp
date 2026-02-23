#include <iostream>
#include <set>
#include <random>
#include <fstream>
#include <string>
#include <cmath>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int range = 100000000; //100 million range
    std::uniform_int_distribution<int> dis(1, range);
    for (int i = 10; i <= 10000000; i *= 10) { //10 million generations

        std::set<int> numbers;

        // Generate unique random numbers
        while (numbers.size() < i) {
            numbers.insert(dis(gen));
        }

        // Open the file for writing
        std::string fname = std::to_string(i) + "_numbers.csv";
        std::ofstream writeFile(fname);
        if (!writeFile.is_open()) {
            std::cerr << "Unable to open file for writing." << std::endl;
            return 1; // Return an error code
        }

        // Write the numbers to the file in ascending order
        for (int num : numbers) {
            writeFile << num << "\n";
        }

        // Close the file
        writeFile.close();

        std::cout << "Numbers written to " << fname << " successfully. " 
            << "(" << i << " numbers)\n";
    }

    return 0;
}
