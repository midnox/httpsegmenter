FFLIBS=`pkg-config --libs libavformat libavcodec libavutil`
FFFLAGS=`pkg-config --cflags libavformat libavcodec libavutil`
CC := gcc
CFLAGS := -g -Wall

all: live_segmenter

%.o: %.c
	$(CC) $(CFLAGS) $(FFFLAGS) live_segmenter.c -c

live_segmenter: live_segmenter.o
	$(CC) $(CFLAGS) $(FFFLAGS) $< -o $@ $(LDFLAGS) $(FFLIBS)

clean:
	rm -f live_segmenter
