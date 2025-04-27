/**
 * @file group.c
 * @author Luke Early
 * Source file for group component.
 */

#include "group.h"
#include "input.h"

/**
 * This function dynamically allocates storage for the Group, initializes its 
 * fields (to store the two resizable arrays) and returns a pointer to the new 
 * Group. It’s kind of like a constructor in Java.
 * 
 * @return pointer to Group Struct
 */
Group *makeGroup()
{
  Group *g1 = ( Group *)malloc( sizeof( Group ) ); // pointer to new group
  
  /* Pointer to a list of pointers to Item Structs */
  Item **itemList = ( Item **)malloc( INIT_CAPACITY * sizeof( Item ) );
  g1->iList = itemList;
  g1->iCap = INIT_CAPACITY;
  g1->iCount = 0;

  /* Pointer to a list of pointers to Member Structs */
  Member **memberList = ( Member **)malloc( INIT_CAPACITY * sizeof( Member ) );
  g1->mList = memberList;
  g1->mCap = INIT_CAPACITY;
  g1->mCount = 0;

  return g1;
}

/**
 * This function frees the memory used to store the given Group, including 
 * freeing space for all the Items, Members, and Member SaleItem lists, freeing 
 * the resizable arrays of pointers and freeing space for the Group struct itself.
 * 
 * @param group group containing all memory to be freed
 */
void freeGroup( Group *group )
{
  free( group->iList );
  free( group->mList );
  for ( int i = 0; i < group->mCount; i++ ){
    free( group->iList[ i ]->mList );
  }
  free( group );
}

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
void readItems( char const *filename, Group *group )
{
  // validate and open file
  FILE *itemFilePtr = fopen( filename, "r" );
  if ( itemFilePtr == NULL ) {
    printf( "Can't open file: %s\n", filename );
    exit( EXIT_FAILURE );
  }

  // first line to process
  char *itemLine = readLine( itemFilePtr );

  // continue process until EOF
  while ( itemLine != NULL ) {
    // dynalloc Item struct
    Item *itemPtr = ( Item *)malloc( sizeof( Item ) );

    int fieldCountTracker = 0;
    int offset = 0;
    int nextOffset = 0;

    int itemId = 0;
    int itemCost = 0;
    char itemName[ NAME_MAX ] = "";


    if ( sscanf( itemLine, " %d%n", &itemId, &offset ) == 1 ) {
      fieldCountTracker++;
    } else {
      printf( "Invalid item file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    if ( sscanf( itemLine + offset, " %d%n", &itemCost, &nextOffset ) == 1 ) {
      fieldCountTracker++;
      offset = offset + nextOffset;
    } else {
      printf( "Invalid item file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    if ( sscanf( itemLine + offset, " %30c%n", itemName, &nextOffset ) == 1 ) {
      fieldCountTracker++;
    } else {
      printf( "Invalid item file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    itemName[ strlen( itemName ) ] = '\0';
    itemPtr->id = itemId;
    itemPtr->cost = itemCost;
    strncpy( itemPtr->name, itemName, NAME_MAX );
    
    itemPtr->mList = (char**)malloc( INIT_CAPACITY*sizeof( char* ) );
    for ( int i = 0; i < INIT_CAPACITY; i++ ) {
      itemPtr->mList[ i ] = (char*)malloc( ID_MAX * sizeof( char ) );
    }
    itemPtr->mCap = INIT_CAPACITY;
    itemPtr->mCount = 0;

    group->iList[ group->iCount++ ] = itemPtr;

    itemLine = readLine( itemFilePtr );
  }
}

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
void readMembers( char const *filename, Group *group )
{
  // validate and open file
  FILE *memberFilePtr = fopen( filename, "r" );
  if ( memberFilePtr == NULL ) {
    printf( "Can't open file: %s\n", filename );
    exit( EXIT_FAILURE );
  }

  // first line to process
  char *memberLine = readLine( memberFilePtr );

  // continue process until EOF
  while ( memberLine != NULL ) {
    // dynalloc Item struct
    Member *memberPtr = ( Member *)malloc( sizeof( Member ) );

    int fieldCountTracker = 0;

    int offset = 0;
    int nextOffset = 0;
    
    char memberId[ ID_MAX ] = "";
    char memberName[ NAME_MAX ] = "";

    if ( sscanf( memberLine, "%8s%n", memberId, &offset ) == 1 ) {
      fieldCountTracker++;
    } else {
      printf( "Invalid member file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    if ( sscanf( memberLine + offset, " %30c%n", memberName, &nextOffset ) == 1 ) {
      fieldCountTracker++;
    } else {
      printf( "Invalid member file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    if ( fieldCountTracker != 2 ) {
      printf( "Invalid member file: %s\n", filename );
      exit( EXIT_FAILURE );
    }

    for ( int i = 0; i < group->mCount; i++ ) {
      if ( strncmp( memberId, group->mList[ i ]->id, ID_MAX ) == 0 ) {
        printf( "Invalid member file: %s\n", filename );
        exit( EXIT_FAILURE );
      }
    }

    // init group member
    memberName[ strlen( memberName ) ] = '\0';
    strncpy( memberPtr->id, memberId, ID_MAX );
    strncpy( memberPtr->name, memberName, NAME_MAX );
    memberPtr->list = ( SaleItem **)malloc( INIT_CAPACITY * sizeof( SaleItem ) );
    for ( int i = 0; i < INIT_CAPACITY; i++ ) {
      memberPtr->list[ i ] = ( SaleItem *)malloc( sizeof( SaleItem ) );
      memberPtr->list[ i ]->itemPtr = ( Item *)malloc( sizeof( Item ) );
      memberPtr->list[ i ]->numSold = 0;
    }
    memberPtr->count = 0;
    memberPtr->capacity = INIT_CAPACITY;

    if ( group->mCount + 1 > group->mCap ) {
      group->mCap = group->mCap * 2;
      group->mList = ( Member**)realloc( group->mList, group->mCap * sizeof( Member ) );
    }

    group->mList[ group->mCount++ ] = memberPtr;

    memberLine = readLine( memberFilePtr );
  }
}

/** 
 * This function sorts the items in the given group based on input from user.
 * 
 * Used in conjunction with qsort()
 * 
 * @param group the group with the items that will be sorted
 * @param compare the function that will handle the sorting
 */
void sortItems( Group *group, int (* compare) (void const *va, void const *vb ))
{
  qsort( group->iList, group->iCount, sizeof( Item* ), compare );
}

/** 
 * This function sorts the members in the given group based on input from user.
 * 
 * Used in conjunction with qsort()
 * 
 * @param group the group with the items that will be sorted
 * @param compare the function that will handle the sorting
 */
void sortMembers( Group *group, int (* compare) (void const *va, void const *vb ))
{
  qsort( group->mList, group->mCount, sizeof( Member* ), compare );
}

/** 
 * This function prints all or some of the items based on test and str from user input. 
 * 
 * @param group from which the items will be printed
 * @param test function to handle determining if an item should be printed or not
 * @param str basis upon which items are or are not printed
 */
void listItems( Group *group, bool (*test)( Item const *item, char const *str ), char const *str )
{
  int totalMoneyMade = 0;
  int totalNumSold = 0;

  for ( int i = 0; i < group->iCount; i++ ) {
    if ( test( group->iList[ i ], str ) ) {
      printf( "%3d %-30s %6d %6d %6d", 
              group->iList[ i ]->id, 
              group->iList[ i ]->name, 
              group->iList[ i ]->cost, 
              group->iList[ i ]->numSold, 
              group->iList[ i ]->numSold * group->iList[ i ]->cost );
      printf( "\n" );
    }

    totalMoneyMade = totalMoneyMade + ( group->iList[ i ]->numSold * group->iList[ i ]->cost );
    totalNumSold = totalNumSold + group->iList[ i ]->numSold;
  }

  printf( "%-41s %6d %6d\n", "TOTAL", totalNumSold, totalMoneyMade  );
  printf( "\n" );
}

/** 
 * This function prints all or some of the members based on test and str from user input. 
 * 
 * @param group from which the members will be printed
 * @param test function to handle determining if an item should be printed or not
 * @param str basis upon which members are or are not printed
 */
void listMembers( Group *group, bool (*test)( Member const *member, char const *str ), char const *str )
{
  int totalMoneyMade = 0;
  int totalNumSold = 0;

  for ( int i = 0; i < group->mCount; i++ ) {
    int memberItemsSold = 0;
    int memberSales = 0;

    if ( test( group->mList[ i ], str ) ) {
      for ( int j = 0; j < group->mList[ i ]->count; j++ ) {
        int currItemSold = group->mList[ i ]->list[ j ]->numSold;
        int currItemCost = group->mList[ i ]->list[ j ]->itemPtr->cost;

        memberItemsSold = memberItemsSold + currItemSold;
        memberSales = memberSales + ( currItemSold * currItemCost );
      }
      printf( "%-8s %-30s %6d %6d", 
              group->mList[ i ]->id, 
              group->mList[ i ]->name,
              memberItemsSold,
              memberSales );
      printf( "\n" );

    }
    
    totalMoneyMade = totalMoneyMade + memberSales;
    totalNumSold = totalNumSold + memberItemsSold;
  }

  printf( "%-39s %6d %6d\n", "TOTAL", totalNumSold, totalMoneyMade  );
  printf( "\n" );
}
