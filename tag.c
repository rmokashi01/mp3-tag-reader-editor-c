#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for remove() and rename()
#include "tag.h"

int convert_big_to_little_endian(unsigned char *bytes)
{
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

void write_little_endian_to_big(int value, unsigned char *bytes)
{
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
}

Status read_mp3_tag(const char *filename, ID3v2Tag *tag)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("Error opening file");
        return READ_FAILURE;
    }

    fseek(fp, 10, SEEK_SET);

    while (!feof(fp))
    {
        unsigned char frame_header[10];
        if (fread(frame_header, 1, 10, fp) != 10)
            break;

        char frame_id[5] = {0};
        strncpy(frame_id, (char *)frame_header, 4);
        int frame_size = convert_big_to_little_endian(&frame_header[4]);
        if (frame_size < 1 || frame_size > 1024)
            break;

        fgetc(fp); // skip encoding byte
        char buffer[1024] = {0};
        fread(buffer, 1, frame_size - 1, fp);

        if (strcmp(frame_id, "TIT2") == 0)
            strncpy(tag->title, buffer, sizeof(tag->title));
        else if (strcmp(frame_id, "TPE1") == 0)
            strncpy(tag->artist, buffer, sizeof(tag->artist));
        else if (strcmp(frame_id, "TALB") == 0)
            strncpy(tag->album, buffer, sizeof(tag->album));
        else if (strcmp(frame_id, "TYER") == 0)
            strncpy(tag->year, buffer, sizeof(tag->year));
        else if (strcmp(frame_id, "TCOM") == 0)
            strncpy(tag->composer, buffer, sizeof(tag->composer));
        else if (strcmp(frame_id, "TCON") == 0)
            strncpy(tag->content_type, buffer, sizeof(tag->content_type));
    }

    fclose(fp);
    return READ_SUCCESS;
}

void print_tag(const ID3v2Tag *tag)
{
    printf("Title       : %s\n", tag->title);
    printf("Artist      : %s\n", tag->artist);
    printf("Album       : %s\n", tag->album);
    printf("Year        : %s\n", tag->year);
    printf("Composer    : %s\n", tag->composer);
    printf("Content Type: %s\n", tag->content_type);
}

Status edit_mp3_tag(const char *filename, const char *frame_id, const char *new_value)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("Error opening original file");
        return READ_FAILURE;
    }

    // Load entire file into memory
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (!buffer)
    {
        fclose(fp);
        return READ_FAILURE;
    }
    fread(buffer, 1, file_size, fp);
    fclose(fp);

    // Find ID3 header size
    int tag_size = ((buffer[6] & 0x7F) << 21) | ((buffer[7] & 0x7F) << 14) |
                   ((buffer[8] & 0x7F) << 7) | (buffer[9] & 0x7F);

    long pos = 10; // Skip ID3 header
    while (pos + 10 < file_size)
    {
        char id[5] = {0};
        memcpy(id, &buffer[pos], 4);

        int frame_size = convert_big_to_little_endian(&buffer[pos + 4]);
        if (frame_size <= 0 || frame_size > 1024)
            break;

        if (strncmp(id, frame_id, 4) == 0)
        {
            int new_len = strlen(new_value);
            int diff = new_len - (frame_size - 1); // -1 for encoding byte
            int new_tag_size = tag_size + diff;

            long new_file_size = file_size + diff;
            unsigned char *new_buf = (unsigned char *)malloc(new_file_size);
            if (!new_buf)
                return READ_FAILURE;

            // Copy everything up to the frame
            memcpy(new_buf, buffer, pos + 10 + 1);
            memcpy(&new_buf[pos + 10 + 1], new_value, new_len);

            // Fill any extra space
            for (int i = new_len; i < frame_size - 1 + diff; i++)
                new_buf[pos + 11 + i] = 0;

            // Copy the rest
            memcpy(&new_buf[pos + 10 + 1 + new_len], &buffer[pos + 10 + frame_size], file_size - (pos + 10 + frame_size));

            // Update frame size
            write_little_endian_to_big(new_len + 1, &new_buf[pos + 4]);

            // Update ID3 header size
            new_tag_size = new_tag_size & 0x0FFFFFFF;
            new_buf[6] = (new_tag_size >> 21) & 0x7F;
            new_buf[7] = (new_tag_size >> 14) & 0x7F;
            new_buf[8] = (new_tag_size >> 7) & 0x7F;
            new_buf[9] = new_tag_size & 0x7F;

            // Write to temp file
            char temp_file[256];
            snprintf(temp_file, sizeof(temp_file), "%s_temp.mp3", filename);
            FILE *out = fopen(temp_file, "wb");
            fwrite(new_buf, 1, new_file_size, out);
            fclose(out);

            free(buffer);
            free(new_buf);

            // Replace original file
            remove(filename);
            rename(temp_file, filename);
            return READ_SUCCESS;
        }
        pos += 10 + frame_size;
    }

    free(buffer);
    return READ_FAILURE;
}
