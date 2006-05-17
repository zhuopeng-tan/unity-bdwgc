#ifndef LIBGC_MONO_DEBUGGER_H
#define LIBGC_MONO_DEBUGGER_H

#if defined(_IN_LIBGC_GC_H) || defined(_IN_THE_MONO_DEBUGGER)

typedef struct
{
	void (* initialize) (void);

	void (* thread_created) (pthread_t tid, void *stack_ptr);
	void (* thread_exited) (pthread_t tid, void *stack_ptr);

	void (* stop_world) (void);
	void (* start_world) (void);
} GCThreadFunctions;

extern GCThreadFunctions *gc_thread_vtable;

extern void
GC_mono_debugger_add_all_threads (void);

#else
#error "This header is only intended to be used by the Mono Debugger"
#endif

#endif

