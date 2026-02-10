#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// TODO: add error messages and clean up return values

#define mimi_data_filepath "/home/nine/.local/share/mimi/data"
#define mimi_data_dirpath "/home/nine/.local/share/mimi"

// command keywords for when calling from shell
#define kw_remind "remind"
#define kw_list "list"

// serialization i/o formats
#define reminder_format_in "\"%63[^\"]\"\n"
#define reminder_format_out "\"%s\"\n"

typedef struct reminder
{
    char desc[64];
} reminder;

int main(int argc, char *argv[])
{
    // TODO: display reminders when called and return
    if (argc == 1)
    {
        FILE* mimi_data = fopen(mimi_data_filepath, "r");
        if (!mimi_data) return 1;

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, reminder_format_in, desc) == 1) printf("%s\n", desc);
        }

        return 0;
    }

    if (argc > 3) return 2;

    // TODO: isolate into function
    // TODO: add a check for hitting max amount of characters (bigger than buffer)
    // this appends a reminder object to data
    if (!strcmp(argv[1], kw_remind))
    {
        if (argc != 3) return 3;

        reminder rmn;

        if (strlen(argv[2]) >= sizeof(rmn.desc))
        {
            printf("description too big\n");
            return 1;
        }

        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        if (mkdir(mimi_data_dirpath, 0755) == -1) if (errno != EEXIST) return -1;
        FILE* mimi_data = fopen(mimi_data_filepath, "a");
        if (!mimi_data) return 4;

        fprintf(mimi_data, reminder_format_out, rmn.desc);
    }
    // this prints the reminders in data
    // TODO: isolate into function
    else if (!strcmp(argv[1], kw_list))
    {
        FILE* mimi_data = fopen(mimi_data_filepath, "r");
        if (!mimi_data) return 5;

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, reminder_format_in, desc) == 1) printf("%s\n", desc);
        }
    }

    return 0;
}
