FFLIBS=`pkg-config --libs libavformat libavcodec libavutil`
FFFLAGS=`pkg-config --cflags libavformat libavcodec libavutil`

all:
	gcc -Wall -g segmenter.c -o segmenter ${FFFLAGS} ${FFLIBS}

old_ffmpeg:
	gcc -Wall -g segmenter.c -o segmenter -lavformat -lavcodec -lavutil -lm -o build/debug/segmenter

clean:
	rm -f segmenter

install: segmenter
	cp segmenter /usr/local/bin/

uninstall:
	rm /usr/local/bin/segmenter