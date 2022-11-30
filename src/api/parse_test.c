#include "parse.h"
#include "fetch.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define FILE_NAME "idonotexist.txt"
#define FILE_CONTENT "dsfnksdfdldsdklfnsdfdsfs\ndgiohngdfoidf\ndsfdsdfsdf\ndsdfsfs{sdfds\ndrgfdgdg}dsfsf\nfdgfdd"

int main()
{
    FILE *fp = fopen(FILE_NAME, "w");
    fputs(FILE_CONTENT, fp);
    fclose(fp);

    char *content = NULL;
    int p = parse_read_file_to_end(FILE_NAME, &content);
    assert(p == 0);
    assert(strcmp(FILE_CONTENT, content) == 0);
    return 0;
}