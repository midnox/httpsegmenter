FFFLAGS = $(shell pkg-config --cflags libavformat libavcodec libavutil)
FFLIBS = $(shell pkg-config --libs libavformat libavcodec libavutil)
CC := gcc
CFLAGS := -g -Wall $(FFFLAGS)
LDFLAGS := $(FFLIBS)

all: live_segmenter

%.o: %.c
	$(CC) $(CFLAGS) live_segmenter.c -c

live_segmenter: live_segmenter.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f live_segmenter
