#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s <spec-file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror(argv[1]);
        return 1;
    }

    const char *slash = strrchr(argv[1], '/');
    const char *base  = slash ? slash + 1 : argv[1];
    char basename[256];
    strncpy(basename, base, sizeof(basename) - 1);
    basename[sizeof(basename) - 1] = 0;
    char *dot = strrchr(basename, '.');
    if (dot)
        *dot = 0;

    char hpath[300], cpath[300];
    snprintf(hpath, sizeof(hpath), "%s.h", basename);
    snprintf(cpath, sizeof(cpath), "%s.c", basename);

    FILE *hptr = fopen(hpath, "w");
    FILE *cptr = fopen(cpath, "w");
    if (!hptr || !cptr) {
        perror("fopen output");
        return 1;
    }

    fprintf(hptr, "#ifndef %s_H\n#define %s_H\n\n", basename, basename);
    fprintf(cptr, "#include <stdio.h>\n#include \"%s.h\"\n\n", basename);

    char curr;
    while (fscanf(in, "%c", &curr) == 1) {

        if (curr == '\n')          /* blank line: skip it, keep parsing */
            continue;
        if (curr == '#') {         /* comment line: discard to end of line */
            while (fscanf(in, "%c", &curr) == 1 && curr != '\n')
                ;
            continue;
        }

        char keyword[40];
        char symbol[8];
        keyword[0] = curr;
        int position = 1;

        /* read the identifier up to the first blank */
        while (fscanf(in, "%c", &curr) == 1) {
            if (curr == ' ' || curr == '\t')
                break;
            keyword[position] = curr;
            position++;
        }
        keyword[position] = 0;

        fprintf(hptr, "int %s(int a,int b);\n", keyword);

        while (fscanf(in, "%c", &curr) == 1 && (curr == ' ' || curr == '\t'))
            ;

        symbol[0] = curr;
        position = 1;

        while (fscanf(in, "%c", &curr) == 1) {
            if (curr == '\n' || curr == ' ' || curr == '\t')
                break;
            symbol[position] = curr;
            position++;
        }
        symbol[position] = 0;

        fprintf(cptr,
                "int %s(int a,int b) { printf(\"%s(%%d,%%d)\\n\", a, b); return a %s b; }\n",
                keyword, keyword, symbol);
    }

    fprintf(hptr, "\n#endif\n");

    fclose(in);
    fclose(hptr);
    fclose(cptr);
    return 0;
}
