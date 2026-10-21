#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

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
        fprintf(out, ".text\n");
        fprintf(out, ".global %s\n", name);
        fprintf(out, "%s:\n", name);
        fprintf(out, "pushl %%ebp\n");
        fprintf(out, "movl %%esp, %%ebp\n");
        fprintf(out, "pushl %%ecx\n");

        char *tok;
        while ((tok = strtok(NULL, " \t\n")) != NULL) {
            if (isdigit((uint8_t)tok[0]) || 
                        (tok[0] == '-' && isdigit((uint8_t)tok[1]))) {
                fprintf(out, "pushl $%s\n", tok);
            } else if (tok[1] == '\0' && strchr("+-*/", tok[0])) {
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
                    case '/' : {
                        fprintf(out, "idivl %%ecx, %%eax\n");
                    }
                }

                fprintf(out, "pushl %%eax\n");
            } else if (isalpha((uint8_t)tok[0]) && tok[1] == '\0'){
                // 0 is old ebp, 4 is return address, 8 starts variables
                fprintf(out, "pushl %d(%%ebp)\n", 8 + 4 * (tok[0] - 'a'));
            }
    
        
        }
    //epilogue
        fprintf(out, "popl %%eax\n");
        fprintf(out, "popl %%ecx\n");
        fprintf(out, "popl %%ebp\n");
        fprintf(out, "ret\n");
        fclose(out);
    }

    fclose(in);
    return 0;
}
