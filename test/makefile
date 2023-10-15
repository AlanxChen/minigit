CC = gcc
CFLAGS = -Wall -O2

SRCS = main.c commen_fun.c core_function/init.c repository/repository.c repository/config_msg.c
OBJS = $(SRCS:.c=.o)
TARGET = minigit

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)