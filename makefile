OBJS	= divideMat.o
SOURCE	= divideMat.c
HEADER	= 
OUT	= divideMat
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

divideMat.o: divideMat.c
	$(CC) $(FLAGS) divideMat.c 


clean:
	rm -f $(OBJS) $(OUT)
