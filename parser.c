#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror(argv[1]);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof line, in)) {
        char vars[52];
        int var_count = 0;
        char *name = strtok(line, " \t\r\n");
        if (!name) 
            continue;

        char *colon = strtok(NULL, " \t\r\n");
        if (!colon || strcmp(colon, ":") != 0) {
            fprintf(stderr, "%s is not valid\n", name);
            continue;
        }

        int depth = 0;
        bool failed = false;    
        char outname[256];
        snprintf(outname, sizeof outname, "%s.s", name);
        FILE *out = fopen(outname, "w");
        if (!out) { perror(outname); continue; }

        //prologue
        fprintf(out, ".text\n");
        fprintf(out, ".global %s\n", name);
        fprintf(out, "%s:\n", name);
        fprintf(out, "pushl %%ebp\n");
        fprintf(out, "movl %%esp, %%ebp\n");

        char *tok;
        while ((tok = strtok(NULL, " \t\r\n")) != NULL) {
            if (isdigit((uint8_t)tok[0]) || 
                        (tok[0] == '-' && isdigit((uint8_t)tok[1]))) {
                
                //makes sure it's a valid number
                int counter = 1;
                while (tok[counter] != '\0') {
                    if (!isdigit((uint8_t)tok[counter])) {
                        fprintf(stderr, "%s is not valid\n", name);
                        failed = true;
                        break;
                    }
                    counter++;
                }

                if (failed)
                    break;

                fprintf(out, "pushl $%s\n", tok);
                depth++;
            } else if (tok[1] == '\0' && strchr("+-*/", tok[0])) {
                if (depth < 2) {
                    fprintf(stderr, "%s is not valid\n", name);
                    failed = true;
                    break;
                }
                depth--;
                
                fprintf(out, "popl %%ecx\n");
                fprintf(out, "popl %%eax\n");

                switch(tok[0]) {
                    case '+': {
                        fprintf(out, "addl %%ecx, %%eax\n");
                        break;
                    }
                    case '-': {
                        fprintf(out, "subl %%ecx, %%eax\n");
                        break;
                    }
                    case '*': {
                        fprintf(out, "imull %%ecx, %%eax\n");
                        break;
                    }
                    case '/':
                        fprintf(out, "movl %%eax, %%edx\n");
                        fprintf(out, "sarl $31, %%edx\n");
                        fprintf(out, "idivl %%ecx\n");
                        break;
                }

                fprintf(out, "pushl %%eax\n");
            } else if (isalpha((uint8_t)tok[0]) && tok[1] == '\0') {
                int idx = -1;
                for (int i = 0; i < var_count; i++) {
                    if (vars[i] == tok[0]) { 
                        idx = i;
                        break; 
                    }
                }
                if (idx < 0) {
                    idx = var_count;
                    vars[var_count++] = tok[0];
                }
                fprintf(out, "pushl %d(%%ebp)\n", 8 + 4 * idx);
                depth++;
            } else {
                fprintf(stderr, "%s is not valid\n", name);
                failed = true;
                break;
            }
        }

        if (!failed && depth != 1) {
            fprintf(stderr, "%s is not valid\n", name);
            failed = true;
        }

        //epilogue
        if (!failed) {
            fprintf(out, "popl %%eax\n");
            fprintf(out, "popl %%ebp\n");
            fprintf(out, "ret\n");
            fclose(out);
        } else {
            fclose(out);
            remove(outname);
        }
    }

    fclose(in); 
    return 0;
}
