#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror(argv[1]);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof line, in)) {
        char *name = strtok(line, " \t\n");
        if (!name) continue;

        char *colon = strtok(NULL, " \t\n");
        if (!colon || strcmp(colon, ":") != 0)
            continue;

        char outname[256];
        snprintf(outname, sizeof outname, "%s.s", name);
        FILE *out = fopen(outname, "w");
        if (!out) { perror(outname); continue; }

        //prologue

        char *tok;
        while ((tok = strtok(NULL, " \t\n")) != NULL) {
            
        }

        //epilogue

        fclose(out);
    }

    fclose(in);
    return 0;
}
