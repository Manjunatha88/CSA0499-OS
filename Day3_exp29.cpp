#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char line[1024];
    char *search_term = argv[1];

    while (fgets(line, sizeof(line), stdin)) {
        if (strstr(line, search_term)) {
            printf("%s", line);
        }
    }

}
