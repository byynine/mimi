#include "inc/minc.c"

const char* REMINDER_FORMAT_IN  = "{\n    \"index\": \"%d\",\n    \"description\": \"%[^\"]\"\n}";
const char* REMINDER_FORMAT_OUT = "{\n    \"index\": \"%d\",\n    \"description\": \"%s\"\n}";

typedef struct reminder
{
    size_t index;
    char description[32];
    // int cdate[5]; // int cdate[] = { 23, 21, 7, 2, 2026 };
} reminder;

int main(int argc, char* argv[])
{
    if (argc != 2) return 1;

    FILE* mimi = fopen("mimi.json", "a");
    if (mimi == NULL) return 1;

    fseek(mimi, 0, SEEK_END);
    long filesize = ftell(mimi);
    rewind(mimi);
    
    char buf[] = "{\n}\n";
    fwrite(buf, 1, sizeof(buf), mimi);

    fclose(mimi);

    size_t remind_index = 0;

    if (strequal(argv[1], "list"))
    {
        // TODO: list of reminders

        FILE* file = fopen("mimi.json", "r");
        if (file == NULL) return 1;

        reminder rmn;

        fscanf(file, REMINDER_FORMAT_IN, &rmn.index, rmn.description);

        printf("index: %d\ndescription: %s\n", rmn.index, rmn.description);
        fclose(file);
    }
    else if (strequal(argv[1], KW_REMIND))
    {
        // data structure on disk
        // OVERALL DATA
        // REMINDERS
        // 
        // remind_n N | remind_n 12
        // remind "foo bar" date whatever

        reminder rmn = {
            .index = remind_index,
            .description = "null",
        };

        FILE* file = fopen("mimi.json", "r+");
        if (!file) return 1;

        // Move to the end to get file size
        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        rewind(file);  // go back to start

        // Read the entire file
        char buffer[filesize + 1];
        fread(buffer, 1, filesize, file);
        buffer[filesize] = '\0';

        // Move to position 1 (after first character)
        fseek(file, 1, SEEK_SET);

        fprintf(file, REMINDER_FORMAT_OUT, rmn.index, rmn.description);

        // Write the rest of original content
        fwrite(buffer + 1, 1, filesize - 1, file);

        remind_index++;
        fclose(file);
    }

    return 0;
}
