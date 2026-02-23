#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
#include <chrono>
using namespace std;

/**
 * @brief returns the first index of elem if it exists, otherwise returns -1
 * iterativeSearch starts at the first index and iterates sequentially to the next until it either
 * finds the element or until it reaches the the end (i.e. element does not exist)
 *
 * @param v : vector of elements
 * @param elem : integer to look for
 * @return int
 */
int iterativeSearch(vector<int>v, int elem)
{
    for (int i = 0; i < v.size(); i++) if (elem == v[i]) return i;
    return -1;
}

/**
 * @brief returns the index of elem, if it exists in v. Otherwise it returns -1.
 * binarySearch is recursive (i.e. function calls itself).
 * It utilizes the fact that v is increasing order (e.g. values go up and
 * duplicates are not allowed).
 *
 * It calculates the mid from the start and end index. It compares v[mid] (i.e. value
 * at mid) with elem and decides whether to search the left half (lower values)
 * or right half (upper values).
 *
 *
 * @param v : vector of elements
 * @param start : leftmost index (starting value is 0)
 * @param end  :rightmost index (starting value is v.size())
 * @param elem : integer to look for
 * @return int
 */
int binarySearch(vector<int>& v, int start, int end, int elem)
{
    if (end < start) return -1;

    int piv = start + (end - start) / 2;

    if (v[piv] == elem) return piv;
    else if (v[piv] < elem) return binarySearch(v, piv + 1, end, elem);
    else return binarySearch(v, start, piv - 1, elem);
}

/**
 * @brief updates v to contain the values from filename (leave as is)
 *
 * It is expected that the files contain values ranging from one to
 * one hundred million in ascending order (no duplicates).
 *
 * @param filename : string
 * @param v :vector
 */
void vecGen(string filename, vector<int>& v) {
    ifstream file(filename);
    int num;
    v.clear();

    if (!file.is_open()) {
        cout << "Failed to open: " << filename << endl;
        return;
    }

    while (file >> num) {
        v.push_back(num);
    }

    file.close();
}

/**
 * @brief writes to file the time it took to search with respect to the
 *  size of the vector, n
 *  Number of Elements (n)	    Time (sec)
    XXXX	                    X.XXXXX
    XXXX	                    X.XXXXX
 * @param filename (string) : filename (e.g. output_10000_numbers.csv)
 * @param times (vector<double>) : average times
 * @param n (vector<int>) : sizes of vectors
 */
void writeTimes(string filename, const vector<double> times, const vector<int> n) {
    ofstream myFile(filename);

    myFile << "Number of Elements (n)\t Time (sec) " << endl;
    for (int i = 0; i < times.size(); i++) {

        myFile << n[i] << "," << times[i] << "\n";
    }
    myFile.close();
    cout << "Wrote to " << filename << endl;
}

/**
 * @brief computes the average of the elements in vector, a
 *
 * @param a  vector of double
 * @return double
 */
double average(const vector<double> a) {

    double sum = 0.0;
    for (int i = 0; i < (int)a.size(); i++) sum += a[i];
    return sum / a.size();

}

int main() {
    // test elements to search for
    vector<int> elem_to_find;
    vecGen("test_elem.csv", elem_to_find);
    // size (n) of all tests 
    vector<int> file_sizes;
    vecGen("sizes.csv", file_sizes);

    //n list of numbers
    vector<int>v;

    //results of times
    vector<double> times;

    //results of avg per workload size
    vector<double> avg;

    // create a for loop to iterate through the file sizes
    for (int i = 0; i < file_sizes.size(); i++) {
        // get the name/size of the file and assign it to string called filename
        string filename = to_string(file_sizes[i]) + "_numbers.csv";
        //call vecGen on filename and v
        vecGen(filename, v);
        // print filename (this will be good for debugging)
        cout << "Loaded: " << filename << " (n=" << v.size() << ")\n";
        // call times.clear() // this ensures that we reset times everytime we read a new file 
        times.clear();
        // create another for loop to iterate through all the elements from elem_to_find. 
        // the code here should be nearly identical to the code from the previous lab 
        for (int j = 0; j < (int)elem_to_find.size(); j++) {
            auto start = chrono::high_resolution_clock::now();
            (void)iterativeSearch(v, elem_to_find[j]);
            auto end = chrono::high_resolution_clock::now();

            double elapse_time_in_sec = chrono::duration<double>(end - start).count();

            // append the elapsed_time_in_sec to the vector,times (hint: push_back())
            // This code shuold be within the for loop that iterates 
            // through all the elements from elem_to_find
            times.push_back(elapse_time_in_sec);
        }

        // call average on the vector, times, and save it as a double. This code should be 
        // outside the for loop that iterates through all the elements from elem_to_find
        // but within the for loop that iterates through the file sizes
        double avg_time = average(times);

        // append the double to avg. (hint: push_back())
        // This code should be outside the for loop that iterates through
        // all the elements from elem_to_find
        // but within the for loop that iterates through the file sizes
        avg.push_back(avg_time);
    }

    //outside both for loops call writeTimes with the appropriate parameters
    // the first parameter should be "iterativeSearch_times.csv"
    // read the function brief to complete the rest of the parameters
    writeTimes("iterativeSearch_time.csv", avg, file_sizes);

    // call avg.clear() to reset avg, so we can use it for binarySearch
    avg.clear();

    // repeat the nested for loops used for iterativeSearch, but call binarySearch instead
    for (int i = 0; i < (int)file_sizes.size(); i++) {
        string filename = to_string(file_sizes[i]) + "_numbers.csv";
        vecGen(filename, v);
        cout << "Loaded: " << filename << "(n= " << v.size() << ")\n";
        times.clear();
        for (int j = 0; j < (int)elem_to_find.size(); j++) {
            clock_t start = clock();

            for (int k = 0; k < 1000; k++) {
                binarySearch(v, 0, v.size() - 1, elem_to_find[j]);
            }

            clock_t end = clock();

            double elapsed_time_in_sec = double(end - start) / CLOCKS_PER_SEC;
            times.push_back(elapsed_time_in_sec);
        }

        double avg_time = average(times);
        avg.push_back(avg_time);
    }

    //outside both for loops call writeTimes with the appropriate parameters
    // the first parameter should be "binarySearch_times.csv"
    // read the function brief to complete the rest of the parameters
    writeTimes("binarySearch_times.csv", avg, file_sizes);

    return 0;
}