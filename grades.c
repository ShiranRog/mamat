#include "grades.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linked-list.h"


// Static Functions:

/* What we consider an element */
static struct success_data {
    char *course;
    int grade;
};

////////////////////////////* functions on a single element level*/////////////////////////////

/* needed for grade list creation function*/
static int success_clone_elements(struct success_data *success_pointer, void **output) {
    /* grade_pointer points to grade element, output points to pointer of the new grade element */
    struct success_data *new_data = malloc(sizeof(struct success_data));
    if (new_data==NULL) return -1; //FAILED TO ALLOCATE
    new_data->course = strdup(success_pointer->course);
    if (new_data->course == NULL) {
        // FAILED TO ALLOCATE
        free(new_data);
        return -1;
    }
    new_data->grade = success_pointer->grade;
    *output = new_data;
    return 0;
}

/* needed for grade list creation function*/
static void success_destroy_elements(struct success_data *success_pointer){
    /* grade_pointer points to grade element*/
    if (success_pointer==NULL) return 0;
    if ((success_pointer->course)!=NULL) {
        // we need to free the string itself that was generated using strdup
        free(success_pointer->course); 
        }
    // now we need to free the int and the pointer to the string:
    free(success_pointer);
}

///////////////////* functions on a linked list of elements (success_data structs) *////////////////

/* cloning linked list for grades of single student */
static int grades_list_clone(struct list *old_grades_list, void **output) {
    struct list *new_grade_list = list_init(success_clone_elements, success_destroy_elements);
    if (new_grade_list==NULL) return -1; //FAILED TO INITIATE LIST
    struct iterator *old_list_iterator = list_begin(old_grades_list);
    int checkup_on_push_front = 0; //Needed to verify success
    struct success_data *specific_element = list_get(old_list_iterator); // initiating elements for copying
    // Iterating over every element in old list:
    while(old_list_iterator != NULL) {
        specific_element = list_get(old_list_iterator); // the elements we would like to copy
        if (specific_element==NULL) {
            list_destroy(new_grade_list);
            return -1;
            }
        checkup_on_push_front = list_push_front(new_grade_list, specific_element);
        if (checkup_on_push_front!=0) {
            list_destroy(new_grade_list);
            return -1;
            }
        old_list_iterator = list_next(old_list_iterator);
    }
    *output = new_grade_list;
    return 0;
}

/*destrying a list of grades, always successful*/
static void *grades_list_destroy(struct list *old_grades_list) {
    if (old_grades_list==NULL) return;
    list_destroy(old_grades_list);
    return;
}

//////////////////* functions on a single student (student struct as defined here)*///////////////

// We have a list of success data elements which is the grades_list. 
// With it we create student information:

static struct student {
    char *student_name;
    int student_ID;
    struct list *grades_list;
};


/* needed for student list creation function */
static int student_clone_elements(struct student *student_pointer, void **output) {
    /* student_pointer points to student element, output points to pointer of the new student element */
    struct student *new_data = malloc(sizeof(struct student));
    if (new_data==NULL) return -1; //FAILED TO ALLOCATE
    new_data->student_ID = student_pointer->student_ID;
    struct list *p = new_data->grades_list; // we need a pointer to a pointer
    int checkup = 0;
    checkup = grades_list_clone(student_pointer->grades_list, &p);
    if (checkup==-1) {
        free(new_data);
        return -1;
        }
    new_data->student_name = strdup(student_pointer->student_name);
    if (new_data->student_name == NULL) {
        // FAILED TO ALLOCATE
        list_destroy(new_data->grades_list);
        free(new_data);
        return -1;
    }
    *output = new_data;
    return 0;
}

/* needed for student list creation function*/
static void student_destroy_elements(struct student *student_pointer) {
    /* grade_pointer points to grade element*/
    if (student_pointer != NULL) {
        if (student_pointer==NULL) return;
        if (student_pointer->student_name!= NULL) free(student_pointer->student_name);
        if (student_pointer->grades_list != NULL) grades_list_destroy(student_pointer->grades_list);
        free(student_pointer);
    }
    return;
}

// End of static functions section

/* Grades is a linked list of students */
struct grades {
    struct list *all_students;
};

/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init() {
    struct grades *g = malloc(sizeof(struct grades));
    if (g==NULL) return NULL; // FAILED TO ALLOCATE
    g->all_students = list_init(student_clone_elements, student_destroy_elements);
    if (g->all_students==NULL) // FAILED TO INITIATE LIST 
    { 
        free(g);
        return NULL;
    }
    return g;
};

/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades){
    if (grades != NULL) {
        if (grades->all_students != NULL) list_destroy(grades->all_students);
        free(grades);
    }
    return;
    };

/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with 
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){
    /* testing grades */
    if (grades==NULL) return -1;
    if (grades->all_students == NULL) return -1;
    /* testing if id already exists*/
    struct iterator *it = list_begin(grades->all_students);
    struct student *specific_element = list_get(it); // initiating elements for searching
    // Iterating over every element in old list:
    while(it != NULL) {
        specific_element = list_get(it); // the elements we would like to search
        if (specific_element==NULL) {
            // error on list
            return -1;
            }
        if (specific_element->student_ID == id) {
            return -1;
            }
        it = list_next(it);
    }
    /* In Order to add element we need to create it first*/
    struct student *new_student = malloc(sizeof(struct student));
    if (new_student==NULL) return -1;
    new_student->student_name = strdup(name);
    if (new_student->student_name==NULL) {
        free(new_student);
        return -1;
    }
    new_student->grades_list = list_init(grades_list_clone, grades_list_destroy);
    if (new_student->grades_list == NULL){
        free(new_student->student_name);
        free(new_student);
        return -1;
    }
    new_student->student_ID = id;
    int flag = 0;
    flag = list_push_back(grades->all_students, new_student);
    if (flag!=0) {
        list_destroy(new_student->grades_list);
        free(new_student->student_name);
        free(new_student);
        return -1;
    }
    return 0;
};

/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade)
    {
    if (grades==NULL) return -1;
    if (grades->all_students == NULL) return -1;
    if ((grade > 100)||(grade < 0)) return -1;
    struct iterator *it = list_begin(grades->all_students);
    struct student *specific_element = list_get(it); // initiating elements for searching
    // Iterating over every element in old list:
    while(it != NULL) {
        specific_element = list_get(it); // the elements we would like to search
        if (specific_element==NULL) 
        {
            // error on list
            return -1;
        }
        if (specific_element->student_ID == id) 
        {
            break;
        }
        it = list_next(it);
    }
    if ( it == NULL ) return -1;
    if (specific_element->grades_list == NULL) return -1;
    struct list *grade_list = specific_element->grades_list;
    // Check if the course exists
    struct iterator *grade_it = list_begin(grade_list);
    struct success_data *grade_data = NULL;
    while (grade_it != NULL) {
        grade_data = list_get(grade_it);
        if (grade_data != NULL && strcmp(grade_data->course, name) == 0) {
            // Course already exists
            return -1;
        }
        grade_it = list_next(grade_it);
    }
    // Create a new course in list
    struct success_data *new_grade = malloc(sizeof(struct success_data));
    if (new_grade == NULL) // FAILED TO ALLOCATE
    {
        return -1; 
    }
    new_grade->course = strdup(name);
    if (new_grade->course == NULL) // FAILED TO ALLOCATE
    {
        free(new_grade);
        return -1; 
    }
    new_grade->grade = grade;
    // Add the new grade
    int flag = -1;
    flag = list_push_back(grade_list, new_grade);
    if (flag != 0) // FAILED TO ALLOCATE
    {
        free(new_grade->course);
        free(new_grade);
        return -1;
    }
    return 0;
};

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out) {
    if (grades == NULL || grades->all_students == NULL || out == NULL) {
        if (out != NULL) *out = NULL; // Set out to NULL
        return -1;
    }
    // Find the student ID
    struct iterator *it = list_begin(grades->all_students);
    struct student *specific_student = NULL;
    while (it != NULL) {
        specific_student = list_get(it);
        if (specific_student == NULL){
            if (*out != NULL) *out = NULL; // Set out to NULL
            return -1;
            }
        if (specific_student->student_ID == id)
            break;
        it = list_next(it);
    }
    if (it == NULL) {
        *out = NULL; // Student doesnt exist
        return -1;
    }

    *out = strdup(specific_student->student_name);
    if (*out == NULL) {
        return -1; // FAILED TO ALLOCATE
    }
    // Calculate the average grade
    struct iterator *grade_it = list_begin(specific_student->grades_list);
    struct success_data *grade_data = list_get(grade_it);
    if(grade_data==NULL) {
        free(*out);
        *out = NULL;
        return -1;
    }
    int sum = 0;
    int counter = 0;
    while (grade_it != NULL) {
        grade_data = list_get(grade_it);
        if (grade_data != NULL) {
            sum += grade_data->grade;
            counter++;
        }
        grade_it = list_next(grade_it);
    }
    if (counter == 0) return 0;
    float average = (float)sum / counter;
    return average; // Return 0 if no grades
};


static int print_student_individual(struct student *specific_student){
    // Print student info
    printf("%s %d:", specific_student->student_name, specific_student->student_ID);
    struct iterator *grade_it = list_begin(specific_student->grades_list);
    struct success_data *grade_data = NULL;
    int first =1; 
    while (grade_it != NULL) {
        grade_data = list_get(grade_it);
        if (grade_data == NULL) return -1;
        if (first!=1) printf(", ");
        printf(" %s %d", grade_data->course, grade_data->grade);
        grade_it = list_next(grade_it);
    }
    printf("\n");
    return 0;
}
;


/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id) {
    if (grades == NULL) return -1;
    if(grades->all_students == NULL) return -1;
    // Find the student ID
    struct iterator *it = list_begin(grades->all_students);
    struct student *specific_student = NULL;
    while (it != NULL) {
        specific_student = list_get(it);
        if (specific_student == NULL) return -1;
        if (specific_student->student_ID == id)
            break;
        it = list_next(it);
    }
    if (it == NULL) {
        return -1; // Student doesnt exist
    }
    // Print student info
    int flag =-1;
    flag = print_student_individual(specific_student);
    if (flag ==-1) return -1;
    return 0;
}
;


/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order 
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades) {
    if (grades == NULL || grades->all_students == NULL) {
        return -1;
    }
    // go over all students
    struct iterator *it = list_begin(grades->all_students);
    struct student *specific_student = NULL;
    while (it != NULL) {
        specific_student = list_get(it);
        if (specific_student == NULL) return -1;
        int flag = print_student_individual(specific_student);
        if (flag == -1) return -1;
        it = list_next(it);
    }

    return 0;
}
;