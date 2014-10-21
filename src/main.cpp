#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//output login @ machine $
void prompt(){
	int pid = fork();
		if(pid == -1){
			perror("fork() presented error");
			exit(1);
			}
		else if(pid==0){
			char host[50];
			if ((gethostname(host, sizeof(host)-1))==-1) {
				host[0] = 'h';
				host[1] = 'o';
				host[2] = 's';
				host[3] = 't';
				host[4] = '\0';
				perror("Error trying to get hostname");
			}

			char login[50];
	
			if (getlogin_r(login, sizeof(login)-1)) {
				login[0] = 'l';
				login[1] = 'o';
				login[2] = 'g';
				login[3] = 'i';
				login[4] = 'n';
				login[5] = '\0';
				perror("Error trying to get user login");
			}

			cout << login << "@" << host << "$ ";
			exit(1);
			}else if(pid>0){
				if(-1 == wait(0))
			   	perror("wait() presented error");
		
			}			 
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
//for(int i=0;i<size;i++) cout<<str[i]<<endl;

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
//	for(int g=0;g<aux+2;g++)cout<<"indice "<<g<<" "<<newstr[g]<<endl;
	
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
	    prompt(); //output login @ machine $
	    cin.getline(input,256);
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
