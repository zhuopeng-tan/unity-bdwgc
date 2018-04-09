#include <iostream>
#include "../../include/gc.h"

int main()
{
	GC_INIT();

	GC_malloc(10);
	std::cout << "Hello World" << std::endl;
	return 0;
}