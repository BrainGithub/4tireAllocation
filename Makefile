CC=gcc
CPP=g++
STD=c99
CFLAGS=-I.
DEPS = *.h
#LIBS = -lpthread -lm -lsqlite3 -L/usr/lib64/mysql/ -lmysqlclient  
LIBS = -lpthread -lm 
OBJ = main.o \
	accel.o gyro.o locator.o trajectory_segment.o \
	rtos_main_task.o serialIO.o \
	simm_emitter_task.o 

TAR = tireAllocate

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -std=$(STD)

%.o: %.cpp $(DEPS)
	g++ -c -o $@ $< $(CFLAGS)

$(TAR): $(OBJ)
	mkdir ./obj
	$(CPP) -o $@ $^ $(CFLAGS) $(LIBS)
	mv $(TAR) $(OBJ) ./obj/

.PHONY: clean
clean:
	rm ./obj -rf
	rm $(TAR) $(OBJ)
