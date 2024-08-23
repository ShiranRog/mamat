#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int *operate(FILE *f, int nbins);
// start operating on file
void organizing_in_histogram_bins(int nbins, int *array, int grade);
// place in histogram bins


int main(int argc, char *argv[]) {
    int nbins = 10;  // Initialize with the default that was given
    int i;
    int file_name_index=-1;
    int len=0;

    // Iterate through all stdin arguments
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-nbins") == 0)
        {
        	i++;
        	if (i <= argc){
            	nbins = atoi(argv[i]);
        	}
        }
        else
        {
        	len = strlen(argv[i]);
        	if (len > 4)
        	{
        		if (strcmp(argv[i] + len - 4, ".txt") == 0)
        		{
        		file_name_index = i;
        		}
        	}
        }
    }

    FILE *f;

    if (file_name_index == -1 || !strcmp("-", argv[1]) )
    {
        f = stdin;
    }
    else
    {
        f = fopen(argv[file_name_index], "r");
    }
    if (!f) {
        fprintf(stderr, "File not found: \"%s\"\n", argv[1]);
        return 1;
    }

	int *hist = operate(f, nbins);
    fprintf(stdout, "Histogram values:\n");
    int space_between_bins = 100/nbins;
    int bin_low = 0;
    int bin_high = space_between_bins-1;
    for (int i = 0; i < nbins; i++) {
        fprintf(stdout, "Bin %d-%d \t%d\n", bin_low,bin_high , hist[i]);
        bin_low+=space_between_bins;
        bin_high+=space_between_bins;
    }
    free(hist);

    fclose(f);  // Close the file when done
	return 0;
}


int *operate(FILE *f, int nbins) {
    int retval;
    int line_n;
    int grade;

    int *array_hist = (int *)malloc(nbins * sizeof(int));
    if (array_hist == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < nbins; i++) {
    	array_hist[i] = 0;
    }

    line_n = 0;

    while(1) {
        retval = fscanf(f, "%d", &grade);
        if (retval == EOF){
            break; /* Finished */
        } else if (retval != 1 ) {
            /* Error */
            fprintf(stderr, "Error: not a number\n");
            free(array_hist);
            exit(1);
        } else {
        	if ((grade < 0)||(grade > 100)){
        		free(array_hist);
        		fprintf(stderr,"“Error at line %d: grade %d invalid", line_n+1, grade);
        		exit(1);
        	}
            organizing_in_histogram_bins(nbins, array_hist, grade);
        }
        line_n++;
    }

    return array_hist;
}


/* finding the histogram */
void organizing_in_histogram_bins(int nbins, int *array, int grade) {

    int binindex;
    binindex= (grade * nbins)/100;

	array[binindex] ++;

}
