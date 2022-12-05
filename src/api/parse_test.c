#include "parse.h"
#include "fetch.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define FILE_NAME "idonotexist.txt"
#define FILE_CONTENT "dsfnksdfdldsdklfnsdfdsfs\ndgiohngdfoidf\ndsfdsdfsdf\ndsdfsfs{sdfds\ndrgfdgdg}dsfsf\nfdgfdd"

int main()
{
    /******************** Read entire file ********************/
    FILE *fp = fopen(FILE_NAME, "w");
    fputs(FILE_CONTENT, fp);
    fclose(fp);

    char *content = NULL;
    int p = parse_read_file_to_end(FILE_NAME, &content);
    assert(p == 0);
    assert(strcmp(FILE_CONTENT, content) == 0);

    
    
    /******************** Regex units ********************/
    store_item_s regex_item = {
        .name = "some text here and 10 STK with more",
        .price = 40,
        .unit = UNKNOWN,
        .size = 1,
        .price_per_unit = regex_item.price
    };

    parse_populate_item_unit(&regex_item);

    // char* c_size = parse_try_regex_group("some text here and 10 STK with more", "/([0-9]+)[ ]?STK/i");
    char* c_size = parse_try_regex_group("some text here and 10 STK with more", "([0-9]+)[ ]?STK");
    assert(strcmp(c_size, "10") == 0);
    double rexeg_size = parse_try_extract_size("some text here and 10 stk with more", "STK");
    assert(rexeg_size == 10);

    return 0;
}