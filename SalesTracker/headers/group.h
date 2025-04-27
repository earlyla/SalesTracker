/**
 * @file group.h
 * @author Luke Early
 * Group header file containing all function prototypes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NAME_MAX 30
#define ID_MAX 8
#define INIT_CAPACITY 5

struct ItemStruct {
  int id;
  char name[ NAME_MAX ];
  int cost;
  int numSold;
  char **mList;
  int mCap;
  int mCount;
};
typedef struct ItemStruct Item;

struct SaleItemStruct {
  Item *itemPtr;
  int numSold;
};
typedef struct SaleItemStruct SaleItem;

struct MemberStruct {
  char id[ ID_MAX ];
  char name[ NAME_MAX ];
  SaleItem **list;
  int count;
  int capacity; // initial capacity of 5
};
typedef struct MemberStruct Member;

struct GroupStruct {
  int iCount;
  Item **iList;
  int iCap;
  int mCount;
  Member **mList;
  int mCap;
};
typedef struct GroupStruct Group;

/**
 * This function dynamically allocates storage for the Group, initializes its 
 * fields (to store the two resizable arrays) and returns a pointer to the new 
 * Group. It’s kind of like a constructor in Java.
 * 
 * @return pointer to Group Struct
 */
Group *makeGroup();

/**
 * This function frees the memory used to store the given Group, including 
 * freeing space for all the Items, Members, and Member SaleItem lists, freeing 
 * the resizable arrays of pointers and freeing space for the Group struct itself.
 * 
 * @param group group containing all memory to be freed
 */
void freeGroup( Group *group );

/** 
 * This function reads all the items from an item file with the given name. 
 * 
 * It makes an instance of the Item struct for an item in the file and stores a 
 * pointer to that Item in the resizable item array in group.
 * 
 * Exits unsuccessful if criteria for file are not met.
 * 
 * @param filename is the name of the group file passed to the function
 * @param group is the pointer to the group that the file will read the item into
 */
void readItems( char const *filename, Group *group );

/** 
 * This function reads all the members from a member file with the given name.
 * 
 * Makes an instance of the Member struct for a member in the file and stores a 
 * pointer to that Member in the resizable member array in group.
 * 
 * Exits unsuccessful if:
 *     - name longer than 30
 *     - ID longer than 8
 *     - missing a field
 *     - two IDs are the same
 * 
 * @param filename string for the member file name
 * @param group pointer to group this will populate
 */
void readMembers( char const *filename, Group *group );

/** 
 * This function sorts the items in the given group based on input from user.
 * 
 * Used in conjunction with qsort()
 * 
 * @param group the group with the items that will be sorted
 * @param compare the function that will handle the sorting
 */
void sortItems( Group *group, int (* compare) (void const *va, void const *vb ));

/** 
 * This function sorts the members in the given group based on input from user.
 * 
 * Used in conjunction with qsort()
 * 
 * @param group the group with the items that will be sorted
 * @param compare the function that will handle the sorting
 */
void sortMembers( Group *group, int (* compare) (void const *va, void const *vb ));

/** 
 * This function prints all or some of the items based on test and str from user input. 
 * 
 * @param group from which the items will be printed
 * @param test function to handle determining if an item should be printed or not
 * @param str basis upon which items are or are not printed
 */
void listItems( Group *group, bool (*test)( Item const *item, char const *str ), char const *str );

/** 
 * This function prints all or some of the members based on test and str from user input. 
 * 
 * @param group from which the members will be printed
 * @param test function to handle determining if an item should be printed or not
 * @param str basis upon which members are or are not printed
 */
void listMembers( Group *group, bool (*test)( Member const *member, char const *str ), char const *str );
