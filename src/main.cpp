#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void prompt(){
	cout<<"$ ";
}

//clear the line from the first # to the end of the input
void comment(char line[],int linesize){
	char * hashtag;
	hashtag = strchr(line,'#');
	if(hashtag!=NULL){		
		 memset(&line[hashtag-line],0,linesize);			
	}
}
void execute(char *str[],int size){
//for(int i=0;i<sizeof(str);i++) cout<<str[i]<<endl;

char * newstr[256];
char * connector;
int i,j,aux;

	for(i=0;i<size;i++){
		//newstr[0] receives commands, first parameter after a connector
		newstr[0] = str[i];
		aux=1;
		//test command without flags carryng a ';'
		connector = strchr(newstr[0],';');
		if(connector!=NULL) newstr[0][connector-newstr[0]] = '\0';
		else
			for(j=i+1;j<size;j++){
				connector = strchr(str[j],';');
				if(connector!=NULL){
				//erase the last character if ';'
					str[j][connector-str[j]] = '\0';
				}

				//add flags to newstr
				newstr[aux] = str[j];
				aux++;
				i = j;
	
				if(connector!=NULL) break;
			}

	//	for(int k=0;k<aux;k++)cout<<"indice "<<k<<" "<<newstr[k]<<endl;
	
		int pid = fork();
		if(pid == -1){
			perror("There was an error with fork().");
			exit(1);
		}
		else if(pid == 0){	
		  	 if(-1 == execvp(newstr[0], newstr)){
				perror("There was an error");
			}		
    	 		 exit(1);
		}
		else if(pid>0){
			if(-1 == wait(0))
		   	perror("There was an error with wait()");
		
		}
	
		// clear the vector newstr in order to execute new commands
		for(int k=0;k<aux;k++)newstr[k]='\0';
	}
}



int main(){
   int INPUTSIZE=256,index;
   char input[256];
   char * str[256];
   char * pch;

   while(true){
	do{
	    //output login@machine$
	    prompt();
	    cin.getline(input,INPUTSIZE);
	}while(input[0]=='#');

	comment(input,INPUTSIZE);

	if(memcmp(input, "exit",4)==0) exit(0);


	index=0;
	pch= strtok (input," ");

	while(pch != NULL){
	    str[index]=pch;
	    pch = strtok(NULL, " ");
	    index++;
	}

	str[index] = NULL;
	//for(int i=0;i<sizeof(str);i++) cout<<str[i]<<endl;

	execute(str,index);

    }
 return 0;
}
