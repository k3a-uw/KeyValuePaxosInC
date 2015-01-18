/*
 * log.c
 *
 *  Created on: Jan 11, 2015
 *      Author: kevanderson
 */

#ifndef LOG_H
#include "log.h"
#endif
/*******************************************************************************
 * WRITES A WELL FORMED MESSAGE AND RESPONSE TO THE SERVER.LOG FILE THAT WILL  *
 * INCLUDE A TIMESTAMP OF THE WRITE.  IF THE SERVER.LOG FILE DOES NOT EXIST IT *
 * WILL BE CREATED.  IF THE FILE IS UNABLE TO BE OPENED, LOG_WRITE WILL RETURN *
 * -1 OTHERWISE WILL RETURN 0.                                                 *
 ******************************************************************************/
int log_write(char* filename, char * host, char * ip, int port, char * message, int is_recv)
{
	FILE * fd = fopen(filename, "a");


	char msg[9];

	if (is_recv == 1)
	{
		strcpy(msg, "Received");
	} else {
		strcpy(msg, "Sent");
	}

	if (fd == NULL)
	{
		printf("Cannot Open File");
		return -1;
	} else {
		char timestamp[30];
		buildTimeStamp(timestamp);
		fprintf(fd, "{{Time=%s},{Host=%s},{ip=%s},{Port=%d},{%s=%s}}\n", timestamp, host, ip, port, msg, message);
		printf("{{Time=%s},{Host=%s},{ip=%s},{Port=%d},{%s=%s}}\n", timestamp, host, ip, port, msg, message);
		fclose(fd);
		return 0;
	}
}


/*******************************************************************************
 * A HELPER FUNCTION THAT RETURNS A 30 CHARACTER ARRAY REPRESENTING THE        *
 * CURRENT TIME WITH MILLISECOND PRECISION.  THE TIMESTAMP IS PROVIDED IN      *
 * LOCAL TIME.  THE FORMAT OF THE TIMESTAMP IS AS FOLLOWS:                     *
 * (TMZ) YYYY-MM-DD HH:MM:SS.SSS (e.g. (PDT) 2015-01-31 23:59:52.123           *
 * FOR MILLISECOND PRECISION, POSIX IS ASSUMED.                                *
 ******************************************************************************/
void buildTimeStamp(char * output)
{

	// TIME_T IS ONLY SECOND PRECISION, RELY ON POSIX gettimeofday FUNCTION
	struct timeval current_tv;
	gettimeofday(&current_tv, NULL);

	// TO LEVERAGE BUILT IN TIME TO STRING FUNCTIONS, CONVERT to a TIME_T
	time_t current_tm = (time_t) current_tv.tv_sec;

	// GET STRING OF TIME STAMP
	char cdate[26];
	strftime(cdate, sizeof(cdate), "(%Z) %Y-%m-%d %H:%M:%S", localtime(&current_tm));

	// GET STRING OF MILLISECONDS
	char ms[4];
	sprintf(ms, "%03d", current_tv.tv_usec / 1000);

	// PUT IT ALL TOGETHER TO RETURN
	sprintf(output, "%s.%s", cdate, ms);
	return;
}
