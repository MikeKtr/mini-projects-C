CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -L.
LIBS = -lsig

LIB_SOURCES = sig_default.c sig_mask.c sig_ignore.c sig_handle.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

all: static shared dynamic


static: $(LIB_OBJECTS)
	ar rcs libsig.a $(LIB_OBJECTS)
	$(CC) $(CFLAGS) main.c -L. -lsig -o main_static

shared: $(LIB_OBJECTS)
	$(CC) -shared -o libsig.so $(LIB_OBJECTS)
	$(CC) $(CFLAGS) main.c -L. -lsig -Wl,-rpath,. -o main_shared


dynamic: $(LIB_OBJECTS)
	$(CC) -shared -o libsig.so $(LIB_OBJECTS)
	$(CC) $(CFLAGS) main.c -D DYNAMIC_LIB -ldl -o main_dynamic

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.a *.so main_static main_shared main_dynamic