#include <stdlib.h>
#include <stddef.h>

void* reallocarray(void* ptr, size_t nmemb, size_t size) {
	return realloc(ptr, nmemb*size);
}
