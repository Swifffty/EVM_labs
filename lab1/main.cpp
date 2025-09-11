#include <iostream>
#include <iomanip>

unsigned long long pow(int x, int cnt) {
    unsigned long long power = 1;
    for (int i = 1; i <= cnt; i++) {
        power *= x;
    }
    return power;
}

unsigned long long row_macloren(int x, int N) {
    int cnt = 2;
    unsigned long long res = 1;
    while (cnt <= N) {
        unsigned long long pw = pow(x, cnt);
        for (int i = 2; i<= cnt; i++) {
            pw /= i;
        }
        res += pw;
        cnt++;
    }
    return res;
}

int main() {
    int N = 60000;
    int x = 1;
    unsigned long long answer =  row_macloren(x, N);
    std::cout<<answer;
    return 0;
}