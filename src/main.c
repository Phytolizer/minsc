#include <linenoise.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    linenoiseHistoryLoad("minsc.history");

    while (true) {
        char* line = linenoise(">> ");
        if (line == NULL) {
            break;
        }

        linenoiseHistoryAdd(line);

        if (strcmp(line, "1 + 2 * 3") == 0) {
            printfln("7");
        } else {
            printfln("ERROR: Invalid expression!");
        }

        free(line);
    }

    linenoiseHistorySave("minsc.history");
}
