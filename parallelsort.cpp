//Implementing Parallel Quick Sort

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

class QSMultiThreading 
{
    private:
        int strt_;
        int end_;
        vector<int>& arr_;

    public:
        QSMultiThreading(int strt, int end, vector<int>& arr) 
            : strt_(strt), end_(end), arr_(arr) {}
        
        int partition(int strt, int end, vector<int>& arr) 
        {
            int i = strt;
            int j = end;

            int pivoted = rand() % (j - i + 1) + i;

            swap(arr[j], arr[pivoted]);
            j--;

            while (i <= j) 
            {
                if (arr[i] <= arr[end]) 
                {
                    i++;
                    continue;
                }
                if (arr[j] >= arr[end]) 
                {
                    j--;
                    continue;
                }
                swap(arr[i], arr[j]);
                j--;
                i++;
            }

            swap(arr[j + 1], arr[end]);

            return j + 1;
        }

        void operator() () 
        {
            if (strt_ >= end_) 
            {
                return;
            }

            int p = partition(strt_, end_, arr_);

            QSMultiThreading left(strt_, p - 1, arr_);
            QSMultiThreading right(p + 1, end_, arr_);

            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    left();
                }
                #pragma omp section
                {
                    right();
                }
            }
        }

};

int main() 
{
    srand(time(NULL));

    int n;
    cout << "Enter num of elements: ";
    cin >> n;
    vector<int> arr(n);

    cout << "Enter elements: ";

    for (int i = 0; i < n; ++i) 
    {
        cin >> arr[i];
    }

    QSMultiThreading(0, n - 1, arr)();

    cout << "Sorted array: ";
    for (int i = 0; i < n; ++i) 
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}






/*
Explanation of the Code: Quick Sort Using Parallel Divide and Conquer Strategy
The given code implements Quick Sort using a parallel divide-and-conquer strategy 
with the help of the OpenMP library for multi-threading. Here's the detailed explanation:

Step-by-Step Code Explanation

1. Input and Initialization:
   - The main function initializes a random seed for random pivot selection.
   - Takes input for the number of elements and the array itself.

2. Partitioning:
   - The partition() function selects a random pivot within the range [start, end] 
     and places it at the end of the array.
   - Elements smaller than the pivot are moved to its left, and larger elements to its right.
   - The function returns the pivot's final position after partitioning.

3. Parallel Divide and Conquer:
   - The operator() function handles the divide-and-conquer logic.
   - If the subarray has one or zero elements, it terminates recursion.
   - Otherwise, the array is divided around the pivot.
   - Two new tasks (for the left and right subarrays) are created using the QSMultiThreading class.

4. Multi-Threading with OpenMP:
   - OpenMP's #pragma omp parallel sections directive is used to execute tasks in parallel.
   - Each section runs a recursive call to the left and right subarrays independently.

5. Output:
   - After sorting, the sorted array is printed.

Parallel Divide and Conquer Strategy
-------------------------------------
The divide-and-conquer approach involves three steps:

1. Divide: Split the array into two parts around the pivot.
2. Conquer: Recursively sort the subarrays.
3. Combine: Merge results (implicit in Quick Sort).

Parallelization is achieved by:
- Processing the left and right subarrays concurrently using multi-threading.
- OpenMP manages the threads, ensuring tasks are executed simultaneously, leveraging multi-core processors.

Threading Concept
------------------
1. Threading:
   - A thread is the smallest unit of a CPU's processing power.
   - Multiple threads allow tasks to run concurrently.

2. How It's Used:
   - The code uses OpenMP to spawn threads for the left and right recursive calls.
   - Each thread works on a different part of the array.

3. OpenMP Directives:
   - #pragma omp parallel sections: Divides work into sections to run in parallel.
   - #pragma omp section: Marks individual tasks to be executed concurrently.

Parallel Programming and OpenMP
---------------------------------
1. Parallel Programming:
   - Involves breaking tasks into subtasks that run concurrently.
   - Maximizes CPU utilization, reduces execution time, and solves large-scale problems efficiently.

2. OpenMP Library:
   - A widely used library for parallel programming in C/C++.
   - Simplifies the creation and management of threads with minimal code changes.
   - The directives (e.g., #pragma omp) guide the compiler to parallelize the code.

3. Relation to the Code:
   - The OpenMP directives handle thread creation and synchronization for recursive quick sort calls.

Mathematical Analysis
----------------------
Time Complexity:
1. Sequential Quick Sort:
   - Best and Average Case: O(n log n)
   - Worst Case: O(n^2) (occurs when the pivot divides the array poorly).

2. Parallel Quick Sort:
   - Best Case: O(n/p log(n/p)), where p is the number of threads.
   - Speedup Factor: S_p = T_seq / T_par, where T_seq and T_par are sequential and parallel execution times, respectively.

Space Complexity:
- O(log n): Recursive stack space.

Applications
------------
1. Parallel Programming:
   - Data analytics (e.g., sorting large datasets).
   - Machine learning preprocessing.
   - Computational biology and simulations.

2. Quick Sort with Parallel Divide-and-Conquer:
   - Large-scale sorting tasks (e.g., in distributed systems).
   - Real-time applications requiring fast sorting (e.g., financial algorithms).

Exam Invigilator Questions
---------------------------
1. Question: What does the partition() function do in this code?
   Answer: It rearranges the array such that all elements smaller than the pivot 
   are on its left, and larger elements are on its right. It returns the final position of the pivot.

2. Question: How does OpenMP parallelize the recursion?
   Answer: Using #pragma omp parallel sections, the recursion for left and right 
   subarrays is run in separate threads.

3. Question: What is the advantage of random pivot selection?
   Answer: Random pivot selection reduces the likelihood of worst-case partitioning, 
   improving average-case performance.

4. Question: What is the time complexity of this parallel algorithm?
   Answer: Best case is O(n/p log(n/p)), where p is the number of threads.

5. Question: Why is OpenMP preferred for this task?
   Answer: OpenMP simplifies parallelization with minimal code changes, 
   making it easy to leverage multi-core CPUs for divide-and-conquer algorithms.

6. Question: Can Quick Sort handle duplicate elements efficiently?
   Answer: Yes, Quick Sort handles duplicates by correctly partitioning elements equal 
   to the pivot on either side.
*/

