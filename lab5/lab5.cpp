/*
 * Student Name: Donaven Bruce
 * Student ID: 0000000
 */
#include <iostream>
using namespace std; // using statement added here

const int SIZE = 10; // Array size constant

// Swaps values using pointers
void swap(int *ptrA, int *ptrB)
{
    int temp = *ptrA;
    *ptrA = *ptrB;
    *ptrB = temp;
}

// Pointer bubble sort in Ascending order
void bubbleSort(int *array, int size)
{
    bool madeAswap;

    do
    {
        madeAswap = false;
        for (int i = 1; i < size; i++)
        {
            // Uses index for loop; could use pointer iteration
            if (*(array + i - 1) > *(array + i))
            {
                swap((array + i - 1), (array + i));
                madeAswap = true;
            }
        }
        size--; // largest value now in place
    } while (madeAswap); // stop early if no swaps
}

// Prints all elements of an array to the console
void printArray(const int *array, int size)
{
    for (int i = 0; i < size; i++)
        std::cout << *(array + i) << " ";
    std::cout << std::endl;
}

// Program entry: I/O, sort, copy, print, cleanup
int main()
{

    std::cout << "Student Name: Donaven Bruce\n";
    std::cout << "Student ID: 0000000\n\n";

    int firstArray[SIZE];
    // prompt for one line input
    std::cout << "Enter " << SIZE << " integer numbers (separated by spaces):\n";
    for (int i = 0; i < SIZE; i++)
    {
        // Prompt user to Enter number
        std::cin >> firstArray[i]; // Get 10 ints
    }

    bubbleSort(firstArray, SIZE); // Pointer bubble sort ascending

    int *secondArray = new int[SIZE]; // Alloc new 10 int array

    for (int i = 0; i < SIZE; i++)
    {
        *(secondArray + i) = *(firstArray + (SIZE - 1 - i)); // Copy reversed to new array
    }

    std::cout << "\nFirst array (sorted ascending):\n";
    printArray(firstArray, SIZE); // Display first array ascending

    std::cout << "\nSecond array (sorted descending):\n";
    printArray(secondArray, SIZE); // Display second array descending

    delete[] secondArray; // Free dynamic array
    secondArray = nullptr;

    // Freeze output window code
    std::cin.ignore();
    std::cin.get();

    return 0;
}

// Test cases
// 1 2 3 4 5 6 7 8 9 10
// 10 9 8 7 6 5 4 3 2 1
// 5 5 5 5 5 5 5 5 5 5
// -3 0 7 -1 5 -2 2 2 4 -1
// 4 2 4 2 4 2 4 2 4 2
// 9 1 6 3 0 4 8 2 7 5
// 8 3 3 9 0 -4 1 -2 6 1
// 7 0 2 5 1 3 4 6 2 1
