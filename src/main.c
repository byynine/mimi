#include "inc/minc.c"

// TODO: get rid of magic numbers; e.g buffer sizes

// TODO: function for extracting contents of mimi.json
int read_json(long* index_out, char* desc_out)
{
    FILE* json = fopen("mimi.json", "r");
    if (json == NULL) return 1;

    long index;
    char description[32];

    char line_buf[64];

    // TODO: FIX not adding actual indices and descriptions
    // read mimi.json and input values into index and description
    while (fgets(line_buf, sizeof(line_buf), json))
    {
        if (sscanf(line_buf, "        \"index\": \"%ld\"", &index) == 1)
        {
            fgets(line_buf, sizeof(line_buf), json);
            sscanf(line_buf, "        \"description\": \"%[^\"]\"", description);
            // printf("read index=%ld, description=%s\n", index, description);
        }
    }

    *index_out = index;
    if (snprintf(desc_out, sizeof(description), "%s", description) < 0)
    {
        printf("error while reading json with snprintf");
        return 1;
    }

    fclose(json);
    return 0;
}

// TODO: too much redundancy with file open/close, file size reading,
//       reading file contents and overall code.
int main(int argc, char* argv[])
{
    if (argc != 2) return 1;

    init_json();

    if (strequal(argv[1], "list")) // TODO: isolate this into a function
    {
        long index;
        char description[32];

        int r = read_json(&index, description);

        printf("r: %d\n", r);
        printf("index: %ld\ndescription: %s\n", index, description);

        // FILE* file = fopen("mimi.json", "r");
        // if (file == NULL) return 1;

        // char line[64];
        // long index;
        // char description[32];

        // while (fgets(line, sizeof(line), file)) {
        //     if (sscanf(line, "        \"index\": \"%ld\"", &index) == 1) {
        //         fgets(line, sizeof(line), file);
        //         sscanf(line, "        \"description\": \"%[^\"]\"", description);
        //         printf("read index=%ld, description=%s\n", index, description);
        //     }
        // }

        // fclose(file);
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

        fwrite(buffer + 1, 1, filesize - 1, file);
        fclose(file);
    }

    return 0;
}
