#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_h {
    size_t size; // size of the array in memory measured in slots, can change at runtime, defaults to 10
    size_t len; // current amount of items stored in the array
    void* data; // the array itself, contains the items
} JPList;


JPList* list_ctor_(size_t type_size, size_t size, void* array){
    JPList *list = malloc(sizeof(JPList));
    if(list == NULL) exit(EXIT_FAILURE);
    
    list->data = malloc(type_size * size);
    if(list->data == NULL) exit(EXIT_FAILURE);
    
    if(array == NULL){
        list->len = 0;
        list->size = size;
    } else {
        memmove(list->data, array, type_size * size);
        list->len = size;
        list->size = size * 2;
    }

    return list;
}

void list_dtor(JPList **list_ptr){
    free((*list_ptr)->data);
    free(*list_ptr);
    *list_ptr = NULL;
}
