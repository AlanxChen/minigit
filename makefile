CC = gcc
CFLAGS = -Wall -O2

SRCS = main.c commen_fun.c core_function/init.c repository/repository.c repository/config_msg.c repository/object.c core_function/hash.c core_function/cat_file.c 
OBJS = $(SRCS:.c=.o)
TARGET = minigit

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lssl -lcrypto -lz

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)