CC=gcc
STD=c99
CFLAGS=-I.
DEPS = *.h
#LIBS = -lpthread -lm -lsqlite3 -L/usr/lib64/mysql/ -lmysqlclient  
LIBS = -lpthread -lm 
OBJ = main.o \
	accel.o gyro.o locator.o trajectory_segment.o \
	serialIO.o simm_emitter_task.o \
	rtos_main_task.o

TAR = tireAllocate

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -std=$(STD)

$(TAR): $(OBJ)
	mkdir ./obj
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv $(TAR) $(OBJ) ./obj/

.PHONY: clean
clean:
	rm ./obj -rf
	rm $(TAR) $(OBJ)
