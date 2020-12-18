

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
#include "my_readv.h"



#ifndef BUF_SIZE
#define BUF_SIZE 1
#endif

/*
 * Implement readv and writev using read and write and malloc 
 * */

int main(void)
{
	int fd, openFlags;
	mode_t filePerms;
	openFlags = O_RDWR | O_CREAT | O_APPEND;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	char buf[1];




	fd = open("writehere", openFlags, filePerms);
	if(fd == -1)
	{
		printf("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while((numRead=read(**vectorArray, buf, BUF_SIZE) > 0))
	{
		if(write(fd, buf, numRead) == -1)
		{
			printf("%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

	}	

	exit(EXIT_SUCCESS);
}


