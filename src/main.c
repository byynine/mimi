#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// TODO: ADD indices to reminders and add remove option

// filepaths for storing reminder data
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
    // display reminders when called and return
    if (argc == 1)
    {
        FILE* mimi_data = fopen(mimi_data_filepath, "r");
        if (!mimi_data) { printf("error: couldn't read file %s\n", mimi_data_filepath); return 1; }

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

    if (argc > 3) { printf("error: too many arguments\n"); return 1; }

    // TODO: isolate into function
    // this appends a reminder object to data
    if (!strcmp(argv[1], kw_remind))
    {
        if (argc != 3) { printf("error: command %s expects one more argument\n", kw_remind); }

        reminder rmn;

        if (strlen(argv[2]) >= sizeof(rmn.desc))
        {
            printf("error: description too big\n");
            return 1;
        }

        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        if (mkdir(mimi_data_dirpath, 0755) == -1)
            if (errno != EEXIST)
            {
                printf("error: failure when creating directory %s", mimi_data_dirpath);
                return 1;
            }

        FILE* mimi_data = fopen(mimi_data_filepath, "a");
        if (!mimi_data) { printf("error: couldn't append to file %s\n", mimi_data_filepath); return 1; }

        fprintf(mimi_data, reminder_format_out, rmn.desc);
    }
    // this prints the reminders in data
    // TODO: isolate into function
    else if (!strcmp(argv[1], kw_list))
    {
        FILE* mimi_data = fopen(mimi_data_filepath, "r");
        if (!mimi_data) { printf("error: couldn't read file %s", mimi_data_filepath); return 1; }

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, reminder_format_in, desc) == 1) printf("%s\n", desc);
        }
    }
    else
    {
        printf("error: unknown command\n");
        return 1;
    }

    return 0;
}
