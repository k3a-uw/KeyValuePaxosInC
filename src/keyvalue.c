
/*
 ============================================================================
 Name        : keyvalue.c
 Author      : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version     : 2015.01.10
 Description : TCSS558 Project 1
 ============================================================================
 */

#ifndef KEYVALUE_H
  #include "keyvalue.h"
#endif


/*******************************************************************************
 * WRITES A WELL FORMED MESSAGE AND RESPONSE TO THE SERVER.LOG FILE THAT WILL  *
 * INCLUDE A TIMESTAMP OF THE WRITE.  IF THE SERVER.LOG FILE DOES NOT EXIST IT *
 * WILL BE CREATED.  IF THE FILE IS UNABLE TO BE OPENED, LOG_WRITE WILL RETURN *
 * -1 OTHERWISE WILL RETURN 0.                                                 *
 ******************************************************************************/
int log_write(char * host, int port, char * message, char * response);



/*******************************************************************************
 * CONSTRUCTS A NEW KEYVALUE OBJECT OF THE DEFAULT SIZE.  WILL RETURN NULL IF  *
 * MEMORY ALLOCATION FAILS, OTHERWISE RETURNS A POINTER TO AN INITIALIZED LIST *
 ******************************************************************************/
kv * kv_new()
{
	// ALLOCATE MEMORY FOR THE STRUCT
	kv * p_list = (kv *) malloc(sizeof(kv));

	if (p_list == NULL)
		return NULL;

	// INITIALIZE STRUCT VALUES
	p_list->capacity = KV_DEFAULT_SIZE;
	p_list->size = 0;

	//CREATE AN ARRAY OF 10 ELEMENTS
	p_list->elements = (element *) malloc(sizeof(element) * KV_DEFAULT_SIZE);

	if (p_list->elements == NULL)
		return NULL;

	// INITIALIZE THE KEYS;
	for(int i = 0; i < p_list->capacity; i++)
	{
		p_list->elements[i].key = -1;
	}


	return p_list;

}

/*******************************************************************************
 * HELPER FUNCTION TO EXPAND THE CAPACITY OF THE KEY VALUE STORE WHEN NEEDED,  *
 * THE CAPACITY DOUBLES IN SIZE EACH TIME, THE OLD ELEMENTS LIST IS COPIED TO  *
 * THE NEW ELEMENTS LIST AND IS DESTROYED TO FREE UP MEMORY.  WILL RETURN A    *
 * MEMORY_ALLOCATION_ERROR IF THERE IS AN ERROR CREATING THE NEW ELEMENTS LIST *
 * OTHERWISE IT WILL RETURN 0                                                  *
 ******************************************************************************/
int kv_expand(kv * the_kv)
{
	int new_capacity = the_kv->capacity * 2;

	element * new_elements = (element *) malloc(sizeof(element) * new_capacity);

	if (new_elements == NULL)
		return MEMORY_ALLOCATION_ERROR;

	// INITIALIZE new_elements;
	for(int i = 0; i < new_capacity; i++)
	{
		new_elements[i].key = -1;
	}


	int c = 0;  // a counter for new array

	//COPY EXISTING DATA INTO NEW ARRAY
	for(int i = 0; i < the_kv->capacity; i++)
	{
		if(the_kv->elements[i].key != -1)
		{
			new_elements[c].key   = the_kv->elements[i].key;
			new_elements[c].value = the_kv->elements[i].value;
			c++;
		}
	}

	//REPLACE THE OLD LIST WITH THE NEW LIST
	free(the_kv->elements);
	the_kv->capacity = new_capacity;
	the_kv->elements = new_elements;

	return 0;
}

/*******************************************************************************
 * RETURNS THE INTEGER VALUE OF THE ELEMENT REPRESENTED BY THE KEY PROVIDED.   *
 * IF THE KEY ISN'T FOUND, THE VALUE RETURNED IS NULL. BECAUSE 0 IS A VALID    *
 * RETURN VALUE, THE ADDRESS PROVIDED AS ERR IS ALSO SET TO -1.  A CALLER OF   *
 * THIS FUNCTION SHOULD CHECK THE VALUE AT THAT ADDRESS TO DETERMINE WHETHER   *
 * OR NOT THE KEY WAS FOUND.                                                   *
 ******************************************************************************/
int kv_get(kv * the_kv, int key, int * err)
{
	//LOOK TO SEE IF THE KEY EXISTS AND IF SO, GET ITS INDEX
	int result = kv_exists(the_kv, key);
	if (result != -1)
	{
		*err = 0;
		return the_kv->elements[result].value;
	} else {
		*err = -1;
		return NULL;
	}
}

/*******************************************************************************
 * PUTS THE VALUE PROVIDED INTO THE KEYVALUE STORE UNDER THE KEY PROVIDED. IF  *
 * THE KEY IS NOT UNIQUE (ALREADY EXISTS IN THE KEYVALUE STORE, THE FUNCTION   *
 * WILL RETURN -1.  OTHERWISE, THE CAPACITY WILL EXPAND IF NEEDED AND THE NEW  *
 * VALUE WILL BE STORED IN THE FIRST OPEN SLOT IN THE ELEMENTS ARRAY. WHEN A   *
 * SUCCESSFUL STORE IS COMPLETED, THE FUNCTION WILL RETURN 0.  LASTLY, A KEY   *
 * CANNOT BE -1.  IF IT IS, THE PUT FUNCTION WILL RETURN -1.                   *
 ******************************************************************************/
int kv_put(kv * the_kv, int key , int value)
{

	if(key == -1 || kv_exists(the_kv, key) != -1)
	{
		return -1;
	}
	else
	{
		//CHECK SIZE AND INCREASE IF NECESSARY
		if (the_kv->size == the_kv->capacity)
			kv_expand(the_kv);

		// PUT THE VALUE IN THE FIRST OPEN SLOT AND INCREMENT
		int first_slot = kv_firstOpenSlot(the_kv);
		the_kv->elements[first_slot].key = key;
		the_kv->elements[first_slot].value = value;
		the_kv->size++;
		return 0;
	}

}

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
int kv_firstOpenSlot(kv * the_kv)
{
	return kv_exists(the_kv, -1);
}

/*******************************************************************************
 * LOOPS THROUGH THE ELEMENTS ARRAY IN SEARCH OF THE KEY PROVIDED.  IF FOUND,  *
 * KV_EXISTS WILL RETURN THE INDEX OF THE FIRST (AND THEORETICALLY ONLY)       *
 * OCCURENCE OF THE KEY.  IF AN OCCURENCE IS NOT FOUND, IT WILL RETURN -1      *
 ******************************************************************************/
int kv_exists(kv * the_kv, int key)
{
	for(int i = 0; i < the_kv->capacity; i++)
	{
		if (the_kv->elements[i].key == key)
		{
			return i;
		}
	}

	return -1;
}


/*******************************************************************************
 * DELETES THE KEY PROVIDED FROM THE STORE, BY SIMPLY SETTING THE KEY OF THAT  *
 * OCCURRECE TO -1.  IF THE KEY IS SUCCESSFULLY REMOVED, KV_DEL WILL RETURN 0  *
 * OTHERWISE, IF THE DESIRED KEY IS NOT FOUND, IT WILL RETURN -1.              *
 ******************************************************************************/
int kv_del(kv* the_kv, int key)
{
	int results = kv_exists(the_kv, key);

	if(results != -1)
	{
		the_kv->elements[results].key = -1;
		the_kv->size--;
		return 0;
	} else {
		return -1;
	}
}


/*******************************************************************************
 * KV_PRINT SIMPLY DISPLAYS THE CURRENT STATE OF THE KEYVALUE STORE TO THE     *
 * CONSOLE.  THIS INCLUDES THE CURRENT SIZE AND CAPACITY, AS WELL AS THE KEYS  *
 * AND VALUES CONTAINED IN THE ELEMENT ARRAY, INCLUDING THE EMPTY ELEMENTS.    *
 ******************************************************************************/
void kv_print(kv* the_kv)
{
	printf("The KV currently has %d elements and %d capacity.\n",the_kv->size, the_kv->capacity);
	printf("The elements are as follows\n");
	for (int i = 0; i < the_kv->capacity; i++)
	{
		if(the_kv->elements[i].key == -1)
			printf("[%d]\tKey: Empty\n",i);
		else
			printf("[%d]\tKey: %d\tValue: %d\n", i, the_kv->elements[i].key, the_kv->elements[i].value);
	}
}
