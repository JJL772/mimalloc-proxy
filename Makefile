
all: libmimalloc-vg.so libmimalloc-crt.so

libmimalloc-vg.so: mimalloc-vg.c
	$(CC) -shared -o $@ -fvisibility=default $<

libmimalloc-crt.so: mimalloc-crt.c
	$(CC) -shared -o $@ -fvisibility=default $<

clean:
	rm libmimalloc-crt.so || true
	rm libmimalloc-vg.so || true
