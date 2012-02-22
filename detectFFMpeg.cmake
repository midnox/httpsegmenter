message( STATUS "detecting  ffmpeg ...." )
#TODO: Add more flexible way to detect/
execute_process (
	COMMAND pkg-config --libs libavformat libavcodec libavutil 
	COMMAND sed  -e "s/-lavformat//" -e "s/-lavcodec//" -e "s/-lavutil//" 
	OUTPUT_VARIABLE FFMPEG_LIBS
	OUTPUT_STRIP_TRAILING_WHITESPACE
) 
string (REPLACE "  " " " FFMPEG_LIBS "${FFMPEG_LIBS}")
string (REPLACE "  " " " FFMPEG_LIBS "${FFMPEG_LIBS}")
string (REPLACE "  " " " FFMPEG_LIBS "${FFMPEG_LIBS}")
string (STRIP "${FFMPEG_LIBS}" FFMPEG_LIBS)
message(STATUS "Needed extra ffmpeg libs: ${FFMPEG_LIBS}" )
