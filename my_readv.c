
#define _FILE_OFFSET_BITS 64


#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
/*#include "/lib/tlpi_hdr.h"*/
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>



#ifndef BUF_SIZE
#define BUF_SIZE 1
#endif

/*
 * Implement readv and writev using read and write and malloc 
 * */

char **my_readv(int fd)
{


	ssize_t numRead;
	int lineLength, lineCounter;
	char **vectorArray;
	char buf[1];
	int lineAggregator;



	//fd=open("file", openFlags, filePerms);
	if(fd == -1)
	{
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	lineLength = 0;
	lineCounter = 0;

	/* Let's first count the number of lines to malloc
	 * */
	while((numRead = read(fd, buf, BUF_SIZE)) > 0)
	{
		//printf("%c\n", *buf);
		if(*buf == '\n')
		{
			lineCounter++;
		}

	}
	//printf("%d\n", lineCounter);

	/*  malloc the lines */
	vectorArray = malloc(lineCounter * sizeof(char *));
	if(vectorArray == NULL)
	{
		printf("%s\n", strerror(errno));
		printf("malloc failure\n");
		exit(EXIT_FAILURE);
	}



	/* rewind the file since we now know the linecounts*/
	lseek(fd, 0, SEEK_SET);

	/* Reset lineCounter, we will need this later*/
	lineCounter = 0;

	lineAggregator = 0;
	while(read(fd, buf, BUF_SIZE) > 0)
	{
		//printf("%c\n", *buf);
		lineLength++;
		if(*buf == '\n')
		{
			vectorArray[lineCounter] = calloc((lineLength + 1), sizeof(char));
			if(vectorArray[lineCounter] == NULL)
			{
				printf("calloc error\n");
				printf("%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			lseek(fd, lineAggregator, SEEK_SET);
			read(fd, vectorArray[lineCounter], lineLength);
			lineAggregator = lineAggregator+lineLength;
			lineLength = 0;
			//printf("%s", vectorArray[lineCounter]);
			lineCounter++;
		}
	}

	return(vectorArray);


	/*
	 *The free'ing is for checking through valgrind
	 *This is a function so we have a return value
	 * We return the vectorArray
	 *
	 for(int i = 0; i < lineCounter; i++)
	 {
	 free(vectorArray[i]);
	 }
	 free(vectorArray);
	 */

	/* 
	   exit(EXIT_SUCCESS);
	   */


}

/*
 * Main function to check that it all works and that valgrind runs through
*/

int main(void)
{
	int fd, openFlags;
	mode_t filePerms; 	
	openFlags = O_RDWR | O_CREAT | O_APPEND;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	fd = open("file", openFlags,filePerms);
	int i;

	char **myvec;
	myvec = my_readv(fd);

	for(i = 0; i < 3; i++)
	{
		printf("%s", myvec[i]);
	}
	
	
	
	
	for(i = 0; i < 3; i++)
	{
		free(myvec[i]);
	}
	free(myvec);

	
	
}

