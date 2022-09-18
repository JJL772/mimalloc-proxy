
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>
#include <dlfcn.h>
#include <assert.h>

// Source generally wants 16 byte align
#define FORCE_ALIGN 16

#define DBG_TRACE fprintf(stderr, "mimalloc-proxy: %s: not implemented\n", __FUNCTION__)

void(*_libc_free)(void*);
void*(*_libc_aligned_alloc)(size_t, size_t);
void*(*_libc_malloc)(size_t);
size_t(*_libc_malloc_usable_size)(void*);
void*(*_libc_realloc)(void*, size_t);
void*(*_libc_calloc)(size_t, size_t);

static void init_funcs() {
	static void* hlibc = NULL;
	if (hlibc)
		return;
	hlibc = dlopen("libc.so.6", RTLD_LAZY);
	assert(hlibc);
	
	_libc_free = dlsym(hlibc, "free");
	_libc_malloc = dlsym(hlibc, "malloc");
	_libc_aligned_alloc = dlsym(hlibc, "aligned_alloc");
	_libc_malloc_usable_size = dlsym(hlibc, "malloc_usable_size");
	_libc_realloc = dlsym(hlibc, "realloc");
	_libc_calloc = dlsym(hlibc, "calloc");
}


void mi_option_enable(int x) {}
void mi_option_disable(int x) {}
void mi_register_output(void* a, void* b) {};
void mi_register_error(void* a, void* b) {};
void mi_process_init() {}
void mi_collect(int a) {}
void mi_stats_print_out(void* a,void* b) {}

void* mi_malloc(size_t size) {
	init_funcs();
#ifdef FORCE_ALIGN
	return _libc_aligned_alloc(FORCE_ALIGN, size);
#else
	return _libc_malloc(size);
#endif
}

void* mi_realloc(void* p, size_t size) {
	init_funcs();
	return _libc_realloc(p, size);
}

void mi_free(void* p) {
	init_funcs();
	_libc_free(p);
}

void* mi_malloc_aligned(size_t size, size_t align) {
	init_funcs();
	return _libc_aligned_alloc(align, size);
}

void* mi_realloc_aligned(void* p, size_t newsize, size_t align) {
	init_funcs();
	// This is a pretty crappy & slow implementation
	size_t toCopy = _libc_malloc_usable_size(p);
	void* newp = _libc_aligned_alloc(align, newsize);
	memcpy(newp, p, toCopy);
	_libc_free(p);
	return newp;
}

size_t mi_usable_size(const void* p) {
	init_funcs();
	return _libc_malloc_usable_size((void*)p);
}

void* mi_expand(void* a, size_t b) {
	DBG_TRACE; // Log this, dont' know if it's ever needed
	return 0;
}

int mi_is_in_heap_region(void* a) {
	return 1;
}

void mi_process_info(size_t* a, size_t* b, size_t* c, size_t* d, size_t* e, size_t* f, size_t* g, size_t* h) {
	*a = *b = *c = *d = *e = *f = *g = *h = 0;
}
