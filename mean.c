#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void operate(FILE *f);

int main (int argc, char *argv[]) {
    FILE *f;

    if (argc == 1 || !strcmp("-", argv[1]) ) {
        f = stdin;
    } else {
        f = fopen(argv[1], "r");
    }
    if (!f) {
        fprintf(stderr, "File not found: \"%s\"\n", argv[1]);
        return 1;
    }

    operate(f);
}


void operate(FILE *f) {
    int grade;
    int retval;
    double avg;
    int line_n;

    avg = 0;
    line_n = 0;

    while(1) {
        retval = fscanf(f, "%d", &grade);
        if (retval == EOF){
            break; /* Finished */
        } else if (retval != 1 ) {
            /* Error */
            fprintf(stderr, "Error: not a number\n");
            exit(1);
        }
        if ((grade < 0)||(grade > 100)){
			fprintf(stderr,"“Error at line %d: grade %d invalid", line_n+1, grade);
			exit(1);
        }
        avg += grade;
        line_n++;
    }
    printf("%.2lf\n", avg / line_n);

}
