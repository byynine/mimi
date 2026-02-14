#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// TODO: ADD indices to reminders and add remove option

// filepaths for storing reminder data
#define MIMI_DATA_FILEPATH "/home/nine/.local/share/mimi/data"
#define MIMI_DATA_DIRPATH "/home/nine/.local/share/mimi"

// command keywords for when calling from shell
#define CMD_REMIND "remind"
#define CMD_LIST "list"

// serialization i/o formats
#define REMINDER_FORMAT_IN "\"%63[^\"]\"\n"
#define REMINDER_FORMAT_OUT "\"%s\"\n"

typedef struct reminder
{
    char desc[64];
} reminder;

int main(int argc, char *argv[])
{
    // display reminders when called and return
    if (argc == 1)
    {
        FILE* mimi_data = fopen(MIMI_DATA_FILEPATH, "r");
        if (!mimi_data) { printf("error: couldn't read file %s\n", MIMI_DATA_FILEPATH); return 1; }

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, REMINDER_FORMAT_IN, desc) == 1) printf("%s\n", desc);
        }

        return 0;
    }

    if (argc > 3) { printf("error: too many arguments\n"); return 1; }

    // TODO: isolate into function
    // this appends a reminder object to data
    if (!strcmp(argv[1], CMD_REMIND))
    {
        if (argc != 3) { printf("error: command %s expects one more argument\n", CMD_REMIND); }

        reminder rmn;

        if (strlen(argv[2]) >= sizeof(rmn.desc))
        {
            printf("error: description too big\n");
            return 1;
        }

        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        if (mkdir(MIMI_DATA_DIRPATH, 0755) == -1)
            if (errno != EEXIST)
            {
                printf("error: failure when creating directory %s", MIMI_DATA_DIRPATH);
                return 1;
            }

        FILE* mimi_data = fopen(MIMI_DATA_FILEPATH, "a");
        if (!mimi_data) { printf("error: couldn't append to file %s\n", MIMI_DATA_FILEPATH); return 1; }

        fprintf(mimi_data, REMINDER_FORMAT_OUT, rmn.desc);
    }
    // this prints the reminders in data
    // TODO: isolate into function
    else if (!strcmp(argv[1], CMD_LIST))
    {
        FILE* mimi_data = fopen(MIMI_DATA_FILEPATH, "r");
        if (!mimi_data) { printf("error: couldn't read file %s", MIMI_DATA_FILEPATH); return 1; }

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, REMINDER_FORMAT_IN, desc) == 1) printf("%s\n", desc);
        }
    }
    else
    {
        printf("error: unknown command\n");
        return 1;
    }

    return 0;
}
