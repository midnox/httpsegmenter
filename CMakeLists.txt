project(httplvs-sd)
cmake_minimum_required(VERSION 2.6)

include(detectFFMpeg.cmake)


add_executable(segmenter segmenter.c)
target_link_libraries(segmenter -lavformat -lavcodec -lavutil ${FFMPEG_LIBS})
