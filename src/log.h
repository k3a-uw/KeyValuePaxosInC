
/*
 * log.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kevanderson
 */

#ifndef LOG_H
 #define LOG_H


#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _SYS_TIME_H_
#include <sys/time.h>
#endif

#ifndef _TIME_H_
#include <time.h>
#endif



/*******************************************************************************
 * WRITES A WELL FORMED MESSAGE AND RESPONSE TO THE SERVER.LOG FILE THAT WILL  *
 * INCLUDE A TIMESTAMP OF THE WRITE.  IF THE SERVER.LOG FILE DOES NOT EXIST IT *
 * WILL BE CREATED.  IF THE FILE IS UNABLE TO BE OPENED, LOG_WRITE WILL RETURN *
 * -1 OTHERWISE WILL RETURN 0.                                                 *
 ******************************************************************************/
int log_write(char* filename, char * host, char * message);


/*******************************************************************************
 * A HELPER FUNCTION THAT RETURNS A 30 CHARACTER ARRAY REPRESENTING THE        *
 * CURRENT TIME WITH MILLISECOND PRECISION.  THE TIMESTAMP IS PROVIDED IN      *
 * LOCAL TIME.  THE FORMAT OF THE TIMESTAMP IS AS FOLLOWS:                     *
 * (TMZ) YYYY-MM-DD HH:MM:SS.SSS (e.g. (PDT) 2015-01-31 23:59:52.123           *
 * FOR MILLISECOND PRECISION, POSIX IS ASSUMED.                                *
 ******************************************************************************/
void buildTimeStamp(char * output);


#endif
