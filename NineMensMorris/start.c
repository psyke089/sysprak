#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {// Kommandozeilenparameter einlesen
 	 char *id, *to;
 	 id = NULL;
 	 to = NULL;
	int ret;

	while ((ret=getopt(argc, argv, "i:a:")) != -1) {
		switch (ret) {
	      case 'i':
	        id = optarg;
	       	printf("Ihre id ist: %s\n",optarg);
	         break;
	      case 'a':
	         to = optarg;
	         break;
	      default:
     	   //unter 2
	      	printf("Defaulllt");
  		}
  	}
}