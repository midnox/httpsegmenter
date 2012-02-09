#ifndef segmenter_h_a22d54e85a2948bbb2c2219e34c7c168_included
#define segmenter_h_a22d54e85a2948bbb2c2219e34c7c168_included

//#include <pthread.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define MAX_FILENAME_LENGTH 1024

//Type of options detected
typedef enum {INPUT_FILE, OUTPUT_FILE, OUTPUT_DIR, OUTPUT_BASE_NAME,OUTPUT_AUDIO_ONLY,OUTPUT_VIDEO_ONLY,SEGMENT_LENGTH,OUTPUT_VERBOSITY,VERSION,NO_MORE_OPTIONS,INVALID} inputOption;

//type of output to perform
#define OUTPUT_STREAM_AUDIO 1
#define OUTPUT_STREAM_VIDEO 2

//index for options in requiredOptions @ parseCommandLine
#define INPUT_FILE_INDEX 0
#define OUTPUT_FILE_INDEX 1
#define OUTPUT_DIR_INDEX 2
#define OUTPUT_BASE_NAME_INDEX 3
#define OUTPUT_SEGMENT_LENGTH_INDEX 4

#ifdef  __cplusplus
}
#endif

#endif