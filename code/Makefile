CC = g++
PROJ = test
OBJS = MemoryPool.o test.o
CPP_FLAGS = '-std=c++11'
IS_CHECK = false

$(PROJ): $(OBJS)
	$(CC) -std=c++11 $^ -o $@

%.o: %.cpp
ifeq ($(IS_CHECK), true)
	$(CC) -std=c++11 -D_CHECK -c $^ -o $@
else
	$(CC) -std=c++11 -c $^ -o $@
endif

clean:
	rm *.o  ${PROJ} 
