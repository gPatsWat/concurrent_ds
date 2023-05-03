CC=clang++
BUILD_PATH=../build
TARGET_ISA=x86

CFLAGS= -O0 -pthread -fno-exceptions -I../ds -g
LDFLAGS=

# CPDIR= ../../../../tests/test-progs/threads/bin/$(TARGET_ISA)/linux/

thread_launch: thread_launch.cpp
	$(CC) -o $(BUILD_PATH)/thread_launch thread_launch.cpp $(CFLAGS)

clean:
	rm $(BUILD_PATH)/*