#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tag.h"
 /*
 Name: Rehan Mokashi
 Date: 08/08/2025
 Description: Mp3 Tag Reaer Project
 */

// Show usage info
void print_usage(const char *program)
{
    printf("Usage:\n");
    printf("  %s -v <mp3_filename>\n", program);
    printf("  %s -e -<option> <new_value> <mp3_filename>\n", program);
    printf("Options for -e:\n");
    printf("  -t   Edit Title\n");
    printf("  -a   Edit Artist\n");
    printf("  -A   Edit Album\n");
    printf("  -y   Edit Year\n");
    printf("  -c   Edit Composer\n");
    printf("  -g   Edit Genre\n");
}

// Check if file has .mp3 extension
int has_mp3_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return (dot && strcmp(dot, ".mp3") == 0);
}


// Validate year is digits only
int is_valid_year(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return strlen(str) == 4; // typical year is 4 digits
}

int main(int argc, char *argv[])
{
    
    // View tags
    if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        if (!has_mp3_extension(argv[2]))  // For view case
        {
            printf("❌ Error: Only .mp3 files are allowed!\n");
            return READ_FAILURE;
        }

        ID3v2Tag tag = {0};
        if (read_mp3_tag(argv[2], &tag) == READ_SUCCESS)
        {
            print_tag(&tag);
        }
        else
        {
            printf("❌ Failed to read tags from: %s\n", argv[2]);
            return READ_FAILURE;
        }
    }
    // Edit tag
    else if (argc == 5 && strcmp(argv[1], "-e") == 0)
    {
        const char *edit_flag = argv[2];
        const char *new_value = argv[3];
        const char *filename = argv[4];
        const char *frame_id = NULL;

        if (!has_mp3_extension(filename))  // For edit case
        {
            printf("❌ Error: Only .mp3 files are allowed!\n");
            return READ_FAILURE;
        }


        if (strcmp(edit_flag, "-t") == 0)
        {
            frame_id = "TIT2";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Title change option------------------------\n");
        }
        else if (strcmp(edit_flag, "-a") == 0)
        {
            frame_id = "TPE1";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Artist change option-----------------------\n");
        }
        else if (strcmp(edit_flag, "-A") == 0)
        {
            frame_id = "TALB";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Album change option------------------------\n");
        }
        else if (strcmp(edit_flag, "-y") == 0)
        {
            if (!is_valid_year(new_value))
            {
                printf("❌ Error: Year should be 4 digits only.\n");
                return READ_FAILURE;
            }
            frame_id = "TYER";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Year change option-------------------------\n");
        }
        else if (strcmp(edit_flag, "-c") == 0)
        {
            frame_id = "TCOM";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Composer change option---------------------\n");
        }
        else if (strcmp(edit_flag, "-g") == 0)
        {
            frame_id = "TCON";
            printf("-----------------------------Select Edit option-----------------------------\n");
            printf("--------------------------Select Genre change option------------------------\n");
        }
        else
        {
            printf("❌ Unknown edit flag: %s\n", edit_flag);
            print_usage(argv[0]);
            return READ_FAILURE;
        }

        if (edit_mp3_tag(filename, frame_id, new_value) == READ_SUCCESS)
        {
            printf("%s        : %s\n", (strcmp(frame_id, "TIT2") == 0 ? "Title" :
                                      strcmp(frame_id, "TPE1") == 0 ? "Artist" :
                                      strcmp(frame_id, "TALB") == 0 ? "Album" :
                                      strcmp(frame_id, "TYER") == 0 ? "Year" :
                                      strcmp(frame_id, "TCOM") == 0 ? "Composer" :
                                      strcmp(frame_id, "TCON") == 0 ? "Content Type" :
                                      "Unknown"),
                   new_value);
            printf("--------------------------%s changed successfully--------------------------\n",
                   (strcmp(frame_id, "TIT2") == 0 ? "Title" :
                    strcmp(frame_id, "TPE1") == 0 ? "Artist" :
                    strcmp(frame_id, "TALB") == 0 ? "Album" :
                    strcmp(frame_id, "TYER") == 0 ? "Year" :
                    strcmp(frame_id, "TCOM") == 0 ? "Composer" :
                    strcmp(frame_id, "TCON") == 0 ? "Genre" :
                    "Field"));
        }
        else
        {
            printf("❌ Failed to update tag '%s'.\n", frame_id);
            return READ_FAILURE;
        }
    }
    else
    {
        print_usage(argv[0]);
        return READ_FAILURE;
    }

    return READ_SUCCESS;
}
