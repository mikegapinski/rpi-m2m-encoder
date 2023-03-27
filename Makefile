CC = gcc
CFLAGS = -Wall -O3 -g3
LIBS = -lv4l2 -lm

OBJS = m2m.o logging.o frame.o main.o

all: m2m_encoder

m2m_encoder: $(OBJS)
	$(CC) $(CFLAGS) -o m2m_encoder $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o m2m_encoder
