LAVCFLAGS = $(shell pkg-config --cflags libavformat libavcodec libavutil)
LAVLDFLAGS = $(shell pkg-config --libs libavformat libavcodec libavutil)
CC := gcc
CFLAGS := -g -Wall

all: live_segmenter

%.o: %.c
	$(CC) $(CFLAGS) $(LIBAVCFLAGS) live_segmenter.c -c

live_segmenter: live_segmenter.o
	$(CC) $(CFLAGS) $(LIBAVCFLAGS) $< -o $@ $(LDFLAGS) $(LAVLDFLAGS)

clean:
	rm -f live_segmenter
