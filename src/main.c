#include "inc/minc.c"

int main(int argc, char* argv[])
{
    if (argc != 2) return 1;

    init_json();

    if (strequal(argv[1], "list")) // TODO: isolate this into a function
    {
        FILE* file = fopen("mimi.json", "r");
        if (file == NULL) return 1;

        char line[64];
        long index;
        char description[32];

        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "        \"index\": \"%ld\"", &index) == 1) {
                fgets(line, sizeof(line), file); // read next line
                sscanf(line, "        \"description\": \"%[^\"]\"", description);
                printf("Parsed index=%ld, description=%s\n", index, description);
            }
        }
    }
    else if (strequal(argv[1], KW_REMIND)) // TODO: isolate this into a function
    {
        reminder rmn = {
            .index = 0,
            .description = "null",
        };

        FILE* file = fopen("mimi.json", "r+");
        if (!file) return 1;

        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        rewind(file);

        char buffer[filesize + 1];
        fread(buffer, 1, filesize, file);
        buffer[filesize] = '\0';

        fseek(file, 2, SEEK_SET);

        if (filesize < 6)
            fprintf(file, REMINDER_FORMAT_OUT0, rmn.index, rmn.description);
        else
            fprintf(file, REMINDER_FORMAT_OUT1, rmn.index, rmn.description);

        // Write the rest of original content
        fwrite(buffer + 1, 1, filesize - 1, file);

        fclose(file);
    }

    return 0;
}
