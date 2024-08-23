#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int operate(FILE *f);

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

	int min_g = operate(f);
	fprintf(stdout, "%d", min_g);

    fclose(f);  // Close the file when done
	return 0;
}


int operate(FILE *f) {
    int retval;
    int line_n;
    int grade;
    int min_grade;

    min_grade=101;
    line_n = 0;

    while(1) {
        retval = fscanf(f, "%d", &grade);
        if (retval == EOF){
            break; /* Finished */
        } else if (retval != 1 ) {
            /* Error */
            fprintf(stderr, "Error: not a number\n");
            exit(1);
        } else {
        	if ((grade < 0)||(grade > 100)){
        		fprintf(stderr,"“Error at line %d: grade %d invalid", line_n+1, grade);
        		exit(1);
        	}
        	if (grade<min_grade){
        		min_grade=grade;
        	}
        }
        line_n++;
    }

    return min_grade;
}
