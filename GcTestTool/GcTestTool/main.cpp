#include <sys/time.h>
#include <stdio.h>
#include <algorithm>
#include "include/gc.h"

double GetTimeMs()
{
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	double time_in_mill =
	(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000.0 ;
	return time_in_mill;
}

double totalGCTime = 0;
double maxGCTime = 0;
double currentGCStart = 0;
int gcCount = 0;

void GcCallback(GCEventType event_type)\
{
	switch (event_type)
	{
		case GC_EVENT_START:
			currentGCStart = GetTimeMs();
			break;
		case GC_EVENT_END:
			{
				double thisGcTime = GetTimeMs() - currentGCStart;
				totalGCTime += thisGcTime;
				maxGCTime = std::max(thisGcTime, maxGCTime);
				gcCount++;
			}
			break;
		default:
			break;
	}
	
}
/* Invoked when the heap grows or shrinks.      */
/* Called with the world stopped (and the       */
/* allocation lock held).  May be 0.            */
GC_API void GC_CALL GC_set_on_event(GC_on_event_proc);

double StartBenchmark()
{
	totalGCTime = 0;
	maxGCTime = 0;
	currentGCStart = 0;
	
	return GetTimeMs();
}

void EndBenchmark(double startTime)
{
	printf("Total/GC/Spike/Count: %lfms/%lfms/%lfms/%dx\n", (GetTimeMs()-startTime), totalGCTime, maxGCTime, gcCount);
	GC_clear_roots();
}

void RunGCBenchmark(const char* name, void(*testFunc)(size_t, size_t), size_t numObjects, size_t size)
{
	printf("%s %lu x %lu\n", name, numObjects, size);

	double startTime = StartBenchmark();

	testFunc(numObjects, size);
	
	EndBenchmark(startTime);
}

void RunGCBenchmark(const char* name, void(*testFunc)(size_t, size_t, size_t), size_t numLists, size_t numObjectsPerList, size_t size)
{
	printf("%s %lu x %lu x %lu\n", name, numLists, numObjectsPerList, size);
	double startTime = StartBenchmark();
	
	testFunc(numLists, numObjectsPerList, size);
	
	EndBenchmark(startTime);
}

void AllocateObjects(size_t numObjects, size_t size)
{
	for (int i=0; i<numObjects; i++)
		GC_malloc(size);
}

void AllocateLinkedLists(size_t numLists, size_t numObjectsPerList, size_t size)
{
	for (int i=0; i<numLists; i++)
	{
		void *start = GC_malloc_uncollectable(size);
		void *lastObject = start;
		for (int j=1; j<numObjectsPerList; j++)
		{
			void *obj =GC_malloc(size);
			*(void**)lastObject = obj;
			lastObject = obj;
		}
	}
}

int main(int argc, const char * argv[])
{
	GC_INIT();
	GC_set_on_event(GcCallback);

	RunGCBenchmark("Allocate Objects", AllocateObjects, 1000, 16);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 10000, 16);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 100000, 16);
	printf("\n");
	
	RunGCBenchmark("Allocate Objects", AllocateObjects, 1000, 1024);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 10000, 1024);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 100000, 1024);
	printf("\n");
	
	RunGCBenchmark("Allocate Objects", AllocateObjects, 1000, 65536);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 10000, 65536);
	RunGCBenchmark("Allocate Objects", AllocateObjects, 100000, 65536);
	printf("\n");
	
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 100, 16);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 1000, 16);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 10000, 16);
	printf("\n");
	
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 100, 1024);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 1000, 1024);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 10000, 1024);
	printf("\n");
	
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 100, 65536);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 1000, 65536);
	RunGCBenchmark("Allocate Linked Lists", AllocateLinkedLists, 10, 10000, 65536);
	
	GC_deinit();
}
