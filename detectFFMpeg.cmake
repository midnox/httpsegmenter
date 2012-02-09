message( STATUS "detecting  ffmpeg ...." )
#TODO: Add more flexible way to detect/
execute_process (
	COMMAND pkg-config --libs libavformat libavcodec libavutil 
	COMMAND sed  -e "s/-lavformat//" -e "s/-lavcodec//" -e "s/-lavutil//" -e "s/ -l/ /g" -e "s/^-l//" -e "s/-pthread/pthread/"
	OUTPUT_VARIABLE FFMPEG_LIBS
	OUTPUT_STRIP_TRAILING_WHITESPACE
) 
message( STATUS "Needed extra ffmpeg libs: ${FFMPEG_LIBS}" )
