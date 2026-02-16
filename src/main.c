#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// Filepaths for storing reminder data.
#define DATA_FILEPATH      "/home/nine/.local/share/mimi/data"
#define DATA_DIRPATH       "/home/nine/.local/share/mimi"
#define DATA_TEMP_FILEPATH "/home/nine/.local/share/mimi/temp"

// Commands for when calling from shell.
#define CMD_HELP0   "help"
#define CMD_HELP1   "h"
#define CMD_HELP2   "-h"
#define CMD_HELP3   "--help"
#define CMD_REMIND0 "remind"
#define CMD_REMIND1 "r"
#define CMD_LIST0   "list"
#define CMD_LIST1   "l"
#define CMD_DELETE0 "delete"
#define CMD_DELETE1 "d"

// Serialization i/o formats.
#define DATA_FORMAT_IN  "\"%63[^\"]\" %ld\n"
#define DATA_FORMAT_OUT "\"%s\" %ld\n"

typedef struct reminder {
    char desc[64]; // Description of the reminder.
    time_t rtime;  // Date when the reminder will run.
} reminder;

int main(int argc, char *argv[])
{
    // Display any reminders if its date has been met and return.
    if (argc == 1)
    {
        FILE* mimi_data = fopen(DATA_FILEPATH, "r");
        if (!mimi_data) { printf("error: couldn't read file %s\n", DATA_FILEPATH); return 1; }

        time_t ctime = time(NULL);

        // Print reminders.
        char line_buf[128];
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            time_t rtime;
            if (sscanf(line_buf, DATA_FORMAT_IN, desc, &rtime) == 2)
                if (ctime > rtime)
                    printf("reminder    %s\n", desc);
        }

        fclose(mimi_data);
        return 0;
    }

    // Print usage.
    if (!strcmp(argv[1], CMD_HELP0) ||
        !strcmp(argv[1], CMD_HELP1) ||
        !strcmp(argv[1], CMD_HELP2) ||
        !strcmp(argv[1], CMD_HELP3))
    {
        const char *usage = 
            "Usage: mimi [option] COMMAND\n"
            "A terminal reminder. Run without arguments to print due reminders.\n"
            "Options:\n"
            "  -h, --help    Print this message.\n"
            "Commands:\n"
            "  r, remind     Create a reminder.\n"
            "                Usage: mimi remind DESC TIME\n"
            "                DESC  Description of the reminder.\n"
            "                TIME  Time in seconds until the reminder goes off.\n"
            "  d, delete     Delete a reminder.\n"
            "                Usage: mimi delete INDEX\n"
            "                INDEX  Index of the reminder to delete.\n"
            "  l, list       List all reminders.\n"
            "  h, help       Print this message.\n";
        printf("%s", usage);
    }
    // Appends a reminder object to data.
    else if (!strcmp(argv[1], CMD_REMIND0) || !strcmp(argv[1], CMD_REMIND1))
    {
        if (argc != 4) { printf("error: invalid argument count\n"); return 1; }

        // Get current data in seconds and then add specified seconds from argument.
        char *end;
        long in_rtime = strtol(argv[3], &end, 10);
        if (*end != '\0')
        {
            printf("error: command %s argument is not a valid number\n", CMD_REMIND0);
            return 1;
        }
        time_t ctime = time(NULL);
        time_t reminder_time = ctime + in_rtime;

        reminder rmn;

        if (strlen(argv[2]) >= sizeof(rmn.desc))
        {
            printf("error: description too big\n");
            return 1;
        }

        // Add data to reminder structure.
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);
        rmn.rtime = reminder_time;

        // Check if DATA_DIRPATH exists, if not create it.
        if (mkdir(DATA_DIRPATH, 0755) == -1)
            if (errno != EEXIST)
            {
                printf("error: failure when creating directory %s", DATA_DIRPATH);
                return 1;
            }

        FILE* mimi_data = fopen(DATA_FILEPATH, "a");
        if (!mimi_data) { printf("error: couldn't append to file %s\n", DATA_FILEPATH); return 1; }

        // Write description and reminder date to DATA_FILEPATH.
        fprintf(mimi_data, DATA_FORMAT_OUT, rmn.desc, rmn.rtime);
        fclose(mimi_data);
    }
    // Delete reminders.
    else if (!strcmp(argv[1], CMD_DELETE0) || !strcmp(argv[1], CMD_DELETE1))
    {
        if (argc != 3) { printf("error: invalid argument count\n"); return 1; }

        // Check if reminder index is a valid number.
        char *end;
        long argidx = strtol(argv[2], &end, 10);
        if (*end != '\0')
        {
            printf("error: command %s argument is not a valid number\n", CMD_DELETE0);
            return 1;
        }

        FILE *mimi_data0 = fopen(DATA_FILEPATH, "r");
        if (!mimi_data0) { printf("error: couldn't read file %s\n", DATA_FILEPATH); return 1; }

        FILE *mimi_temp_data0 = fopen(DATA_TEMP_FILEPATH, "w");
        if (!mimi_temp_data0) { printf("error: couldn't open file %s\n", DATA_TEMP_FILEPATH); return 1; }

        reminder rmn;

        long idx = 0;

        // Write everything except the reminder to delete into DATA_TEMP_FILEPATH.
        char line_buf0[128];
        while (fgets(line_buf0, sizeof(line_buf0), mimi_data0))
        {
            if (argidx != idx &&
                sscanf(line_buf0, DATA_FORMAT_IN, rmn.desc, &rmn.rtime) == 2)
                fprintf(mimi_temp_data0, DATA_FORMAT_OUT, rmn.desc, rmn.rtime);
            idx++;
        }

        fclose(mimi_data0);
        fclose(mimi_temp_data0);

        FILE *mimi_data1 = fopen(DATA_FILEPATH, "w");
        if (!mimi_data1) { printf("error: couldn't open file %s\n", DATA_FILEPATH); return 1; }

        FILE *mimi_temp_data1 = fopen(DATA_TEMP_FILEPATH, "r");
        if (!mimi_temp_data1) { printf("error: couldn't read file %s\n", DATA_TEMP_FILEPATH); return 1; }

        // Write DATA_TEMP_FILEPATH contents to DATA_FILEPATH.
        char line_buf1[128];
        while (fgets(line_buf1, sizeof(line_buf1), mimi_temp_data1))
            fprintf(mimi_data1, "%s", line_buf1);

        fclose(mimi_data1);
        fclose(mimi_temp_data1);
    }
    // Verbosely list reminders.
    else if (!strcmp(argv[1], CMD_LIST0) || !strcmp(argv[1], CMD_LIST1))
    {
        if (argc != 2) { printf("error: invalid argument count\n"); return 1; }

        FILE* mimi_data = fopen(DATA_FILEPATH, "r");
        if (!mimi_data) { printf("error: couldn't read file %s\n", DATA_FILEPATH); return 1; }

        size_t idx = 0;
        char line_buf[128];
        
        // Print reminders.
        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            time_t rtime;
            if (sscanf(line_buf, DATA_FORMAT_IN, desc, &rtime) == 2)
            {
                char buf[64];
                struct tm *tm_info = localtime(&rtime);
                strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
                printf("%zu    %s    %s\n", idx++, buf, desc);
            }
        }

        fclose(mimi_data);
    }
    else
    {
        printf("error: unknown command\n");
        return 1;
    }

    return 0;
}
