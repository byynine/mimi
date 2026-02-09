#include <stdio.h>
#include <string.h>

// TODO: CHANGE all constants into macros

// TODO: (in further versions) change to /home/user/.local/share/mimi/mimi.json
const char *mimi_data_filepath = "tmp/data";

// command keywords for when calling from shell
const char *kw_remind = "remind"; // create a reminder
const char *kw_list = "list";     // print list of reminders

// serialization i/o formats
const char *reminder_format_in = "\"%[^\"]\"";
const char *reminder_format_out = "\"%s\"\n";
// const char *reminder_format_out_1 = "{\"index\":\"%ld\",\"description\":\"%s\"},";

// [
// {desc:"foo"},{desc:"awdwa"},{desc:"awae"},
// {desc:"bar"},
// {desc:"baz"},
// {desc:"something"}
// ]

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
        char line_buf[64];

        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, reminder_format_in, &desc) == 1) printf("%s\n", desc);
        }

        return 0;
    }

    if (argc > 3) return 2;

    // this appends a reminder object to data
    if (!strcmp(argv[1], kw_remind))
    {
        if (argc != 3) return 3;
        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        FILE* mimi_data = fopen(mimi_data_filepath, "a");
        if (!mimi_data) return 4;

        fprintf(mimi_data, reminder_format_out, rmn.desc);
    }
    // this prints the reminders in data
    else if (!strcmp(argv[1], kw_list))
    {
        FILE* mimi_data = fopen(mimi_data_filepath, "r");
        if (!mimi_data) return 5;

        reminder rmn;
        snprintf(rmn.desc, sizeof(rmn.desc), "%s", argv[2]);

        char line_buf[64];

        while (fgets(line_buf, sizeof(line_buf), mimi_data))
        {
            char desc[64];
            if (sscanf(line_buf, reminder_format_in, &desc) == 1) printf("%s\n", desc);
        }
    }

    return 0;
}
