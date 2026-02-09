const char* REMINDER_FORMAT_IN  = "{\"index\":\"%ld\",\"description\":\"%[^\"]\"}";
const char* REMINDER_FORMAT_OUT0 = "{\"index\":\"%ld\",\"description\":\"%s\"}";
const char* REMINDER_FORMAT_OUT1 = "{\"index\":\"%ld\",\"description\":\"%s\"},";

typedef struct reminder
{
    size_t index;
    char description[32];
} reminder;

int init_json()
{
    FILE* json = fopen("mimi.json", "a");
    if (json == NULL) return 1;

    fseek(json, 0, SEEK_END);
    long json_size = ftell(json);
    rewind(json);

    if (json_size == 0)
    {
        char init_buf[] = "[]\n";
        fwrite(init_buf, 1, sizeof(init_buf), json);
        fclose(json);
    }

    return 0;
}
