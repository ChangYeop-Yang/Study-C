/*
- file: spwd.c
- author: 2015118527_양창엽
- datetime: 2017-10-18 15:46)
- description: Display Directory Name and Inode.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/* Declear Function  */
ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main()
{
	printpathto(get_inode("."));
	putchar('\n');
	return 0;
}

/* Define Function  */
ino_t get_inode(char * fname) { /* Returns inode number of the file.  */
	struct stat info;
	
	/* Read File Attribute Error! */
	if (stat(fname, &info) == EOF) {
		fprintf(stderr, "Cannot Stat");
		perror(fname); 
		exit(1);	
	}
	
	printf("\x1b[31m get_inode->\x1b[0m inode: %d, name: %s\n", info.st_ino, fname);

	/* Return inode */	
	return info.st_ino;
}

void inum_to_name(ino_t inode_to_find, char * namebuf, int buflen) {
	DIR	* dir_ptr;	/* Directory */
	struct dirent * direntp;	/* Each Entry  */
	
	dir_ptr = opendir(".");
	
	/* Empty Directory  */
	if (dir_ptr == NULL) { perror("."); exit(1); }

	while( (direntp = readdir(dir_ptr)) != NULL) {
		printf("\x1b[32m inum_to_name->\x1b[0m inode: %d, name: %s\n", direntp->d_ino, direntp->d_name);

		/* Current Directory inode compare find_inode  */
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}

	fprintf(stderr, "error looking for inum %d\n", inode_to_find);
	exit(1);
}

void printpathto(ino_t this_inode) {
	ino_t	my_inode;
	char	its_name[BUFSIZ];

	if (get_inode("..") != this_inode) {
		
		printf("\x1b[34m Before chdir->\x1b[0m inode: %d\n", get_inode(".."));
		chdir("..");		/* Up one Directory  */
		inum_to_name(this_inode, its_name, BUFSIZ);
		
		my_inode = get_inode(".");
		printf("\x1b[33m printpathto->\x1b[0m inode: %d, name: %s\n", my_inode, its_name);
		printpathto(my_inode);
		printf("/%s", its_name);
	}
}
