/**
 * @file fundraiser.c
 * @author Luke Early
 * Main file for fundraiser program.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "group.h"
#include "input.h"

/**
 * Prints message to stdout informing user legal CLA
 */
void usage() {
  printf( "usage: fundraiser item-file member-file\n" );
  exit( EXIT_FAILURE );
}

/**
 * Prints information to stdout if files can't be opened
 * 
 * @param filename to be printed at stdout
 */
void badFile( char filename[] ) {
  printf( "Can't open file: %s\n", filename );
  exit( EXIT_FAILURE );
}

/**
 * Returns true for all items
 * 
 * @param item to test
 * @param str by which to test item
 * 
 * @return true for every item
 * 
 */
bool testItems( Item const *item, char const *str ) {
  return true;
}

/**
 * Returns true for all members
 * 
 * @param member to test
 * @param str by which to test member
 * 
 * @return true for every member
 * 
 */
bool testMembers( Member const *member, char const *str ) {
  return true;
}

/**
 * Find member based on their ID
 * 
 * @param member to test
 * @param str ID to test against member ID
 * 
 * @return true if member's ID matches str, else false
 * 
 */
bool findMemberById( Member const *member, char const *str ) {
  if ( strncmp( member->id, str, ID_MAX ) == 0 ) {
    return true;
  } else {
    return false;
  }
}

/**
 * Searches for items based off of strings (case insensitive)
 * 
 * @param item to test
 * @param str by which to test item's name
 * 
 * @return true if item name contains str, false else
 * 
 */
bool searchForItemByString( Item const *item, char const *str ) {
  bool flag = false;
  int strLoc = 0;
  
  for ( int i = 0; i < strlen( item->name ); i++ ) {
    int itemChar = tolower( item->name[ i ] );
    int strChar = tolower( str[ strLoc ] );

    if ( itemChar == strChar ) {
      strLoc++;
    } else if ( strLoc == strlen( str ) ) {
      flag = true;
      break;
    } else {
      strLoc = 0;
    } 
  }

  return flag;
}

/**
 * Searches for members based off of strings (case insensitive)
 * 
 * @param member to test
 * @param str by which to test members's name
 * 
 * @return true if member name contains str, false else
 * 
 */
bool searchForMembersByString( Member const *member, char const *str ) {
  bool flag = false;
  int strLoc = 0;
  
  for ( int i = 0; i < strlen( member->name ); i++ ) {
    int memChar = tolower( member->name[ i ] );
    int strChar = tolower( str[ strLoc ] );

    if ( memChar == strChar ) {
      strLoc++;
    } else if ( strLoc == strlen( str ) ) {
      flag = true;
      break;
    } else {
      strLoc = 0;
    } 
  }

  return flag;
}

/**
 * Searches for item based off of strings (case insensitive)
 * 
 * @param item to test
 * @param str by which to if memberId is in Item
 * 
 * @return true if item contains member ID, false else
 * 
 */
bool compareItemBySeller( Item const *item, char const *str ) {
  bool flag = false;

  for ( int i = 0; i < item->mCount; i++ ){
    if ( strncmp( item->mList[ i ], str, 8 ) == 0 ) {
      flag = true;
    } 
  }

  return flag;
}

/**
 * Compares the name of one item to the name
 * of another.
 * 
 * @param va void pointer to item
 * @param vb void pointer to item
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int compareItemName( void const *va, void const *vb ) {
  Item **a = ( Item **) va;
  Item **b = ( Item **) vb;

  return( strncmp( (*a)->name, (*b)->name, NAME_MAX ) );
}

/**
 * Finds the location of a member in memList.
 * 
 * @param va void pointer to item
 * @param vb void pointer to item
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int findMemberIdx( Group *group, char *memberId ) {
  int idx = 0;
  
  for ( int i = 0; i < group->mCount; i++ ) {
    if ( strncmp( group->mList[ i ]->id, memberId, 8 ) == 0 ) {
      idx = i;
    }
  }

  return idx;
}

/**
 * Compares the id of one item to another.
 * 
 * @param va void pointer to item
 * @param vb void pointer to item
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int compareItemId( void const *va, void const *vb ) {
  Item **a = ( Item **) va;
  Item **b = ( Item **) vb;

  if ( (*a)->id > (*b)->id ) {
    return 1;
  } else if ( (*a)->id < (*b)->id ) {
    return -1;
  } else {
    return 0;
  }
}

/**
 * Compares the name of one member to another.
 * 
 * @param va void pointer to member
 * @param vb void pointer to member
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int compareMemberName( void const *va, void const *vb ) {
  Member **a = ( Member **) va;
  Member **b = ( Member **) vb;

  return( strncmp( (*a)->name, (*b)->name, NAME_MAX ) );
}

/**
 * Compares the sales of one member to another.
 * 
 * @param va void pointer to member
 * @param vb void pointer to member
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int compareMemberSales( void const *va, void const *vb ) {
  Member **a = ( Member **) va;
  Member **b = ( Member **) vb;
  int aTotSales = 0;
  int bTotSales = 0;

  for ( int i = 0; i < (*a)->count; i++ ) {
    int valueToAdd = 0;
    valueToAdd = (*a)->list[ i ]->itemPtr->cost * (*a)->list[ i ]->numSold;
    aTotSales = aTotSales + valueToAdd;
  }

  for ( int i = 0; i < (*b)->count; i++ ) {
    int valueToAdd = 0;
    valueToAdd = (*b)->list[ i ]->itemPtr->cost * (*b)->list[ i ]->numSold;
    bTotSales = bTotSales + valueToAdd;
  }
  
  if ( aTotSales > bTotSales ) {
    return -1;
  } else if ( aTotSales < bTotSales ) {
    return 1;
  } else {
    return strncmp( (*a)->id, (*b)->id, ID_MAX );
  }
}

/**
 * Compares the ID of one member to another.
 * 
 * @param va void pointer to member
 * @param vb void pointer to member
 * @return integer based on item names:
 *         - less than 0 if va comes before vb
 *         - zero if va is the same as vb
 *         - more than 0 if va comes after vb
 */
int compareMemberID( void const *va, void const *vb ) {
  Member **a = ( Member **) va;
  Member **b = ( Member **) vb;

  return( strncmp( (*a)->id, (*b)->id, ID_MAX ) );
}

int main( int argc, char **argv )
{
  // check that the file names are valid
  if ( argc != 3 ) {
    usage();
  }

  /**
   * The following section handles CLA checks
   */
  char itemFileStr[ 20 ];
  char memberFileStr[ 20 ];
  
  if ( strncmp( argv[ 1 ], "items-", 6 ) == 0 ) {
    strcpy( itemFileStr, argv[ 1 ] );
  } else {
    badFile( argv[ 1 ] );
  }

  if ( strncmp( argv[ 2 ], "members-", 8 ) == 0 ) {
    strcpy( memberFileStr, argv[ 2 ] );
  } else {
    badFile( argv[ 2 ] );
  }

  /**
   * This is the Group that will be used for the duration of this program
   */
  Group *gp1 = makeGroup();

  /**
   * This section contains calls to functions which handle 
   * item files and populating item lists
   */
  readItems( itemFileStr, gp1 );

  /**
   * This section contains calls to functions which handle 
   * member files and populating member lists
   */
  readMembers( memberFileStr, gp1 );

  /**
   * This section handles user input
   */
  printf( "cmd> " );
  char *rawUserCommand = readLine( stdin );
  
  while ( rawUserCommand != NULL ) {
    char firstCommand[ 6 ];
    char secondCommand[ 10 ];
    char thirdCommand[ 10 ];
    char searchParam[ 15 ];
    
    int offset = 0;
    int secondOffset = 0;    

    sscanf( rawUserCommand, "%s%n", firstCommand, &offset );

    if ( strncmp( firstCommand, "list", 4 ) == 0 ) {
      if ( sscanf( rawUserCommand + offset, " %s%n", secondCommand, &secondOffset ) == 1 ) {
        // list items
        if ( strcmp( secondCommand, "items" ) == 0 ) {
          printf( "%s\n", rawUserCommand );
          sortItems( gp1, compareItemId  );
          printf( "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total" );
          listItems( gp1, testItems, secondCommand ); 
                    
        } 
        // list item names
        else if ( strcmp( secondCommand, "item" ) == 0 ) {
          if ( sscanf( rawUserCommand + offset + secondOffset, " %s", thirdCommand ) == 1 
               && strcmp( thirdCommand, "names" ) == 0 ) {
            printf( "%s\n", rawUserCommand );
            sortItems( gp1, compareItemName );
            printf( "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total" );
            listItems( gp1, testItems, secondCommand );

          }
        } 
        // list members
        else if ( strcmp( secondCommand, "members" ) == 0 ) {
          printf( "%s\n", rawUserCommand );
          sortMembers( gp1, compareMemberID );
          printf( "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total" );
          listMembers( gp1, testMembers, secondCommand );
          
        } 
        // list member names
        else if ( strcmp( secondCommand, "member" ) == 0 ) {
          if ( sscanf( rawUserCommand + offset + secondOffset, " %s", thirdCommand ) == 1 
               && strcmp( thirdCommand, "names" ) == 0 ) {
            printf( "%s\n", rawUserCommand );
            sortMembers( gp1, compareMemberName );
            printf( "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total" );
            listMembers( gp1, testMembers, secondCommand );

          } else if ( sscanf( rawUserCommand + offset + secondOffset, " %s", thirdCommand ) == 1 ) {
            printf( "%s\n", rawUserCommand );
            printf( "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total" );
            sortItems( gp1, compareItemId );
            listItems( gp1, compareItemBySeller, thirdCommand );

          } else {
            printf( "Invalid Command\n" );
          }
        } 
        // list topsellers
        else if ( strcmp( secondCommand, "topsellers" ) == 0 ) {
          printf( "%s\n", rawUserCommand );
          sortMembers( gp1, compareMemberSales );
          printf( "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total" );
          listMembers( gp1, testMembers, secondCommand );

        } else {
          printf( "Invalid Command\n" );
        }
      } else {
        printf( "Invalid Command\n" );
      }
    } else if ( strncmp( firstCommand, "search", 6 ) == 0 ) {
      // search items <param>
      if ( sscanf( rawUserCommand + offset, " %s%n", secondCommand, &secondOffset ) == 1 
           && strcmp( secondCommand, "item" ) == 0 ) {
        printf( "%s\n", rawUserCommand );
        sscanf( rawUserCommand + offset + secondOffset, " %s", searchParam );
        sortItems( gp1, compareItemId );
        printf( "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total" );
        listItems( gp1, searchForItemByString, searchParam );
      }
      // search members <param>
      else if ( sscanf( rawUserCommand + offset, " %s%n", secondCommand, &secondOffset ) == 1 
           && strcmp( secondCommand, "member" ) == 0 ) {
        printf( "%s\n", rawUserCommand );
        sscanf( rawUserCommand + offset + secondOffset, " %s", searchParam );
        sortMembers( gp1, compareMemberID );
        printf( "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total" );
        listMembers( gp1, searchForMembersByString, searchParam );
      }
    } else if ( strncmp( firstCommand, "sale", 4 ) == 0 ) {
      int currOffset = offset;
      char sellersId[ ID_MAX ];
      int itemId = 0;
      int numSold = 0;
      
      // scan in all info
      sscanf( rawUserCommand + currOffset, " %s%n", sellersId, &secondOffset );
      currOffset = currOffset + secondOffset;
      sscanf( rawUserCommand + currOffset, " %d%n", &itemId, &secondOffset );
      currOffset = currOffset + secondOffset;
      sscanf( rawUserCommand + currOffset, " %d", &numSold );

      if ( numSold < 0 ) {
        printf( "Invalid Command\n" );
      }

      printf( "%s\n", rawUserCommand );

      // find sellersId, give them the sale
      for ( int i = 0; i < gp1->mCount; i++ ) {
        if ( strncmp( gp1->mList[ i ]->id, sellersId, 8 ) == 0 ) {
          if ( gp1->mList[ i ]->count == 0 ) {
            Item *itemSold = NULL;
            for ( int j = 0; j < gp1->iCount; j++ ) {
              if ( gp1->iList[ j ]->id == itemId ) {
                itemSold = gp1->iList[ j ];
                // increase num sold in list
                gp1->iList[ j ]->numSold = gp1->iList[ j ]->numSold + numSold;

                // new SaleItem, increase number sold
                gp1->mList[ i ]->list[ 0 ]->itemPtr = itemSold;
                gp1->mList[ i ]->list[ 0 ]->numSold = numSold;

                // increase count of items
                gp1->mList[ i ]->count = gp1->mList[ i ]->count + 1;

                // add seller to list
                gp1->iList[ i ]->mList[ 0 ] = sellersId;
                gp1->iList[ i ]->mCount++;
                break;
              }
            }
            if ( itemSold == NULL ) {
              printf( "Invalid Command\n" );
            }
          } else {
            if ( ( gp1->mList[ i ]->count + 1 ) >= gp1->mList[ i ]->capacity ) {
              int newCap = gp1->mList[ i ]->capacity * 2;
              gp1->mList[ i ]->list = (SaleItem **)realloc( gp1->mList[ i ]->list, newCap*sizeof( SaleItem ) );

              for ( int j = gp1->mList[ i ]->count; j < newCap; j++ ) {
                gp1->mList[ i ]->list[ j ] = ( SaleItem *)malloc( sizeof( SaleItem ) );
                gp1->mList[ i ]->list[ j ]->itemPtr = ( Item *)malloc( sizeof( Item ) );
                gp1->mList[ i ]->list[ j ]->numSold = 0;
              }
            }

            for ( int j = 0; j < gp1->mList[ i ]->count; j++ ) {
              if ( gp1->mList[ i ]->list[ j ]->itemPtr->id == itemId ) {
                // increase in group item list
                gp1->iList[ j ]->numSold = gp1->iList[ j ]->numSold + numSold;

                // increase in member saleitem list
                gp1->mList[ i ]->list[ j ]->numSold = gp1->mList[ i ]->list[ j ]->numSold + numSold;
                
              } else {
                Item *itemSold = NULL;
                int numItems = gp1->mList[ i ]->count;
                itemSold = gp1->iList[ j ];
                // increase num sold in list
                gp1->iList[ j ]->numSold = gp1->iList[ j ]->numSold + numSold;

                // new SaleItem, increase number sold
                gp1->mList[ i ]->list[ numItems ]->itemPtr = itemSold;
                gp1->mList[ i ]->list[ numItems ]->numSold = numSold;

                // increase count of items
                gp1->mList[ i ]->count = gp1->mList[ i ]->count + 1;

                // add seller to list
                if ( gp1->iList[ i ]->mCount + 1 > gp1->iList[ i ]->mCap ) {
                  int newCap = gp1->iList[ i ]->mCount * 2;
                  gp1->iList[ i ]->mList = (char **)realloc( gp1->iList[ i ]->mList, newCap*sizeof( char* ) );
                  
                  for ( int j = gp1->iList[ i ]->mCount; j < gp1->iList[ i ]->mCap; j++ ) {
                    gp1->iList[ i ]->mList[ j ] = (char*)malloc( ID_MAX * sizeof( char ) );
                  }
                }
                gp1->iList[ i ]->mList[ gp1->iList[ i ]->mCount++ ] = sellersId;
                break;
              }
            }
          }
          printf( "\n" );
          break;
        }
      }
    } else if ( strncmp( rawUserCommand, "quit", 4 ) == 0 ) {
      printf( "%s", rawUserCommand );
      printf( "\n" );
      break;
    }

    if ( feof( stdin ) ) {
      break;
    } else {
      printf( "cmd> " );
      rawUserCommand = readLine( stdin );
    }
  }

  freeGroup( gp1 );
  return EXIT_SUCCESS;
}
