//#include <stdio.h>
//#include <stdlib.h>
//#include <sndfile.h>
//
//int LoadSoundFile(const char *filename, int *rate, int *channels,
//    int *bits, u_int8_t **buf, int *buflen) {
//    SNDFILE *file;
//    SF_INFO file_info;
//    short *buffer_short = NULL;
//    u_int8_t *buffer_8 = NULL;
//    int16_t *buffer_16 = NULL;
//    int i;
//    
//    file = sf_open(filename, SFM_READ, &file_info);
//    if (file == NULL) {
//        printf("Unable to open '%s'.\n", filename);
//        return -1;
//    }
//    
//    if (sf_format_check(&file_info) == -1) {
//        printf("'%s' is not a PCM-based audio file.\n", filename);
//        sf_close(file);
//        return -1;
//    }
//    
//    buffer_short = (short*)malloc(file_info.channels * file_info.frames * sizeof(short));
//    
//    if (sf_readf_short(file, buffer_short, file_info.frames) == -1) {
//        printf("Error while reading samples from '%s'.\n", filename);
//        sf_close(file);
//        free(buffer_short);
//        return -1;
//    }
//    
//    sf_close(file);
//    free(buffer_short);
//    return 0;
//}