#include <iostream>
#include <omp.h>
#include <thread>
#include <mutex>

using namespace std;

int iter_count = 9307170;
int iter_max = 100000000;
int iter_max_edit = 100000000;

mutex mtx;

int main() {
	int thread_count = 8;
	long double pi = 0;
	int iter_max_edit = iter_max;

	auto begin = chrono::system_clock::now();
#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < thread_count; i++) {
			while (iter_max_edit > 0) {
				mtx.lock();
				int start = iter_max_edit;
				int end = iter_count;
				iter_max_edit -= iter_count;
				mtx.unlock();

				long double x = 0.0;

				for (int i = start; i > start - end && i >= 0; i--) {
					x += 4.0 / (1 + ((0.5 + i) / iter_max) * ((0.5 + i) / iter_max));
				}
				lock_guard<mutex>guard(mtx);
				pi += x;
			};
		}
	}
	pi /= iter_max;
	auto end = chrono::system_clock::now();

	cout << "pi = " << pi << endl;
	cout << "time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
	return 0;
}
