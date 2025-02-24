# JPLIST

A dynamic array list implementation in C. Works for any type (even your custom structs)

## Quick start:
1. Include the jplist header file
```C
#include "jplist.h"
```
2. Make a print format macro for the types that you will use inside of the JPList struct (used for printing out the list)
```C
#define format_Person(i) "(name: \"%s\", age: %d)", iget(list, Person, i).name, iget(list, Person, i).age
```
3. Make a comparison function that gets called when comparing items in the JPList struct (it should act like strcmp from "string.h")
```C
int int_cmp(int x, int y){return x - y;}
```
4. Call the "ADD_BASE_FUNC" macro to initialize all of the functions that can be used for the JPList (pass in the type, print format macro you made earlier, and the comparison function)
```C
ADD_BASE_FUNC(int, format_int, int_cmp)
```
**IMPORTANT: you cannot use pointers as a type in the list UNLESS you typedef it** \
For example:
```C
typedef int* int_ptr;
ADD_BASE_FUNC(int_ptr, format_int_ptr, int_ptr_cmp)
```
NOTE: you can also add specific features only by using the "ADD" macros defined in the jplist header file

## Indexing:
Since the JPList struct stores the items in a void* so indexing the items directly is a mess,
thats why I added a "IGET" macro \
Example usage: \
```C
int val = IGET(ints_list, int, 5);
IGET(ints_list, int, 4) = 21;
```