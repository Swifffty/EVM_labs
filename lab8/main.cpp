#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <random>
#include <x86intrin.h>

#define RDTSC __builtin_ia32_rdtsc


void generate_direct_list(std::vector<int>& arr, int N) {
    for (int i = 0; i < N - 1; ++i) {
        arr[i] = i + 1;
    }
    arr[N - 1] = 0;
}


void generate_reverse_list(std::vector<int>& arr, int N) {
    for (int i = 1; i < N; ++i) {
        arr[i] = i - 1;
    }
    arr[0] = N - 1;
}


void generate_random_list(std::vector<int>& arr, int N) {
    generate_direct_list(arr, N);

    std::vector<int> indices(N);
    std::iota(indices.begin(), indices.end(), 0);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

    for (int i = 0; i < N - 1; ++i) {
        arr[indices[i]] = indices[i + 1];
    }
    arr[indices[N - 1]] = indices[0];
}

double measure_access_time(const std::vector<int>& arr, long long M, int N) {
    volatile int k = 0;
    unsigned long long start, end;

    for (int i = 0; i < N; ++i) {
        k = arr[k];
    }

    start = RDTSC();

    for (long long i = 0; i < M; ++i) {
        k = arr[k];
    }

    end = RDTSC();

    return (double)(end - start) / M;
}

int main() {

    std::ofstream outfile("access_time_results.csv");

    outfile << "N_Elements,N_Bytes,Ticks_Direct,Ticks_Reverse,Ticks_Random\n";
    std::cout << "N\t\tSize (MB)\tDirect\t\tReverse\t\tRandom\n";
    std::cout << "------------------------------------------------------------------\n";

    const long long M_TOTAL = 1LL << 30;

    for (int N = (1 << 8); N <= (1 << 26); N *= 2) {
        long long M_PER_TEST = M_TOTAL;

        if (N > (1 << 20)) {
            M_PER_TEST = std::max((long long)N * 10, M_TOTAL / (N / 1024));
            if (M_PER_TEST < N * 4) M_PER_TEST = N * 4;
        }

        std::vector<int> array(N);

        double time_direct, time_reverse, time_random;

        generate_direct_list(array, N);
        time_direct = measure_access_time(array, M_PER_TEST, N);

        generate_reverse_list(array, N);
        time_reverse = measure_access_time(array, M_PER_TEST, N);

        generate_random_list(array, N);
        time_random = measure_access_time(array, M_PER_TEST, N);

        int size_bytes = N * sizeof(int);
        double size_mb = (double)size_bytes / (1024.0 * 1024.0);

        std::cout << N << "\t\t" << std::fixed << std::setprecision(2) << size_mb
        << "\t\t" << std::setprecision(2) << time_direct
        << "\t\t" << time_reverse
        << "\t\t" << time_random << std::endl;

        outfile << N << "," << size_bytes << "," << time_direct << "," << time_reverse << "," << time_random << "\n";
    }

    outfile.close();

    return 0;
}
