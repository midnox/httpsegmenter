FFLIBS=`pkg-config --libs libavformat libavcodec libavutil`
FFFLAGS=`pkg-config --cflags libavformat libavcodec libavutil`
all:
	gcc -Wall -g live_segmenter.c -o live_segmenter ${FFFLAGS} ${FFLIBS}

clean:
	rm -f live_segmenter
