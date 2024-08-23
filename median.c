#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* I am using a binary tree to save the grades in order */
struct tnode { /* the tree node: */
    int grade; /* points to the grade */
    int count; /* number of occurrences */
    struct tnode *left; /* left child */
    struct tnode *right; /* right child */
 };


int operate(FILE *f);
struct tnode *addtree(struct tnode *p, int g); // functino to add new tree nodes
int find_n_smallest_grade(struct tnode *p, int n, int *counter); // locating specific grade from tree
void freetree(struct tnode *p);  // Function to free the tree


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

	int Median = operate(f);
	fprintf(stdout, "%d", Median);

    fclose(f);  // Close the file when done
	return 0;
}


int operate(FILE *f) {
    int retval;
    int line_n;
    int grade;
    struct tnode *root;
    root = NULL;

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
        	root = addtree(root, grade);
        }
        line_n++;
    }


    int counter=0;
    int n=((line_n/2)+(line_n%2));
    int median_grade=find_n_smallest_grade(root, n, &counter);

    freetree(root);  // Free the tree after I'm done with it
    return median_grade;
}

/* addtree: add a node with g, at or below p */
struct tnode *addtree(struct tnode *p, int g) {
   int cond;

   if (p == NULL) { /* a new word has arrived */
       p = (struct tnode *) malloc(sizeof(struct tnode)); /* make a new node */
       if (p == NULL) {
           fprintf(stderr, "Memory allocation failed\n");
           exit(1);
       }
       p->grade = g;
       p->count = 1;
       p->left = p->right = NULL;
   }
   else if ((cond =  g - p->grade ) == 0) {
       p->count++; /* repeated grade */
   }
   else if (cond < 0) {/* less than into left subtree */
       p->left = addtree(p->left, g);
   }
   else { /* greater than into right subtree */
       p->right = addtree(p->right, g);
   }

   return p;
}



/* finding the median */
int find_n_smallest_grade(struct tnode *p, int n, int *counter) {
	/* in this, we are going to the smallest and counting up */
	int new_grade=-1;
	if (p == NULL) { /* we're at the end of the tree */
	   return -1;
	}

	/*smaller subtree*/
	new_grade = find_n_smallest_grade(p->left, n, counter);

	if (new_grade>=0){
	   return new_grade;
	}

	/* current node */
	*counter=*counter+(p->count);
	if(*counter>=n) {
	   return p->grade;
	}

	/*larger subtree*/
	new_grade = find_n_smallest_grade(p->right, n, counter);
	return new_grade;

}


void freetree(struct tnode *p) {
    if (p != NULL) {
        freetree(p->left);   // Free the left subtree
        freetree(p->right);  // Free the right subtree
        free(p);  // Free the current node
    }
}
