#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream file("tests_lab2.txt");

    int n;
    file >> n;

    vector<int> numbers(n);
    for (int i = 0; i < n; i++) {
        file >> numbers[i];
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;

    return 0;
}
