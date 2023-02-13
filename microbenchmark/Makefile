CC=clang++
BUILD_PATH=../build
TARGET_ISA=x86

CFLAGS= -O0 -pthread -fno-exceptions -I../ds -g
LDFLAGS=

# CPDIR= ../../../../tests/test-progs/threads/bin/$(TARGET_ISA)/linux/

qtest: test_queue.cpp
	$(CC) -o $(BUILD_PATH)/qtest test_queue.cpp $(CFLAGS)

clean:
	rm $(BUILD_PATH)/*