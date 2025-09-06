#ifndef TAG_H
#define TAG_H

// Status enum for success/failure
typedef enum
{
    READ_FAILURE = 0,
    READ_SUCCESS = 1
} Status;

// Struct to hold common MP3 tag metadata
typedef struct
{
    char title[128];
    char artist[128];
    char album[128];
    char year[5];           // 4 characters for year + null terminator
    char composer[128];
    char content_type[64];  // genre
} ID3v2Tag;

// Function prototypes
Status read_mp3_tag(const char *filename, ID3v2Tag *tag);
void print_tag(const ID3v2Tag *tag);
Status edit_mp3_tag(const char *filename, const char *frame_id, const char *new_value);
int convert_big_to_little_endian(unsigned char *bytes);

#endif // TAG_H