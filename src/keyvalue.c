
/*
 ============================================================================
 Name         : keyvalue.c
 Author       : Kevin Anderson <k3a@uw.edu> & Daniel Kristiyanto <danielkr@uw.edu>
 Version      : 2015.02.1
 Modifications: Added a pthread_mutex_t to the struct and lock and unlock the
              : lock whenever something is read, or written to the struct.
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
char *substring(char *string, int position, int length);



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

	if (pthread_mutex_init(&(p_list->lock), NULL) != 0)
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

int kv_get_lock_status(kv * the_kv, int key)
{
	if (kv_exists(the_kv, key) == 1)
	{
		return(the_kv->elements[key].status);
	} else {
		return(KV_MISSING);
	}

}

int kv_set_lock_status(kv * the_kv, int key, int status)
{
	if (kv_exists(the_kv, key) == 1)
	{
		the_kv->elements[key].status = status;
		return(0);
	} else {
		return(-1);
	}
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
	pthread_mutex_lock(&(the_kv->lock));
	int new_capacity = the_kv->capacity * 2;

	element * new_elements = (element *) malloc(sizeof(element) * new_capacity);

	if (new_elements == NULL)
	{
		pthread_mutex_unlock(&(the_kv->lock));
		return MEMORY_ALLOCATION_ERROR;
	}

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
	pthread_mutex_unlock(&(the_kv->lock));

	return 0;
}

/*******************************************************************************
 * RETURNS THE INTEGER VALUE OF THE ELEMENT REPRESENTED BY THE KEY PROVIDED.   *
 * IF THE KEY ISN'T FOUND, THE VALUE RETURNED IS NULL. BECAUSE 0 IS A VALID    *
 * RETURN VALUE, THE ADDRESS PROVIDED AS ERR IS ALSO SET TO -1.  A CALLER OF   *
 * THIS FUNCTION SHOULD CHECK THE VALUE AT THAT ADDRESS TO DETERMINE WHETHER   *
 * OR NOT THE KEY WAS FOUND.                                                   *
 ******************************************************************************/
int kv_get(kv * the_kv, int key, int * value)
{
	//LOOK TO SEE IF THE KEY EXISTS AND IF SO, GET ITS INDEX
	pthread_mutex_lock(&(the_kv->lock));

	int result = kv_exists(the_kv, key);
	if (result != -1)
	{
		*value = the_kv->elements[result].value;
		pthread_mutex_unlock(&(the_kv->lock));
		return 0;
	} else {
		pthread_mutex_unlock(&(the_kv->lock));
		return -1;
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
	pthread_mutex_lock(&(the_kv->lock));


	if(key == -1) // CANNOT PLACE KEY OF -1, IT IS A SENTINAL KEY
	{
		pthread_mutex_unlock(&(the_kv->lock));
		return -1;
	}

	int location = kv_exists(the_kv, key);

	if (location == -1)
	{  //INSERT A NEW RECORD
		//CHECK SIZE AND INCREASE IF NECESSARY
		if (the_kv->size == the_kv->capacity)
			kv_expand(the_kv);

		// PUT THE VALUE IN THE FIRST OPEN SLOT AND INCREMENT
		int first_slot = kv_firstOpenSlot(the_kv);
		the_kv->elements[first_slot].key = key;
		the_kv->elements[first_slot].value = value;
		the_kv->size++;
	} else {
		// REPLACE THE OLD KEY
			the_kv->elements[location].value = value;
	}

	pthread_mutex_unlock(&(the_kv->lock));
	return 0;

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
	pthread_mutex_lock(&(the_kv->lock));

	int results = kv_exists(the_kv, key);

	if(results != -1)
	{
		the_kv->elements[results].key = -1;
		the_kv->size--;
		pthread_mutex_unlock(&(the_kv->lock));
		return 0;
	} else {
		pthread_mutex_unlock(&(the_kv->lock));
		return -1;
	}
}

/*******************************************************************************
 * PARSES MESSAGES THAT ARE SENT TO A SERVER TO DETERMINE THE COMMAND, KEY,    *
 * AND VALUES IF APPLICABLE.  THE RESULTS ARE STORED IN RESPECTIVE POINTERS    *
 * PROVIDED.  IF THE MESSAGE IS INVALID, KV_PARSER WILL RETURN -1. 0 OTHERWISE *
 ******************************************************************************/
int kv_parser(char* message, int* ret_command, int* ret_key, int* ret_value)
{
	int command;
	if (strlen(message) < 4)
		return(-1); //MALFORMED;

	command = message[0] - '0';
	if (command < 0 || command > 2)
		return(-2);  //MALFORMED

	*ret_command = command;

	if (message[1] != '|')
		return (-3); //MALFORMED


	int pipes_needed;
	if (command == 0) // it is a PUT and needs 2 more pipes
		pipes_needed = 2;
	else
		pipes_needed = 1;

	int index = 2; //start at the third character of the message
	int pipes[pipes_needed]; //locations of the pipes
	int pipes_found = 0;

	int msg_len = strlen(message);
	while (index < msg_len && pipes_found < pipes_needed)
	{

		if (!(message[index] == '|' || isdigit(message[index]))) // VALIDATE EACH CHAR
			return(-4); //MALFORMED

		if (message[index] == '|')
		{
			pipes[pipes_found] = index;
			pipes_found = pipes_found + 1;
		}

		index = index + 1;
	}

	// MAKE SURE WE HAVE ENOUGH PIPES
	if(pipes_found < pipes_needed)
		return (-5); //MALFORMED


	int key_start = 2;
	int key_length = pipes[0] - key_start;

	if (key_length < 1)
		return(-6); //MALFORMED


	char* key_str;
	key_str = substring (message, key_start+1, key_length);

	char*toss;
	*ret_key = (int) strtol(key_str, &toss, 10);

	int value_start;
	int value_length;

	if (command == 0)
	{ // NEED TO GET THE VALUE
		value_start = pipes[0]+1;
		value_length = pipes[1] - value_start;
		char* value_str = substring (message, value_start+1, value_length);
		char*toss;
		*ret_value = strtol(value_str, &toss, 10);
	} else {
		*ret_value = 0;
	}

	return(0);

}

/*********************************************
 * A SIMPLE HELPER FUNCTION TO GRAB ONLY     *
 * PARTS OF STRINGS. ACCEPTS THE STARTING    *
 * POSITION AND THE LENGTH OF THE SUBSTTRING *
 * DESIRED.                                  *
 *********************************************
 */
char *substring(char *string, int position, int length)
{
   char *pointer;
   int c;

   pointer = malloc(length+1);

   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(-1);
   }

   for (c = 0 ; c < position -1 ; c++)
      string++;

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *string;
      string++;
   }

   *(pointer+c) = '\0';

   return pointer;
}

/*******************************************************************************
 * KV_PRINT SIMPLY DISPLAYS THE CURRENT STATE OF THE KEYVALUE STORE TO THE     *
 * CONSOLE.  THIS INCLUDES THE CURRENT SIZE AND CAPACITY, AS WELL AS THE KEYS  *
 * AND VALUES CONTAINED IN THE ELEMENT ARRAY, INCLUDING THE EMPTY ELEMENTS.    *
 ******************************************************************************/
void kv_print(kv* the_kv)
{
	pthread_mutex_lock(&(the_kv->lock));
	printf("The KV currently has %d elements and %d capacity.\n",the_kv->size, the_kv->capacity);
	printf("The elements are as follows\n");
	for (int i = 0; i < the_kv->capacity; i++)
	{
		if(the_kv->elements[i].key == -1)
			printf("[%d]\tKey: Empty\n",i);
		else
			printf("[%d]\tKey: %d\tValue: %d\n", i, the_kv->elements[i].key, the_kv->elements[i].value);
	}

	pthread_mutex_unlock(&(the_kv->lock));

}
