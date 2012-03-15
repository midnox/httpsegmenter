/*
 * Copyright (c) 2012 Stoian Ivanov
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
#include <errno.h>
#include <ctype.h>


#include "segmenter.h"
//Type of options detected
typedef enum {
	INPUT_FILE, OUTPUT_FILE, OUTPUT_DIR, OUTPUT_BASE_NAME, SEGMENT_LENGTH, ENCODING_PROFILE, URL_PREFIX,
	
	OUTPUT_AUDIO_ONLY,OUTPUT_VIDEO_ONLY,OUTPUT_VERBOSITY,ENABLE_ID3TAGS,
	
	VERSION,PRINT_USAGE,
	
	NO_MORE_OPTIONS,INVALID
	
} inputOption;

void printBanner() {
	static int  bannerPrinted= 1;
	if (bannerPrinted==1) {
		bannerPrinted++;
		fprintf(stderr,"Segmenter for HTTP Live Streaming v" PROGRAM_VERSION "\n");
		fprintf(stderr,"part of HTTP-Live-Video-Stream-Segmenter-and-Distributor @ github\n\n");
	}
}
void printUsage_short(int shortonly){
	printBanner();
	if (shortonly) fprintf(stderr, "\n");
	fprintf(stderr, "Usage: segmenter -i infile [-d baseDir] [-f baseFileName] [-o playListFile.m3u8] [-l 10] [-a|-v] [-t] \n");
	if (shortonly) fprintf(stderr, "  segmenter -h for full help\n");
}

void printUsage() {
	printUsage_short(0);
	fprintf(stderr, "\nOptions (you can use -- or - for short option prefix e.g. -i == --i): \n");
	fprintf(stderr, "-i <infile>\t\tInput file. Required.\n");
	fprintf(stderr, "-o <outfile>\t\tPlaylist file to create. Default is <infile>.m3u8 \n");
	fprintf(stderr, "-d <basedir>\t\tThe base directory for files. Default is  '.'\n");
	fprintf(stderr, "-f <baseFileName>\tSegment files will be named <baseFileName>-#. Default is <infile>\n");
	fprintf(stderr, "-l <segment length>\tThe length of each segment. Default is 5\n");
	fprintf(stderr, "-p <encoding profile>\t\tThe name of the encoding profile\n");
	fprintf(stderr, "-u <url prefix>\t\tThe URL prefix\n");
	fprintf(stderr, "-t\t\t\tEnable id3 tagging code (EXPERIMENTAL)\n");
	fprintf(stderr, "-a\t\t\taudio only decode for < 64k streams.\n");
	fprintf(stderr, "-v\t\t\tvideo only decode for < 64k streams.\n\n");
	fprintf(stderr, "--version\t\tPrint version details and exit.\n");
	fprintf(stderr, "-h,--help\t\tPrint this info.\n");
	fprintf(stderr, "\n\n");
}


inputOption getNextOption(int argc, const char * argv[], char * option) {
    static int optionIndex = 1;

    if (optionIndex >= argc)
        return NO_MORE_OPTIONS;
	int hasnext=(optionIndex<argc-1);
    if (strcmp(argv[optionIndex],"-i")==0 || strcmp(argv[optionIndex],"--i")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: input filename must be given after -i\n");
            return INVALID;
		}
		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);
        option[MAX_FILENAME_LENGTH - 1] = 0;

        //check for valid file name
        if (!isalpha(option[0]) && !isdigit(option[0]) && option[0] != '-' && option[0] != '/' && option[0] != '.') {
            fprintf(stderr,"ERROR: input filename must start with alpha, digit, / or . \n");
            return INVALID;
        }

        optionIndex++;
        return INPUT_FILE;
    }

   if (strcmp(argv[optionIndex],"-o")==0 || strcmp(argv[optionIndex],"--o")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: output filename must be given after -o\n");
            return INVALID;
		}
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);

        //check for valid file name
        if (!isalpha(option[0]) && !isdigit(option[0]) && option[0] != '-' && option[0] != '/' && option[0] != '.') {
            fprintf(stderr,"ERROR: output filename must start with alpha, digit, / or . \n");
            return INVALID;
        }

        if (option[0] != '-' && strncmp(option + (strlen(option) - 5), ".m3u8", 5) != 0) {
            fprintf(stderr,"WARN: output file extension should be .m3u8\n\n");
            //return INVALID;
        }

        optionIndex++;
        return OUTPUT_FILE;
    }
    
	
    if (strcmp(argv[optionIndex],"-d")==0 || strcmp(argv[optionIndex],"--d")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: output directory must be given after -d\n");
            return INVALID;
		}
		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 2);

        //make sure it ends in a slash
        int l = strnlen(option,MAX_FILENAME_LENGTH - 2);
        if (option[l - 1] != '/') {
            option[l] = '/';
            option[l + 1] = 0;
        }

        //check for valid file name
        if (!isalpha(option[0]) && !isdigit(option[0]) && option[0] != '-' && option[0] != '/' && option[0] != '.') {
            fprintf(stderr,"ERROR: output directory must start with alpha, digit, / or . \n");
            return INVALID;
        }

        optionIndex++;
        return OUTPUT_DIR;
    }


    if (strcmp(argv[optionIndex],"-f")==0 || strcmp(argv[optionIndex],"--f")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: output base filename must be given after -f\n");
            return INVALID;
		}
		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);

        //check for valid file name
        if (!isalpha(option[0]) && !isdigit(option[0]) && option[0] != '-' && option[0] != '/' && option[0] != '.') {
            fprintf(stderr,"ERROR: output base filename must start with alpha, digit, / or . \n");
            return INVALID;
        }

        optionIndex++;
        return OUTPUT_BASE_NAME;
    }
    
    if (strcmp(argv[optionIndex],"-p")==0 || strcmp(argv[optionIndex],"--p")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: encoding profile must be given after -p\n");
            return INVALID;
		}
		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);

        optionIndex++;
        return ENCODING_PROFILE;
    }

    if (strcmp(argv[optionIndex],"-u")==0 || strcmp(argv[optionIndex],"--u")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: url prefix must be given after -u\n");
            return INVALID;
		}
		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);

        optionIndex++;
        return URL_PREFIX;
    }

    if (strcmp(argv[optionIndex],"-a")==0 || strcmp(argv[optionIndex],"--a")==0) {
        optionIndex++;
        return OUTPUT_AUDIO_ONLY;
    }
	if (strcmp(argv[optionIndex],"-v")==0 || strcmp(argv[optionIndex],"--v")==0) {
        optionIndex++;
        return OUTPUT_VIDEO_ONLY;
	}


    if (strcmp(argv[optionIndex],"-l")==0 || strcmp(argv[optionIndex],"--l")==0) {
		if (!hasnext){
            fprintf(stderr,"ERROR: length must be given after -l\n");
            return INVALID;
		}		
        strncpy(option, argv[++optionIndex], MAX_FILENAME_LENGTH - 1);
        option[MAX_FILENAME_LENGTH - 1] = 0;

        int a = strtol(option, NULL, 10);
        if (a == 0 && errno != 0) {
            fprintf(stderr,"ERROR: length must be an integer.\n");
            return INVALID;
        }

        if (a <= 2) {
            fprintf(stderr, "ERROR: Segment duration time must be > 2.\n");
            return INVALID;
        }

        optionIndex++;
        return SEGMENT_LENGTH;
    }
    
    if (strcmp(argv[optionIndex],"--version")==0 ) {
        optionIndex++;
        return VERSION;
    }

	if (strcmp(argv[optionIndex],"-h")==0 || strcmp(argv[optionIndex],"--help")==0) {
        optionIndex++;
		return PRINT_USAGE;
	}
	if (strcmp(argv[optionIndex],"-t")==0 || strcmp(argv[optionIndex],"--t")==0) {
        optionIndex++;
		return ENABLE_ID3TAGS;
	}
	
	fprintf(stderr, "ERROR: Unknown option %s\n",argv[optionIndex]);
	
    return INVALID;
}


#define INPUT_FILE_INDEX 0
#define OUTPUT_FILE_INDEX 1
#define OUTPUT_DIR_INDEX 2
#define OUTPUT_BASE_NAME_INDEX 3
#define OUTPUT_SEGMENT_LENGTH_INDEX 4
#define ENCODING_PROFILE_INDEX 5

//assumes that the pointers have allocated memory

int parseCommandLine(
	char * inputFile, char * outputFile, char * baseDir, char * baseName, char * baseExtension, char * encodingProfile, char * urlPrefix,
	int * outputStreams, int * segmentLength, int * verbosity, int * version, int * usage, int * doid3tag,
	
	int argc, const char * argv[]
) {
	printBanner();
	
    int requiredOptions[5] = {0, 0, 0, 0, 0};
    encodingProfile[0] = '\0';
    urlPrefix[0] = '\0';

    inputOption result;
    char option[MAX_FILENAME_LENGTH];

    //default video and audio output
    *verbosity = 0;
    *version = 0;
	*usage = 0;
	*doid3tag = 0;
    *outputStreams = OUTPUT_STREAM_AV;
    strncpy(baseExtension, ".ts", strlen(".ts")+1);

    while (1) {
        result = getNextOption(argc, argv, option);
        switch (result) {
            case INPUT_FILE:
                strncpy(inputFile, option, MAX_FILENAME_LENGTH);
                requiredOptions[INPUT_FILE_INDEX] = 1;
                break;
            case OUTPUT_FILE:
                strncpy(outputFile, option, MAX_FILENAME_LENGTH);
                requiredOptions[OUTPUT_FILE_INDEX] = 1;
                break;
            case OUTPUT_DIR:
                strncpy(baseDir, option, MAX_FILENAME_LENGTH);
                requiredOptions[OUTPUT_DIR_INDEX] = 1;
                break;
            case OUTPUT_BASE_NAME:
                strncpy(baseName, option, MAX_FILENAME_LENGTH);
                requiredOptions[OUTPUT_BASE_NAME_INDEX] = 1;
                break;
            case ENCODING_PROFILE:
                strncpy(encodingProfile, option, MAX_FILENAME_LENGTH);
                break;
            case URL_PREFIX:
                strncpy(urlPrefix, option, MAX_FILENAME_LENGTH);
                break;
            case OUTPUT_AUDIO_ONLY:
				if (*outputStreams != OUTPUT_STREAM_AV) {
					fprintf(stderr, "ERROR: Conflicting -v and -a.\n");
					return -1;
				} else {
					*outputStreams = OUTPUT_STREAM_AUDIO;
					strncpy(baseExtension, ".aac", strlen(".aac"));
				}
                break;
            case OUTPUT_VIDEO_ONLY:
				if (*outputStreams != OUTPUT_STREAM_AV) {
					fprintf(stderr, "ERROR: Conflicting -v and -a.\n");
					return -1;
				} else {
					*outputStreams = OUTPUT_STREAM_VIDEO;
					break;
				}
            case ENABLE_ID3TAGS:
                *doid3tag = 1;
                break;
            case SEGMENT_LENGTH:
                *segmentLength = strtol(option, NULL, 10);
                requiredOptions[OUTPUT_SEGMENT_LENGTH_INDEX] = 1;
            case OUTPUT_VERBOSITY:
                *verbosity = strtol(option, NULL, 10);
                break;
            case VERSION:
                *version = 1;
                break;
            case PRINT_USAGE:
                *usage = 1;
                break;
            case NO_MORE_OPTIONS:
            {
				if (argc==1) *usage=1;
                if (*version == 1 || *usage == 1) 
                    return 0;

                int missing = 0;

                if (requiredOptions[INPUT_FILE_INDEX] == 0) {
                    fprintf(stderr, "ERROR: Missing required option --i for input file.\n");
                    missing = 1;
                }
                if (requiredOptions[OUTPUT_FILE_INDEX] == 0) {
                    fprintf(stderr, "ERROR: Missing required option --o for output playlist file.\n");
                    missing = 1;
                }
                if (requiredOptions[OUTPUT_DIR_INDEX] == 0) {
                    fprintf(stderr, "ERROR: Missing required option --d for output base directory.\n");
                    missing = 1;
                }
                if (requiredOptions[OUTPUT_BASE_NAME_INDEX] == 0) {
                    fprintf(stderr, "ERROR: Missing required option --f for file base name.\n");
                    missing = 1;
                }
                if (requiredOptions[OUTPUT_SEGMENT_LENGTH_INDEX] == 0) {
                    fprintf(stderr, "ERROR: Missing required option --l for segment length.\n");
                    missing = 1;
                }

                if (missing == 1) {
                    printUsage_short(1);
                    return -1;
                }

                return 0;
            }
            case INVALID:
            default:
                return -1;
        }
    }

    return 0;
}
