#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
   	char str[30];
   	char *ptr;
   	long inputNum;
   	int guesses = 0;
   	int toGuess;
   	int cont = 1;

   	srand(time(NULL));
   	
   	toGuess = rand() % 100;

   	printf("=======================\n");

   	
   	printf("Auf geht's!\n");

   	while(cont){

   		scanf("%s", str);
   		
   		if (strcmp(str,"quit") == 0){
   			exit(0);
   		}

   		inputNum = strtol(str, &ptr, 10);
   		
   		if(inputNum == toGuess){
   			printf("great success!!\n");
   			printf("Versuche: %i\n", guesses);
   			printf("noch eine Runde? (j/n)\n");
   			scanf("%s", str);
   			if (strcmp(str, "n") == 0){
   				cont = 0;
   			}else{
   				guesses = 0;
   				toGuess = rand() % 100;
   				printf("Neue Runde, neues Glück!\n");
   			}
   		}else if( inputNum < toGuess ){
   			printf("Zu erratende Zahl ist größer.\n");
   			guesses++;
   		}else{
   			printf("Zu erratende Zahl ist kleiner.\n");
   			guesses++;
   		}

	}
   	
   	return(0);
}