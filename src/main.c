#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jplist.h"


typedef struct testing{
    int num;
    char* name;
} TestStruct;


// added here just to test out the find functions
int int_cmp(int x, int y){return x - y;}

int char_cmp(char x, char y) {return x - y;}

int struct_cmp(TestStruct x, TestStruct y) {return x.num - y.num;}

void multiply(int* i) {*i *= 2;}
void capitalize(char* i) {*i -= 32;}

int filter_for_int(int x){return x < 5;}
int filter_for_char(char x){return x >= 'A' && x <= 'F';}
int filter_for_str(str x){return x[0] == 'H';}
int filter_for_struct(TestStruct x){return x.num <= 15;}


// define the way the print_list_type function will print out the type
// TODO: make this less painful to set up
#define format_int(i) "%d", IGET(list, int, i)
#define format_char(i) "'%c'", IGET(list, char, i)
#define format_str(i) "\"%s\"", IGET(list, str, i)
#define format_test_struct(i) "(num: %d, name: \"%s\")", IGET(list, TestStruct, i).num, IGET(list, TestStruct, i).name

ADD_BASE_FUNC(int, format_int, int_cmp)
ADD_BASE_FUNC(char, format_char, char_cmp)
ADD_BASE_FUNC(str, format_str, strcmp) //strings are static, cannot be modified
ADD_BASE_FUNC(TestStruct, format_test_struct, struct_cmp)

// only used for the ease of writing new test functions
#define TEST_ARGS JPList* int_list, JPList* char_list, JPList* str_list, JPList* struct_list

//tests
void test_append(TEST_ARGS);
void test_print(TEST_ARGS);
void test_insert(TEST_ARGS);
void test_pop(TEST_ARGS);
void test_remove(TEST_ARGS);
void test_find(TEST_ARGS);
void test_map(TEST_ARGS);
void test_filter(TEST_ARGS);
void test_create();
void test_eq(TEST_ARGS);


int main(){
    JPList* ints = LIST_CTOR(int);
    JPList* chars = LIST_CTOR(char);
    JPList* words = LIST_CTOR(str);
    JPList* structs = LIST_CTOR(TestStruct);
    

    test_append(ints, chars, words, structs);

    test_pop(ints, chars, words, structs);

    test_insert(ints, chars, words, structs);

    test_remove(ints, chars, words, structs);

    test_find(ints, chars, words, structs);

    test_map(ints, chars, words, structs);

    test_filter(ints, chars, words, structs);

    test_create();

    test_eq(ints, chars, words, structs);

    list_dtor(&ints);
    list_dtor(&chars);
    list_dtor(&words);
    list_dtor(&structs);
    return 0;
}


void test_append(TEST_ARGS){
    puts("append test");
    for(int i = 0; i < 12; i++){
        append_int(int_list, i);
        append_char(char_list, i + 'a');
    }

    append_str(str_list, "Hello, ");
    append_str(str_list, "world");
    append_TestStruct(struct_list, (TestStruct){.num = 8, .name = "John"});
    append_TestStruct(struct_list, (TestStruct){.num = 17, .name = "Amy"});
    append_TestStruct(struct_list, (TestStruct){.num = 55, .name = "Cain"});

    test_print(int_list, char_list, str_list, struct_list);
}


void test_insert(TEST_ARGS){
    puts("insert test");
    insert_int(int_list, 1, 5);
    insert_char(char_list, 1, 'A');
    insert_str(str_list, 0, "AHOJ");
    insert_TestStruct(struct_list, 1, (TestStruct){.num = 4, .name = "Fred"});

    test_print(int_list, char_list, str_list, struct_list);
}


void test_pop(TEST_ARGS){
    puts("pop test");
    pop_int(int_list);
    pop_char(char_list);
    pop_str(str_list);
    pop_TestStruct(struct_list);

    test_print(int_list, char_list, str_list, struct_list);
}


void test_remove(TEST_ARGS){
    puts("remove test");
    remove_int(int_list, 1);
    remove_char(char_list, 1);
    remove_str(str_list, 0);
    remove_TestStruct(struct_list, 0);

    test_print(int_list, char_list, str_list, struct_list);
}


void test_print(TEST_ARGS){
    printf("char arr len %ld\n", char_list->len);
    printf("char arr size %ld\n", char_list->size);

    print_list_char(char_list);
    print_list_int(int_list);
    print_list_str(str_list);
    print_list_TestStruct(struct_list);
    
    puts("-------------------------");
}


void test_find(TEST_ARGS){
    puts("find test");
    int FIND_NUM = 5;
    char FIND_CH = 'o';
    char* FIND_ST = "Hello, ";
    TestStruct FIND_STRUCT = {.num = 4, .name = "Angel"}; 

    int int_index = find_int(int_list, FIND_NUM);
    int char_index = find_char(char_list, FIND_CH);
    int str_index = find_str(str_list, FIND_ST);
    int struct_index = find_TestStruct(struct_list, FIND_STRUCT);

    printf("found: int %d at: %d \nchar '%c' at: %d \nstr \"%s\" at: %d\nstruct \"%d\" at: %d\n\n", 
            FIND_NUM, int_index, FIND_CH, char_index, FIND_ST, str_index, FIND_STRUCT.num, struct_index);
    test_print(int_list, char_list, str_list, struct_list);
}

void test_map(TEST_ARGS){
    puts("map test");
    map_int(int_list, multiply);
    map_char(char_list, capitalize);

    test_print(int_list, char_list, str_list, struct_list);
}

void test_filter(TEST_ARGS){
    puts("filter test");
    JPList* filtered_int = filter_int(int_list, filter_for_int);
    JPList* filtered_char = filter_char(char_list, filter_for_char);
    JPList* filtered_str = filter_str(str_list, filter_for_str);
    JPList* filtered_struct = filter_TestStruct(struct_list, filter_for_struct);

    test_print(filtered_int, filtered_char, filtered_str, filtered_struct);

    list_dtor(&filtered_int);
    list_dtor(&filtered_char);
    list_dtor(&filtered_str);
    list_dtor(&filtered_struct);
}


void test_create(){
    puts("list make test");
    int arr[] = {1, 2, 3};
    JPList* intlist_decay = LIST(arr);

    JPList* intlist = LIST((int[]){1, 2, 3});
    JPList* charlist = LIST((char[]){'a', 'b', 'c'});
    JPList* strlist = LIST((str[]){"HELLO", "WORLD"});

    print_list_int(intlist_decay);
    print_list_int(intlist);
    print_list_char(charlist);
    print_list_str(strlist);

    list_dtor(&intlist_decay);
    list_dtor(&intlist);
    list_dtor(&charlist);
    list_dtor(&strlist);
    puts("-----------------------------");
}

void test_eq(TEST_ARGS){
    puts("list eq test");

    JPList* int_copy = LIST_CTOR(int);
    for(unsigned i = 0; i < int_list->len; i++){
        append_int(int_copy, IGET(int_list, int, i));
    }
    JPList* char_copy = LIST_CTOR(char);
    for(unsigned i = 0; i < char_list->len; i++){
        append_char(char_copy, IGET(char_list, char, i));
    }
    JPList* str_copy = LIST_CTOR(str);
    for(unsigned i = 0; i < str_list->len; i++){
        append_str(str_copy, IGET(str_list, str, i));
    }
    JPList* struct_copy = LIST_CTOR(TestStruct);
    for(unsigned i = 0; i < struct_list->len; i++){
        append_TestStruct(struct_copy, IGET(struct_list, TestStruct, i));
    }

    switch(eq_list_int(int_list, int_copy)){
    case 1:
        puts("int Success");
        break;
    default:
        puts("int fail!!!");
        break;
    }
    switch(eq_list_char(char_list, char_copy)){
    case 1:
        puts("char Success");
        break;
    default:
        puts("char fail!!!");
        break;
    }
    switch(eq_list_str(str_list, str_copy)){
    case 1:
        puts("str Success");
        break;
    default:
        puts("str fail!!!");
        break;
    }
    switch(eq_list_TestStruct(struct_list, struct_copy)){
    case 1:
        puts("struct Success");
        break;
    default:
        puts("struct fail!!!");
        break;
    }
    list_dtor(&int_copy);
    list_dtor(&char_copy);
    list_dtor(&str_copy);
    list_dtor(&struct_copy);
}