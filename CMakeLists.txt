project(httplvs-sd)
cmake_minimum_required(VERSION 2.6)

include(detectFFMpeg.cmake)


add_executable(segmenter segmenter.c options_parsing.c)
target_link_libraries(segmenter -lavformat -lavcodec -lavutil ${FFMPEG_LIBS})

set (CMAKE_INSTALL_PREFIX /usr/)

install (FILES  "${PROJECT_BINARY_DIR}/hs_config.rb"
                "${PROJECT_BINARY_DIR}/hs_encoder.rb"
                "${PROJECT_BINARY_DIR}/hs_transfer.rb"
                "${PROJECT_BINARY_DIR}/http_streamer.rb"
                DESTINATION bin
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ  WORLD_EXECUTE)
                
install (TARGETS segmenter DESTINATION bin)