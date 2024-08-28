#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num1> <num2>\n", argv[0]);
        return 1;
    }

    double num1 = atof(argv[1]);
    double num2 = atof(argv[2]);
    printf("Sum: %.2f\n", num1 + num2);
    return 0;
}