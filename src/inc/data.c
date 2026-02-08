const char* REMINDER_FORMAT_IN  = "    {\n        \"index\": \"%ld\",\n        \"description\": \"%[^\"]\"\n    }";
const char* REMINDER_FORMAT_OUT0 = "    {\n        \"index\": \"%ld\",\n        \"description\": \"%s\"\n    }";
const char* REMINDER_FORMAT_OUT1 = "    {\n        \"index\": \"%ld\",\n        \"description\": \"%s\"\n    },";

typedef struct reminder
{
    size_t index;
    char description[32];
} reminder;

int init_json()
{
    FILE* json = fopen("mimi.json", "w");
    if (json == NULL) return 1;

    fseek(json, 0, SEEK_END);
    long json_size = ftell(json);
    rewind(json);

    if (json_size == 0)
    {
        char init_buf[] = "[\n]\n";
        fwrite(init_buf, 1, sizeof(init_buf), json);
        fclose(json);
    }

    return 0;
}
