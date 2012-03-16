/*
 * Copyright (c) 2009 Chase Douglas
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**********************************************************************************
 * This code is part of HTTP-Live-Video-Stream-Segmenter-and-Distributor
 * look for newer versions at github.com
 **********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "segmenter.h"
#include "libavformat/avformat.h"

#define IMAGE_ID3_SIZE 9171


void ffmpeg_version() {
	printBanner();
    // output build and version numbers
    fprintf(stderr, "libav versions:\n");
    fprintf(stderr, "  libavutil version:   %s\n", AV_STRINGIFY(LIBAVUTIL_VERSION));
    fprintf(stderr, "  libavutil build:     %d\n", LIBAVUTIL_BUILD);
    fprintf(stderr, "  libavcodec version:  %s\n", AV_STRINGIFY(LIBAVCODEC_VERSION));
    fprintf(stdout, "  libavcodec build:    %d\n", LIBAVCODEC_BUILD);
    fprintf(stderr, "  libavformat version: %s\n", AV_STRINGIFY(LIBAVFORMAT_VERSION));
    fprintf(stderr, "  libavformat build:   %d\n", LIBAVFORMAT_BUILD);
	
    fprintf(stderr, "This tool is version " PROGRAM_VERSION ",  built on " __DATE__ " " __TIME__);
#ifdef __GNUC__
    fprintf(stderr, ", with gcc: " __VERSION__ "\n");
#else
    fprintf(stderr, ", using a non-gcc compiler\n");
#endif
}


void build_id3_tag(char * id3_tag, size_t max_size) {
    int i;
    for (i = 0; i < max_size; i++)
        id3_tag[i] = 0;

    id3_tag[0] = 'I';
    id3_tag[1] = 'D';
    id3_tag[2] = '3';
    id3_tag[3] = 4;
    id3_tag[9] = '?';
    id3_tag[10] = 'P';
    id3_tag[11] = 'R';
    id3_tag[12] = 'I';
    id3_tag[13] = 'V';
    id3_tag[17] = '5';

    char id3_tag_name[] = "com.apple.streaming.transportStreamTimestamp";
    strncpy(&id3_tag[20], id3_tag_name, strlen(id3_tag_name));


}

void build_image_id3_tag(unsigned char * image_id3_tag) {
    FILE * infile = fopen("/home/rrtnode/rrt/lib/audio.id3", "rb");

    if (!infile) {
        fprintf(stderr, "Could not open audio image id3 tag.");
        exit(0);
    }

    fread(image_id3_tag, IMAGE_ID3_SIZE, 1, infile);
    fclose(infile);
}

void fill_id3_tag(char * id3_tag, size_t max_size, unsigned long long pts) {
    id3_tag[max_size - 1] = pts & 0xFF;
    id3_tag[max_size - 2] = (pts >> 8) & 0xFF;
    id3_tag[max_size - 3] = (pts >> 16) & 0xFF;
    id3_tag[max_size - 4] = (pts >> 24) & 0xFF;
    //TODO 33rd bit????
}

void debugReturnCode(int r) {
    switch (r) {
#if USE_OLD_FFMPEG
        case AVERROR_UNKNOWN:
#else
        case AVERROR(EINVAL):
#endif
            /**< unknown error */
            fprintf(stderr, "Unknown error.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_IO:
#else
        case AVERROR(EIO):
#endif             
            /**< I/O error */
            fprintf(stderr, "I/O error.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_NUMEXPECTED:
#else
        case AVERROR(EDOM):
#endif			    
            /**< Number syntax expected in filename. */
            fprintf(stderr, "Number syntax expected in filename.\n");
            break;
        case AVERROR_INVALIDDATA:
            /**< invalid data found */
            fprintf(stderr, "Invalid data found.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_NOMEM:
#else
        case AVERROR(ENOMEM):
#endif        
            /**< not enough memory */
            fprintf(stderr, "Not enough memory.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_NOFMT:
#else
        case AVERROR(EILSEQ):
#endif         
            /**< unknown format */
            fprintf(stderr, "Unknown format.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_NOTSUPP:
#else
        case AVERROR(ENOSYS):
#endif       
            /**< Operation not supported. */
            fprintf(stderr, "Operation not supported.\n");
            break;
#if USE_OLD_FFMPEG
        case AVERROR_NOENT:
#else
        case AVERROR(ENOENT):
#endif        
            /**< No such file or directory. */
            fprintf(stderr, "No such file or directory.\n");
            break;
        case AVERROR_EOF:
            /**< End of file. */
            fprintf(stderr, "End of file.\n");
            break;
        case AVERROR_PATCHWELCOME:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Not yet implemented in FFmpeg. Patches welcome.\n");
            break;
#if USE_OLD_FFMPEG
            /**< Codes For Old FFMPEG Deprecated. */
#else
            /**< New Error Codes For FFMPEG. */
        case AVERROR_BUG:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Internal bug. AVERROR_BUG\n");
            break;
        case AVERROR_BUG2:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Internal bug. AVERROR_BUG2.\n");
            break;
        case AVERROR_STREAM_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Stream not found.\n");
            break;
        case AVERROR_PROTOCOL_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Protocol not found.\n");
            break;
        case AVERROR_OPTION_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Option not found.\n");
            break;
        case AVERROR_MUXER_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Muxer not found. \n");
            break;
        case AVERROR_FILTER_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Filter not found.\n");
            break;
        case AVERROR_EXIT:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Immediate exit was requested; the called function should not be restarted.\n");
            break;
        case AVERROR_ENCODER_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Encoder not found.\n");
            break;
        case AVERROR_DEMUXER_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Demuxer not found.\n");
            break;
        case AVERROR_DECODER_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Decoder not found.\n");
            break;
        case AVERROR_BSF_NOT_FOUND:
            /**< Not yet implemented in FFmpeg. Patches welcome. */
            fprintf(stderr, "Bitstream filter not found.\n");
            break;
#endif	
        default:
            fprintf(stderr, "Unknown return code: %d\n", r);
    }
}

void write_stream_size_file(const char file_directory[], const char filename_prefix[], double size) {
    FILE * outputFile;
    char fullFileName[1024];
    snprintf(fullFileName, 1024, "%s/%s.size", file_directory, filename_prefix);

    outputFile = fopen(fullFileName, "w");
    fprintf(outputFile, "%u", (unsigned int) size);
    fclose(outputFile);
}

static AVStream *add_output_stream(AVFormatContext *output_format_context, AVStream *input_stream) {
    AVCodecContext *input_codec_context;
    AVCodecContext *output_codec_context;
    AVStream *output_stream;

    output_stream = avformat_new_stream(output_format_context, 0);
    if (!output_stream) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }

    input_codec_context = input_stream->codec;
    output_codec_context = output_stream->codec;

    output_codec_context->codec_id = input_codec_context->codec_id;
    output_codec_context->codec_type = input_codec_context->codec_type;
    output_codec_context->codec_tag = input_codec_context->codec_tag;
    output_codec_context->bit_rate = input_codec_context->bit_rate;
    output_codec_context->extradata = input_codec_context->extradata;
    output_codec_context->extradata_size = input_codec_context->extradata_size;

    if (av_q2d(input_codec_context->time_base) * input_codec_context->ticks_per_frame > av_q2d(input_stream->time_base) && av_q2d(input_stream->time_base) < 1.0 / 1000) {
        output_codec_context->time_base = input_codec_context->time_base;
        output_codec_context->time_base.num *= input_codec_context->ticks_per_frame;
    } else {
        output_codec_context->time_base = input_stream->time_base;
    }

    switch (input_codec_context->codec_type) {
#ifdef USE_OLD_FFMPEG
        case CODEC_TYPE_AUDIO:
#else
        case AVMEDIA_TYPE_AUDIO:
#endif
            output_codec_context->channel_layout = input_codec_context->channel_layout;
            output_codec_context->sample_rate = input_codec_context->sample_rate;
            output_codec_context->channels = input_codec_context->channels;
            output_codec_context->frame_size = input_codec_context->frame_size;
            if ((input_codec_context->block_align == 1 && input_codec_context->codec_id == CODEC_ID_MP3) || input_codec_context->codec_id == CODEC_ID_AC3) {
                output_codec_context->block_align = 0;
            } else {
                output_codec_context->block_align = input_codec_context->block_align;
            }
            break;
#ifdef USE_OLD_FFMPEG
        case CODEC_TYPE_VIDEO:
#else
        case AVMEDIA_TYPE_VIDEO:
#endif
            output_codec_context->pix_fmt = input_codec_context->pix_fmt;
            output_codec_context->width = input_codec_context->width;
            output_codec_context->height = input_codec_context->height;
            output_codec_context->has_b_frames = input_codec_context->has_b_frames;

            break;
        default:
            break;
    }

    return output_stream;
}

int write_index_file(const char index[], const char tmp_index[], const unsigned int planned_segment_duration, const unsigned int actual_segment_duration[],
        const char output_directory[], const char url_prefix[], const char output_prefix[], const char output_file_extension[],
        const unsigned int first_segment, const unsigned int last_segment) {
    FILE *index_fp;
    char *write_buf;
    unsigned int i;

    index_fp = fopen(tmp_index, "w");
    if (!index_fp) {
        fprintf(stderr, "Could not open temporary m3u8 index file (%s), no index file will be created\n", tmp_index);
        return -1;
    }

    write_buf = malloc(sizeof (char) * 1024);
    if (!write_buf) {
        fprintf(stderr, "Could not allocate write buffer for index file, index file will be invalid\n");
        fclose(index_fp);
        return -1;
    }

    unsigned int maxDuration = planned_segment_duration;

    for (i = first_segment; i <= last_segment; i++)
        if (actual_segment_duration[i] > maxDuration)
            maxDuration = actual_segment_duration[i];



    snprintf(write_buf, 1024, "#EXTM3U\n#EXT-X-TARGETDURATION:%u\n", maxDuration);

    if (fwrite(write_buf, strlen(write_buf), 1, index_fp) != 1) {
        fprintf(stderr, "Could not write to m3u8 index file, will not continue writing to index file\n");
        free(write_buf);
        fclose(index_fp);
        return -1;
    }

    for (i = first_segment; i <= last_segment; i++) {
        snprintf(write_buf, 1024, "#EXTINF:%u,\n%s%s-%u%s\n", actual_segment_duration[i], url_prefix, output_prefix, i, output_file_extension);
        if (fwrite(write_buf, strlen(write_buf), 1, index_fp) != 1) {
            fprintf(stderr, "Could not write to m3u8 index file, will not continue writing to index file\n");
            free(write_buf);
            fclose(index_fp);
            return -1;
        }
    }

    snprintf(write_buf, 1024, "#EXT-X-ENDLIST\n");
    if (fwrite(write_buf, strlen(write_buf), 1, index_fp) != 1) {
        fprintf(stderr, "Could not write last file and endlist tag to m3u8 index file\n");
        free(write_buf);
        fclose(index_fp);
        return -1;
    }

    free(write_buf);
    fclose(index_fp);

    return rename(tmp_index, index);
}

void output_transfer_command(const unsigned int first_segment, const unsigned int last_segment, unsigned int actual_segment_duration, const int end, const char *encoding_profile) {
  char buffer[1024 * 10];
  memset(buffer, 0, sizeof(char) * 1024 * 10);

  sprintf(buffer, "%d, %d, %d, %d, %s", first_segment, last_segment, actual_segment_duration, end, encoding_profile);

  fprintf(stderr, "segmenter: %s\n\r", buffer);
}

int main(int argc, const char *argv[]) {
    //input parameters
    char inputFilename[MAX_FILENAME_LENGTH], playlistFilename[MAX_FILENAME_LENGTH], baseDirName[MAX_FILENAME_LENGTH], baseFileName[MAX_FILENAME_LENGTH], encodingProfile[MAX_FILENAME_LENGTH], urlPrefix[MAX_FILENAME_LENGTH];
    char baseFileExtension[5]; //either "ts", "aac" or "mp3"
    int segmentLength, outputStreams, verbosity, version,usage,doid3;



    char currentOutputFileName[MAX_FILENAME_LENGTH];
    char tempPlaylistName[MAX_FILENAME_LENGTH];


    //these are used to determine the exact length of the current segment
    double prev_segment_time = 0;
    double segment_time;
    unsigned int actual_segment_durations[2048];
    double packet_time = 0;

    //new variables to keep track of output size
    double output_bytes = 0;

    unsigned int output_index = 1;
    AVOutputFormat *ofmt;
    AVFormatContext *ic = NULL;
    AVFormatContext *oc;
    AVStream *video_st = NULL;
    AVStream *audio_st = NULL;
    AVCodec *codec;
    int video_index;
    int audio_index;
    unsigned int first_segment = 1;
    unsigned int last_segment = 0;
    int write_index = 1;
    int decode_done;
    int ret;
    int i;

    unsigned char id3_tag[128];
    unsigned char * image_id3_tag;

    size_t id3_tag_size = 73;
    int newFile = 1; //a boolean value to flag when a new file needs id3 tag info in it

    if (parseCommandLine(inputFilename, playlistFilename, baseDirName, baseFileName, baseFileExtension, encodingProfile, urlPrefix, &outputStreams, &segmentLength, &verbosity, &version,&usage, &doid3,  argc, argv) != 0)
        return 0;

	if (usage) printUsage();
    if (version) ffmpeg_version();
	if (version || usage) return 0;

	if (doid3) {
		image_id3_tag = malloc(IMAGE_ID3_SIZE);
		if (outputStreams == OUTPUT_STREAM_AUDIO) build_image_id3_tag(image_id3_tag);
		build_id3_tag((char *) id3_tag, id3_tag_size);
	}

    snprintf(tempPlaylistName, strlen(playlistFilename) + strlen(baseDirName) + 1, "%s%s", baseDirName, playlistFilename);
    strncpy(playlistFilename, tempPlaylistName, strlen(tempPlaylistName));
    strncpy(tempPlaylistName, playlistFilename, MAX_FILENAME_LENGTH);
    strncat(tempPlaylistName, ".", 1);
    
    fprintf(stderr, "%s %s\n", playlistFilename, tempPlaylistName);

    //decide if this is an aac file or a mpegts file.
    //postpone deciding format until later
    /*	ifmt = av_find_input_format("mpegts");
    if (!ifmt) 
    {
    fprintf(stderr, "Could not find MPEG-TS demuxer.\n");
    exit(1);
    } */

    av_log_set_level(AV_LOG_DEBUG);

    av_register_all();
    
    if (!strcmp(inputFilename, "-")) {
        strcpy(inputFilename, "pipe:");
    }
    
    ret = avformat_open_input(&ic, inputFilename, NULL, NULL);
    if (ret != 0) {
        fprintf(stderr, "Could not open input file %s. Error %d.\n", inputFilename, ret);
        exit(1);
    }

    if (avformat_find_stream_info(ic, NULL) < 0) {
        fprintf(stderr, "Could not read stream information.\n");
        exit(1);
    }

    oc = avformat_alloc_context();
    if (!oc) {
        fprintf(stderr, "Could not allocate output context.");
        exit(1);
    }

    video_index = -1;
    audio_index = -1;

    for (i = 0; i < ic->nb_streams && (video_index < 0 || audio_index < 0); i++) {
        switch (ic->streams[i]->codec->codec_type) {
#ifdef USE_OLD_FFMPEG
            case CODEC_TYPE_VIDEO:
#else
            case AVMEDIA_TYPE_VIDEO:
#endif
                video_index = i;
                ic->streams[i]->discard = AVDISCARD_NONE;
                if (outputStreams & OUTPUT_STREAM_VIDEO)
                    video_st = add_output_stream(oc, ic->streams[i]);
                break;
#ifdef USE_OLD_FFMPEG
            case CODEC_TYPE_AUDIO:
#else
            case AVMEDIA_TYPE_AUDIO:
#endif
                audio_index = i;
                ic->streams[i]->discard = AVDISCARD_NONE;
                if (outputStreams & OUTPUT_STREAM_AUDIO)
                    audio_st = add_output_stream(oc, ic->streams[i]);
                break;
            default:
                ic->streams[i]->discard = AVDISCARD_ALL;
                break;
        }
    }

    if (video_index == -1) {
        fprintf(stderr, "Source stream must have video component.\n");
        exit(1);
    }

    //now that we know the audio and video output streams
    //we can decide on an output format.
    if (outputStreams == OUTPUT_STREAM_AUDIO) {
        //the audio output format should be the same as the audio input format
        switch (ic->streams[audio_index]->codec->codec_id) {
            case CODEC_ID_MP3:
                fprintf(stderr, "Setting output audio to mp3.");
                strncpy(baseFileExtension, ".mp3", strlen(".mp3"));
                ofmt = av_guess_format("mp3", NULL, NULL);
                break;
            case CODEC_ID_AAC:
                fprintf(stderr, "Setting output audio to aac.");
                ofmt = av_guess_format("adts", NULL, NULL);
                break;
            default:
                fprintf(stderr, "Codec id %d not supported.\n", ic->streams[audio_index]->id);
        }
        if (!ofmt) {
            fprintf(stderr, "Could not find audio muxer.\n");
            exit(1);
        }
    } else {
        ofmt = av_guess_format("mpegts", NULL, NULL);
        if (!ofmt) {
            fprintf(stderr, "Could not find MPEG-TS muxer.\n");
            exit(1);
        }
    }
    oc->oformat = ofmt;

    if (outputStreams & OUTPUT_STREAM_VIDEO && oc->oformat->flags & AVFMT_GLOBALHEADER) {
        oc->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    /*  pass the options to avformat_write_header directly. 
        if (av_set_parameters(oc, NULL) < 0) {
            fprintf(stderr, "Invalid output format parameters.\n");
            exit(1);
        }
     */

    av_dump_format(oc, 0, baseFileName, 1);


    //open the video codec only if there is video data
    if (video_index != -1) {
        if (outputStreams & OUTPUT_STREAM_VIDEO)
            codec = avcodec_find_decoder(video_st->codec->codec_id);
        else
            codec = avcodec_find_decoder(ic->streams[video_index]->codec->codec_id);
        if (!codec) {
            fprintf(stderr, "Could not find video decoder, key frames will not be honored.\n");
        }

        if (outputStreams & OUTPUT_STREAM_VIDEO)
            ret = avcodec_open2(video_st->codec, codec, NULL);
        else
            avcodec_open2(ic->streams[video_index]->codec, codec, NULL);
        if (ret < 0) {
            fprintf(stderr, "Could not open video decoder, key frames will not be honored.\n");
        }
    }

    snprintf(currentOutputFileName, strlen(baseDirName) + strlen(baseFileName) + strlen(baseFileExtension) + 10, "%s%s-%u%s", baseDirName, baseFileName, output_index++, baseFileExtension);

    if (avio_open(&oc->pb, currentOutputFileName,AVIO_FLAG_WRITE) < 0) {
        fprintf(stderr, "Could not open '%s'.\n", currentOutputFileName);
        exit(1);
    }
    newFile = 1;

    int r = avformat_write_header(oc, NULL);
    if (r) {
        fprintf(stderr, "Could not write mpegts header to first output file.\n");
        debugReturnCode(r);
        exit(1);
    }

    //no segment info is written here. This just creates the shell of the playlist file
    write_index = !write_index_file(playlistFilename, tempPlaylistName, segmentLength, actual_segment_durations, baseDirName, urlPrefix, baseFileName, baseFileExtension, first_segment, last_segment);

    do {
        AVPacket packet;

        decode_done = av_read_frame(ic, &packet);

        if (decode_done < 0) {
            break;
        }

        if (av_dup_packet(&packet) < 0) {
            fprintf(stderr, "Could not duplicate packet.");
            av_free_packet(&packet);
            break;
        }

        //this time is used to check for a break in the segments
        //	if (packet.stream_index == video_index && (packet.flags & PKT_FLAG_KEY)) 
        //	{
        //    segment_time = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;			
        //	}
#if USE_OLD_FFMPEG
        if (packet.stream_index == video_index && (packet.flags & PKT_FLAG_KEY))
#else
        if (packet.stream_index == video_index && (packet.flags & AV_PKT_FLAG_KEY))
#endif		
        {
            segment_time = (double) packet.pts * ic->streams[video_index]->time_base.num / ic->streams[video_index]->time_base.den;
        }
        //  else if (video_index < 0) 
        //	{
        //		segment_time = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
        //	}

        //get the most recent packet time
        //this time is used when the time for the final segment is printed. It may not be on the edge of
        //of a keyframe!
        if (packet.stream_index == video_index)
            packet_time = (double) packet.pts * ic->streams[video_index]->time_base.num / ic->streams[video_index]->time_base.den; //(double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
        else if (outputStreams & OUTPUT_STREAM_AUDIO)
            packet_time = (double) audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
        else
            continue;
        //start looking for segment splits for videos one half second before segment duration expires. This is because the 
        //segments are split on key frames so we cannot expect all segments to be split exactly equally. 
        if (segment_time - prev_segment_time >= segmentLength - 0.5) {
            fprintf(stderr, "looking to print index file at time %lf\n", segment_time);
            avio_flush(oc->pb);
            avio_close(oc->pb);

            if (write_index) {
                actual_segment_durations[++last_segment] = (unsigned int) rint(segment_time - prev_segment_time);
                write_index = !write_index_file(playlistFilename, tempPlaylistName, segmentLength, actual_segment_durations, baseDirName, urlPrefix, baseFileName, baseFileExtension, first_segment, last_segment);
                fprintf(stderr, "Writing index file at time %lf\n", packet_time);
                output_transfer_command(first_segment, last_segment, actual_segment_durations[last_segment], 0, encodingProfile);
            }

            struct stat st;
            stat(currentOutputFileName, &st);
            output_bytes += st.st_size;

            snprintf(currentOutputFileName, strlen(baseDirName) + strlen(baseFileName) + strlen(baseFileExtension) + 10, "%s%s-%u%s", baseDirName, baseFileName, output_index++, baseFileExtension);
            if (avio_open(&oc->pb, currentOutputFileName, AVIO_FLAG_WRITE) < 0) {
                fprintf(stderr, "Could not open '%s'\n", currentOutputFileName);
                break;
            }

            newFile = 1;
            prev_segment_time = segment_time;
        }

        if (outputStreams == OUTPUT_STREAM_AUDIO && packet.stream_index == audio_index) {
            if (newFile && outputStreams == OUTPUT_STREAM_AUDIO) {
                //add id3 tag info
                //fprintf(stderr, "adding id3tag to file %s\n", currentOutputFileName);
                //printf("%lf %lld %lld %lld %lld %lld %lf\n", segment_time, audio_st->pts.val, audio_st->cur_dts, audio_st->cur_pkt.pts, packet.pts, packet.dts, packet.dts * av_q2d(ic->streams[audio_index]->time_base) );
                if (doid3) {
					fill_id3_tag((char*) id3_tag, id3_tag_size, packet.dts);
					avio_write(oc->pb, id3_tag, id3_tag_size);
					avio_write(oc->pb, image_id3_tag, IMAGE_ID3_SIZE);
					avio_flush(oc->pb);
				}
                newFile = 0;
            }

            packet.stream_index = 0; //only one stream in audio only segments
            ret = av_interleaved_write_frame(oc, &packet);
        } else if (outputStreams & OUTPUT_STREAM_VIDEO) {
            if (newFile) {
                //fprintf(stderr, "New File: %lld %lld %lld\n", packet.pts, video_st->pts.val, audio_st->pts.val);
                //printf("%lf %lld %lld %lld %lld %lld %lf\n", segment_time, audio_st->pts.val, audio_st->cur_dts, audio_st->cur_pkt.pts, packet.pts, packet.dts, packet.dts * av_q2d(ic->streams[audio_index]->time_base) );
                newFile = 0;
            }
            if (outputStreams == OUTPUT_STREAM_VIDEO)
                ret = av_write_frame(oc, &packet);
            else
                ret = av_interleaved_write_frame(oc, &packet);
        }

        if (ret < 0) {
            fprintf(stderr, "Warning: Could not write frame of stream.\n");
        } else if (ret > 0) {
            fprintf(stderr, "End of stream requested.\n");
            av_free_packet(&packet);
            break;
        }

        av_free_packet(&packet);
    } while (!decode_done);

    //make sure all packets are written and then close the last file. 
    avio_flush(oc->pb);
    av_write_trailer(oc);

    if (video_st && (video_st->codec->codec !=NULL))
        avcodec_close(video_st->codec);

    if (audio_st && (audio_st->codec->codec != NULL)){
        avcodec_close(audio_st->codec);
    }

    for (i = 0; i < oc->nb_streams; i++) {
        av_freep(&oc->streams[i]->codec);
        av_freep(&oc->streams[i]);
    }

    avio_close(oc->pb);
    av_free(oc);

    struct stat st;
    stat(currentOutputFileName, &st);
    output_bytes += st.st_size;


    if (write_index) {
        actual_segment_durations[++last_segment] = (unsigned int) rint(packet_time - prev_segment_time);

        //make sure that the last segment length is not zero
        if (actual_segment_durations[last_segment] == 0)
            actual_segment_durations[last_segment] = 1;

        write_index_file(playlistFilename, tempPlaylistName, segmentLength, actual_segment_durations, baseDirName, urlPrefix, baseFileName, baseFileExtension, first_segment, last_segment);
        
        output_transfer_command(first_segment, last_segment, actual_segment_durations[last_segment],  1, encodingProfile);
    }

    //write_stream_size_file(baseDirName, baseFileName, output_bytes * 8 / segment_time);
    

    return 0;
}
