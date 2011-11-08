CC=arm-linux-gcc
TARGET=main

CFLAGS= -Wall -Os 
LDFLAGS= -lm -lpthread
OBJS+= main.o fbdraw.o draw_rim.o fft.o device.o

all:$(TARGET)

$(TARGET):$(OBJS)
#	$(LD) $^ $(LDFLAGS) -o $@

$(OBJS):%.o:%.c main.h device.h fbdraw.h draw_rim.h device.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@ 


.PHONY:clean
clean:
	-rm main *.o -rf
