/////////////////////////
// dynamic array list  //
// author: Juripan     //
/////////////////////////

#include <stdio.h>
#include <stdlib.h>

#ifndef __JPLIST__H //include guarding
#define __JPLIST__H

#define DEFAULT_SIZE__ 10 //default List size

//////////////////
// custom types ///////////////////////////////////////////////////////////////////////////////////
//////////////////

// List type
typedef struct list_h {
    size_t size; // size of the array in memory, can change at runtime, defaults to 10
    size_t len; // current amount of items stored in the array
    void* data; // the array itself, contains the items 
} JPList;

// string type (char*) because of the "function name"_"type" naming scheme
// char* is static cannot be modified
// (for example: append_char*() is an invalid function name)
typedef char* str;

///////////////////
//      QOL      //////////////////////////////////////////////////////////////////////////////////
///////////////////

//alternate syntax for accessing list elements (no "((type*)list->data)[index]" needed)
#define IGET(list, type, index) (((type*)list->data)[index])

//prints an error message to stderr 
#define PRINTERR(msg) fprintf(stderr, msg "\n")

/////////////////////////
// list base "methods" ////////////////////////////////////////////////////////////////////////////
/////////////////////////

/*
 * macro that calls other macros that initialize the base functions for list manipulation,
 * type is the type of data you would like to store,
 * format_str is a string that determines how the array will be printed (formatting just like in printf),
 * cmp_func is a function pointer that determines how the arrays content will be compared (example: strcmp() in <string.h>)
*/

#define ADD_BASE_FUNC(type, format_str, cmp_func) \
ADD_PRINT_LIST(type, format_str)                  \
ADD_APPEND(type)                                  \
ADD_POP(type)                                     \
ADD_INSERT(type)                                  \
ADD_REMOVE(type)                                  \
ADD_FIND(type, cmp_func)                          \
ADD_MAP(type)                                     \
ADD_FILTER(type)                                  \
ADD_LIST_EQ(type, cmp_func)                       \

/*
 * macro that calls a fuction that returns a pointer to a struct of type JPList,
 * struct is allocated on the heap that constains an empty array of the type specified,
 */
#define LIST_CTOR(type) list_ctor_(sizeof(type), DEFAULT_SIZE__, NULL)

/*
 * macro that calls a fuction that returns a pointer to a struct of type JPList,
 * struct is allocated on the heap that contains the array of items given,
 * doesn't work for creating an empty list (use LIST_CTOR for that)
 */
#define LIST(...) list_ctor_(sizeof(__VA_ARGS__[0]), sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0]), (void*)__VA_ARGS__)

JPList *list_ctor_(size_t type_size, size_t size, void* array);


/*
 * deallocates the memory tied to the list that is passed in,
 * sets its pointer to NULL to prevent accidental use
 */
void list_dtor(JPList **list_ptr);


/*
 * prints out the data that is in the list,
 * format_macro sets the way it is printed (gets called with the index as a parameter inside of printf)
 */
#define ADD_PRINT_LIST(type, format_macro) \
void print_list_##type(JPList *list){      \
    printf("[");                           \
    for(size_t i = 0; i < list->len; i++){ \
        printf(format_macro(i));           \
        printf(", ");                      \
    }                                      \
    printf("]\n");                         \
}

/*
 * adds an item to the end of the array and increments its length,
 * if its about to exceed its size then it reallocs a new bigger array
    and updates its pointer to point to the new array
*/
#define ADD_APPEND(type)                        \
void append_##type(JPList *list, type item){    \
    if(list->len >= list->size - 1) {           \
        LIST_REALLOCATOR__(list, type)          \
    }                                           \
    ((type*)list->data)[list->len++] = item;    \
}


/*
 * pops the last item out and decrements the length variable,
 * if the list is empty then it prints "list is empty, cant pop more items" 
    to stderr and crashes the program
*/
#define ADD_POP(type)                                      \
type pop_##type(JPList *list){                             \
    if (list->len <= 0){                                   \
        PRINTERR("JPList is empty, can't pop more items"); \
        exit(EXIT_FAILURE);                                \
    }                                                      \
    list->len--;                                           \
    return ((type*)list->data)[list->len];                 \
}


/*
 * inserts an item into the array at the index that is passed in
    and pushes all of the items in front of it,
 * if you pass in an index thats out of range it throws an error and kills the program
 * if you want to add to the end of the list use append_"type" instead
 */
#define ADD_INSERT(type)                                                      \
void insert_##type(JPList* list, const unsigned int index, const type item) { \
    if(index > list->len - 1) {                                               \
        PRINTERR("Invalid insert operation, index out of range");             \
        exit(EXIT_FAILURE);                                                   \
    }                                                                         \
    if(list->size <= list->len) {                                             \
        LIST_REALLOCATOR__(list, type)                                        \
    }                                                                         \
    for(size_t i = list->len - 1; i + 1 > index; i--) {                       \
        ((type*)list->data)[i + 1] = ((type*)list->data)[i];                  \
    }                                                                         \
    ((type*)list->data)[index] = item;                                        \
    list->len++;                                                              \
}


/*
 * removes an item at the give index and moves all of the items to fill in the gap left behind
 * if you pass in an index thats out of range it throws an error and kills the program
 */
#define ADD_REMOVE(type)                                            \
void remove_##type(JPList* list, const unsigned int index) {        \
    if(index > list->len - 1) {                                     \
        PRINTERR("Invalid remove operation, index out of range");   \
        exit(EXIT_FAILURE);                                         \
    }                                                               \
    for(size_t i = index; i + 1 < list->len; i++) {                 \
        ((type*)list->data)[i] = ((type*)list->data)[i + 1];        \
    }                                                               \
    list->len--;                                                    \
}

/*
 * finds the item that is passed in according to the criteria defined in the cmp_func,
 * returns the items index,
 * if not found returns -1
 */
#define ADD_FIND(type, cmp_func)                        \
int find_##type(JPList* list, type item) {              \
    for(size_t i = 0; i < list->len; i++) {             \
        if (cmp_func(IGET(list, type, i), item) == 0){  \
            return i;                                   \
        }                                               \
    }                                                   \
    return -1;                                          \
}

/*
 * runs the passed in function on every item in the list,
 * function accepts a pointer to the type of item in the list (modifies the list in-place)
 */
#define ADD_MAP(type)                                       \
void map_##type(const JPList* list, void(*func)(type*)) {   \
    for(size_t i = 0; i < list->len; i++) {                 \
        func(&IGET(list, type, i));                         \
    }                                                       \
}


/*
 * creates a new list that contains the items that satisfy the condition criteria,
 * returns a pointer to the new list
 */
#define ADD_FILTER(type)                                        \
JPList* filter_##type(JPList* list, int(*condition)(type)) {    \
    JPList* res = list_ctor_(sizeof(type), list->len, NULL);    \
    for(size_t i = 0; i < list->len; i++){                      \
        if(condition(IGET(list, type, i))) {                    \
            IGET(res, type, res->len) = IGET(list, type, i);    \
            res->len++;                                         \
        }                                                       \
    }                                                           \
    return res;                                                 \
}

/*
 * compares 2 lists by their length and their content using the compare function,
 * returns 1 if they are the same, else returns 0
 */
#define ADD_LIST_EQ(type, cmp_func)                                     \
int eq_list_##type(JPList* list1, JPList* list2){                       \
    if(list1->len != list2->len){                                       \
        return 0;                                                       \
    }                                                                   \
    for(unsigned i = 0; i < list1->len; i++){                           \
        if(cmp_func(IGET(list1, type, i), IGET(list2, type, i)) != 0){  \
            return 0;                                                   \
        }                                                               \
    }                                                                   \
    return 1;                                                           \
} 

/*
 * macro that doubles the size of the array and reallocates the memory,
 * crashes the program if the reallocation failed,
 * only used in the implementation, not intended to be used outside of the header
 */
#define LIST_REALLOCATOR__(list, type)                              \
    list->size *= 2;                                                \
    list->data = realloc(list->data, list->size * sizeof(type));    \
    if(list->data == NULL) exit(EXIT_FAILURE);

#endif //final endif