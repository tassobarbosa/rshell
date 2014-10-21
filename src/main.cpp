#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pwd.h>
using namespace std;

static int *glob_flag;

//output login @ machine $
void prompt(){
struct passwd *log;
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
			log = getpwuid(getuid());			
			if(log == '\0'){
				perror("Error trying to get user login");
			}

			cout << log->pw_name << "@" << host << "$ ";
			exit(1);
			}else if(pid>0){
				if(-1 == wait(0))
			   	perror("wait() presented error");
		
			}			 
}

void execute(char *str[],int size){

char * newstr[256];
char * connector;
int i,j,aux;

	//create a shared memory to be accessed from child and process
	glob_flag =(int*) mmap(NULL, sizeof *glob_flag, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	for(i=0;i<size;i++){
		*glob_flag = 0;
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
				//check for && setting a flag to 1
				if(memcmp(str[j], "&&",2)==0){
					*glob_flag = 1;	
					i = j;
					break;				
				}
				//check for || setting a flag to 3
				if(memcmp(str[j], "||",2)==0){
					*glob_flag = 3;	
					i = j;
					break;				
				}

				//add flags to newstr
				newstr[aux] = str[j];
				aux++;
				i = j;
	
				if(connector!=NULL) break;
			}
	
		int pid = fork();
		if(pid == -1){
			perror("fork() presented error");
			exit(1);
		}
		else if(pid == 0){					
		  	 if(-1 == execvp(newstr[0], newstr)){	
				perror("There was an error");
				//flag 1 means, first command must be successfull to apply the second
				if(*glob_flag == 1) *glob_flag = 2;
				//flag 3 means, first command must be failed to apply the second
				//number 5 won't pass in the breaking loop
				if(*glob_flag == 3) *glob_flag = 5;
			}					
    	 		exit(1);
		}
		else if(pid>0){
			if(-1 == wait(0))
		   	perror("wait() presented error");
		
		}
	
		// clear the vector newstr in order to execute new commands
		for(int k=0;k<aux;k++)newstr[k]='\0';

		//break loop due to a non valid previous command connected by &&					
		if(*glob_flag == 2) break;
		//break loop due to a valid previous command connected by ||
		if(*glob_flag == 3) break;
	}
}




int main(){
   int index;
   string line;
   char * str[256];
   char * pch;

   while(true){
	do{
	    //output login @ machine $
	    prompt();
	    getline(cin,line);
	}while(line[0]=='#');

	//look for '#', if find erase everything until the end of line
	size_t found = line.find('#');
	  if (found!=std::string::npos)
		line.erase (found,line.length()-found);

	//create a dynamic char that is a copy of input
  	char * input = new char [line.length()+1];
  	strcpy (input, line.c_str());

	//built in function to finish program when typed 'EXIT'
	if(memcmp(input, "exit",4)==0) exit(0);


	index=0;
	pch= strtok (input," ");

	while(pch != NULL){
	    str[index]=pch;
	    pch = strtok(NULL, " ");
	    index++;
	}

	str[index] = NULL;

	execute(str,index);

    }
 return 0;
}
