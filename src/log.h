
/*
 * log.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kevanderson
 */

#ifndef _STDIO_H_
  #include <stdio.h>
#endif

#ifndef _SYS_TIME_H_
#include <sys/time.h>
#endif

#ifndef LOG_H
  #define LOG_H
  #define S_LOGFILE "server.log"
#endif



/*******************************************************************************
 * WRITES A WELL FORMED MESSAGE AND RESPONSE TO THE SERVER.LOG FILE THAT WILL  *
 * INCLUDE A TIMESTAMP OF THE WRITE.  IF THE SERVER.LOG FILE DOES NOT EXIST IT *
 * WILL BE CREATED.  IF THE FILE IS UNABLE TO BE OPENED, LOG_WRITE WILL RETURN *
 * -1 OTHERWISE WILL RETURN 0.                                                 *
 ******************************************************************************/
int log_write(char * host, char * ip, int port, char * message, int is_recv);


/*******************************************************************************
 * A HELPER FUNCTION THAT RETURNS A 30 CHARACTER ARRAY REPRESENTING THE        *
 * CURRENT TIME WITH MILLISECOND PRECISION.  THE TIMESTAMP IS PROVIDED IN      *
 * LOCAL TIME.  THE FORMAT OF THE TIMESTAMP IS AS FOLLOWS:                     *
 * (TMZ) YYYY-MM-DD HH:MM:SS.SSS (e.g. (PDT) 2015-01-31 23:59:52.123           *
 * FOR MILLISECOND PRECISION, POSIX IS ASSUMED.                                *
 ******************************************************************************/
void buildTimeStamp(char * output);
