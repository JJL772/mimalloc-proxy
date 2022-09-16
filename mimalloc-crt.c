
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>

#define FORCE_ALIGN 16

void mi_option_enable(int) {}
void mi_option_disable(int) {}
void mi_register_output(void*, void*) {};
void mi_register_error(void*, void*) {};
void mi_process_init() {}
void mi_collect(int) {}
void mi_process_info(size_t*, size_t*, size_t*, size_t*, size_t*, size_t*, size_t*, size_t*) {};
void mi_stats_print_out(void*,void*) {}

void* mi_malloc(size_t size) {
#ifdef FORCE_ALIGN
	return aligned_alloc(FORCE_ALIGN, size);
#else
	return malloc(size);
#endif
}

void* mi_realloc(void* p, size_t size) {
	return realloc(p, size);
}

void mi_free(void* p) {
	free(p);
}

void* mi_malloc_aligned(size_t size, size_t align) {
	return aligned_alloc(align, size);
}

void* mi_realloc_aligned(void* p, size_t newsize, size_t align) {
	// This is a pretty crappy & slow implementation
	size_t toCopy = malloc_usable_size(p);
	void* newp = aligned_alloc(align, newsize);
	memcpy(newp, p, toCopy);
	free(p);
	return newp;
}

size_t mi_usable_size(const void* p) {
	return malloc_usable_size((void*)p);
}

void* mi_expand(void*, size_t) {
	printf("mi_expand not implemented\n");
	return 0;
}

int mi_is_in_heap_region(void*) {
	return 1;
}
