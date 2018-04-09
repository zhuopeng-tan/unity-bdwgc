#include <iostream>
#include "../../include/gc.h"
#include <chrono>
#include <stdio.h>
#include "include/gc.h"
#include <private/gc_priv.h>

extern "C"
{
	void GC_dirty(ptr_t p);
}

int main(int argc, const char * argv[]) {
	GC_INIT();
	GC_enable_incremental();

	std::chrono::steady_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i<10000; i++)
	{
		void* test = GC_malloc(1024);
		GC_dirty((ptr_t)test);
	}


	std::chrono::steady_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::cout << "Total time: " << int_ms.count() << " ms\n";
	int test;
	std::cin >> test;
}
