#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define OS "win"
#else
#define OS "linux"
#endif

typedef struct {
    char* key;
    char* value;
} Variable;

Variable vars[100];
int var_count = 0;

void compile_script(FILE* input, FILE* output) {
    char line[256];
    int in_win_block = 0;
    int in_linux_block = 0;
    
    fprintf(output, "#include <stdlib.h>\n");
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "int main() {\n");
    
    while(fgets(line, sizeof(line), input)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines and comments
        if(strlen(line) == 0 || line[0] == '#') continue;
        
        // Handle OS blocks
        if(strcmp(line, "[win]") == 0) {
            in_win_block = 1;
            in_linux_block = 0;
            continue;
        }
        if(strcmp(line, "[linux]") == 0) {
            in_linux_block = 1;
            in_win_block = 0;
            continue;
        }
        
        // Skip lines not for current OS
        if((in_win_block && strcmp(OS, "win") != 0) ||
           (in_linux_block && strcmp(OS, "linux") != 0)) continue;
        
        // Variable assignment
        if(line[0] == '@') {
            char* eq = strchr(line, '=');
            if(eq) {
                *eq = 0;
                char* var_name = line+1;
                char* var_value = eq+1;
                
                vars[var_count].key = strdup(var_name);
                vars[var_count].value = strdup(var_value);
                var_count++;
                
                fprintf(output, "    setenv(\"%s\", \"%s\", 1);\n", var_name, var_value);
            }
            continue;
        }
        
        // Command execution
        if(line[0] == '!') {
            char cmd[256];
            strcpy(cmd, line+1);
            
            // Variable substitution
            for(int i = 0; i < var_count; i++) {
                char pattern[50];
                sprintf(pattern, "{%s}", vars[i].key);
                char* found = strstr(cmd, pattern);
                if(found) {
                    memcpy(found, vars[i].value, strlen(vars[i].value));
                    memset(found + strlen(vars[i].value), ' ', strlen(pattern) - strlen(vars[i].value));
                }
            }
            
            fprintf(output, "    system(\"%s\");\n", cmd);
        }
        
        // Conditional statements
        if(strncmp(line, "if ", 3) == 0) {
            char condition[256];
            strcpy(condition, line + 3);
            fprintf(output, "    if (%s) {\n", condition);
            continue;
        }
        if(strcmp(line, "else") == 0) {
            fprintf(output, "    } else {\n");
            continue;
        }
        if(strcmp(line, "endif") == 0) {
            fprintf(output, "    }\n");
            continue;
        }
        
        // Loops
        if(strncmp(line, "for ", 4) == 0) {
            char loop[256];
            strcpy(loop, line + 4);
            fprintf(output, "    for (%s) {\n", loop);
            continue;
        }
        if(strncmp(line, "while ", 6) == 0) {
            char condition[256];
            strcpy(condition, line + 6);
            fprintf(output, "    while (%s) {\n", condition);
            continue;
        }
        if(strcmp(line, "endloop") == 0) {
            fprintf(output, "    }\n");
            continue;
        }
        
        // Functions
        if(strncmp(line, "function ", 9) == 0) {
            char func_name[256];
            strcpy(func_name, line + 9);
            fprintf(output, "void %s() {\n", func_name);
            continue;
        }
        if(strcmp(line, "endfunction") == 0) {
            fprintf(output, "}\n");
            continue;
        }
        if(line[0] == '&') {
            char func_call[256];
            strcpy(func_call, line + 1);
            fprintf(output, "    %s();\n", func_call);
            continue;
        }
        
        // Error handling
        if(strncmp(line, "error ", 6) == 0) {
            char error_msg[256];
            strcpy(error_msg, line + 6);
            fprintf(output, "    fprintf(stderr, \"Error: %s\\n\");\n", error_msg);
            fprintf(output, "    exit(1);\n");
            continue;
        }
    }
    
    fprintf(output, "    return 0;\n}");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: asc <script.as>\n");
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }

    FILE* output = fopen("generated.c", "w");
    if (!output) {
        printf("Error: Could not create output file\n");
        fclose(input);
        return 1;
    }

    compile_script(input, output);

    fclose(input);
    fclose(output);

    printf("Script compiled to 'generated.c'\n");
    return 0;
}


