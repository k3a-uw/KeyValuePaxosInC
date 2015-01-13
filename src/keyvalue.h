/*
 ============================================================================
 Name        : keyvalue.h
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#ifndef _STDIO_H
  #include <stdio.h>
#endif


// GLOBAL INCLUDES STANDARD ERROR CODES USED BY THE SYSTEM.
#ifndef GLOBAL_H
  #include "global.h"
#endif

#ifndef KEYVALUE_H
  #define KEYVALUE_H
  #define KV_DEFAULT_SIZE  8

// CREATE A STRUCT FOR HOLDING KEY VALUE PAIRS
typedef struct element {
	int key;
	int value;
} element;

typedef struct kv {
	int capacity;
	int size;
	element * elements;
} kv;


/*******************************************************************************
 * CONSTRUCTS A NEW KEYVALUE OBJECT OF THE DEFAULT SIZE.  WILL RETURN NULL IF  *
 * MEMORY ALLOCATION FAILS, OTHERWISE RETURNS A POINTER TO AN INITIALIZED LIST *
 ******************************************************************************/
kv * kv_new();

/*******************************************************************************
 * HELPER FUNCTION TO EXPAND THE CAPACITY OF THE KEY VALUE STORE WHEN NEEDED,  *
 * THE CAPACITY DOUBLES IN SIZE EACH TIME, THE OLD ELEMENTS LIST IS COPIED TO  *
 * THE NEW ELEMENTS LIST AND IS DESTROYED TO FREE UP MEMORY.  WILL RETURN A    *
 * MEMORY_ALLOCATION_ERROR IF THERE IS AN ERROR CREATING THE NEW ELEMENTS LIST *
 * OTHERWISE IT WILL RETURN 0                                                  *
 ******************************************************************************/
int kv_expand(kv * the_kv);

/*******************************************************************************
 * RETURNS THE INTEGER VALUE OF THE ELEMENT REPRESENTED BY THE KEY PROVIDED.   *
 * IF THE KEY ISN'T FOUND, THE VALUE RETURNED IS NULL. BECAUSE 0 IS A VALID    *
 * RETURN VALUE, THE ADDRESS PROVIDED AS ERR IS ALSO SET TO -1.  A CALLER OF   *
 * THIS FUNCTION SHOULD CHECK THE VALUE AT THAT ADDRESS TO DETERMINE WHETHER   *
 * OR NOT THE KEY WAS FOUND.                                                   *
 ******************************************************************************/
int kv_get(kv * the_kv, int key, int * err);


/*******************************************************************************
 * PUTS THE VALUE PROVIDED INTO THE KEYVALUE STORE UNDER THE KEY PROVIDED. IF  *
 * THE KEY IS NOT UNIQUE (ALREADY EXISTS IN THE KEYVALUE STORE, THE FUNCTION   *
 * WILL RETURN -1.  OTHERWISE, THE CAPACITY WILL EXPAND IF NEEDED AND THE NEW  *
 * VALUE WILL BE STORED IN THE FIRST OPEN SLOT IN THE ELEMENTS ARRAY. WHEN A   *
 * SUCCESSFUL STORE IS COMPLETED, THE FUNCTION WILL RETURN 0.  LASTLY, A KEY   *
 * CANNOT BE -1.  IF IT IS, THE PUT FUNCTION WILL RETURN -1.                   *
 ******************************************************************************/
int kv_put(kv * the_kv, int key , int value);


/*******************************************************************************
 * A HELPFER FUNTION THAT WILL LOOK FOR THE FIRST EMPTY SLOT IN THE ELEMENTS   *
 * ARRAY AND RETURN ITS INDEX.  IF NO EMPTY SLOT IS FOUND, IT WILL RETURN -1.  *
 * WE ARE GUARANTEED AN OPEN SLOT HOWEVER, BECAUSE WHEN SIZE MEETS CAPACITY,   *
 * (NO OPEN SLOTS) THE CAPACITY DOUBLES BEFORE LOOKING FOR AN OPEN SLOT. IF AN *
 * OPEN SLOT DOES NOT EXIST, SOMETHING HAS GONE VERY WRONG.  THE METHOD FOR    *
 * FINDING THE FIRST OPEN SPOT LEVERAGES THE KV_EXISTS FUNCTION BUT RATHER IT  *
 * SEARCHES FOR -1.  THE WAY KV_EXISTS IS WRITTEN, THE FIRST OCCURRENCE OF THE *
 * KEY IS RETURNED, THUS SATISFYING THE DESIRED BEHAVIOR OF KV_FIRSTOPENSLOT   *
 ******************************************************************************/
int kv_firstOpenSlot(kv * the_kv);

/*******************************************************************************
 * LOOPS THROUGH THE ELEMENTS ARRAY IN SEARCH OF THE KEY PROVIDED.  IF FOUND,  *
 * KV_EXISTS WILL RETURN THE INDEX OF THE FIRST (AND THEORETICALLY ONLY)       *
 * OCCURENCE OF THE KEY.  IF AN OCCURENCE IS NOT FOUND, IT WILL RETURN -1      *
 ******************************************************************************/
int kv_exists(kv * the_kv, int key);

/*******************************************************************************
 * DELETES THE KEY PROVIDED FROM THE STORE, BY SIMPLY SETTING THE KEY OF THAT  *
 * OCCURRECE TO -1.  IF THE KEY IS SUCCESSFULLY REMOVED, KV_DEL WILL RETURN 0  *
 * OTHERWISE, IF THE DESIRED KEY IS NOT FOUND, IT WILL RETURN -1.              *
 ******************************************************************************/
int kv_del(kv* the_kv, int key);

/*******************************************************************************
 * KV_PRINT SIMPLY DISPLAYS THE CURRENT STATE OF THE KEYVALUE STORE TO THE     *
 * CONSOLE.  THIS INCLUDES THE CURRENT SIZE AND CAPACITY, AS WELL AS THE KEYS  *
 * AND VALUES CONTAINED IN THE ELEMENT ARRAY, INCLUDING THE EMPTY ELEMENTS.    *
 ******************************************************************************/
void kv_print(kv* the_kv);



#endif


