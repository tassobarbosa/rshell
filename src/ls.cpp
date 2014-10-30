#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

void head(char *path, char name[]){
	struct stat sb;
	struct passwd *log;
	struct passwd *grp;
	struct tm * timeinfo;
	char buffer [80];

	//create a full pathname, in case a directory was passed by parameter
	char *path_name;
	path_name =(char*) malloc(strlen(name)+1+strlen(path)+1);
	strcpy(path_name,path);
	strcat(path_name,"/");
	strcat(path_name,name);	

	if (stat(path_name, &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	    }

	free(path_name);

	log = getpwuid(sb.st_uid);
	if(log == '\0') perror("Login error");

	grp = getpwuid(sb.st_gid);
	if(grp == '\0') perror("Group error");
	
	timeinfo = localtime(&sb.st_mtime);
	strftime(buffer,80, "%b %e %H:%M",timeinfo);

	 printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
    	 printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
    	 printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
   	 printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
   	 printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
   	 printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
    	 printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
    	 printf( (sb.st_mode & S_IROTH) ? "r" : "-");
    	 printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
   	 printf( (sb.st_mode & S_IXOTH) ? "x" : "-");	
	
	cout<<" "<<sb.st_nlink<<" "
	    <<log->pw_name<<" "
	    <<grp->pw_name<<" "
	    <<sb.st_size<<" "
	    <<buffer<<" "
	    <<name<<endl;
}

void ls(char **argv, int flag, char directory[]){

	DIR *dirp;
	if(!(dirp = opendir(directory))){
		perror("Opendir erro");
		return;
	}

	dirent *direntp;
	while((direntp = readdir(dirp))){

		if(errno != 0){
			 perror ("Readdir erro");			
			return;
		}

		switch(flag){
			//ls		
			case 0:
				//avoid files starting with .
				if(direntp->d_name[0]!='.')
					cout<<direntp->d_name<<"  ";			
			break;

			//ls -a
			case 1:			
				cout<<direntp->d_name<<"  ";			
			break;		

			//ls -l
			case 2:
				//avoid files starting with .
				if(direntp->d_name[0]!='.')
					head(directory,direntp->d_name);
			break;

			//ls -al
			case 3:
				head(directory,direntp->d_name);
			break;

			//ls -R
			case 4:
			break;

			//ls -aR
			case 5:
			break;

			//ls -lR
			case 6:
			break;

			//ls -laR
			case 7:
			break;
		}

	}
	cout<<endl;
	closedir(dirp);
}

int main(int argc, char **argv){

	char * find_flag;
	int flag=0;
	string path_folder="";

	//in case first parameter is not 'ls'
	if(memcmp(argv[1],"ls",2)!=0){
		cout<<"command not valid"<<endl;
		return 0;	
	}

	for(int i=2; i<argc; i++){
		//checking how many flag are passed
		if(memcmp(argv[i],"-",1)==0){
			find_flag = strchr(argv[i],'a');
			if(find_flag != NULL){
				flag+= 1;
			}
			find_flag = strchr(argv[i],'l');
			if(find_flag != NULL){
				flag+=2;
			}
			find_flag = strchr(argv[i],'R');
			if(find_flag != NULL){
				flag+=4;
			}
		}else{		
			//recieving folder as argument
			path_folder = argv[i];	
		}
	}
	//if no folder has been passed as argument
	if(path_folder=="") path_folder = get_current_dir_name();

	char * directory = new char[path_folder.length()+1];
	strcpy(directory, path_folder.c_str());

	ls(argv, flag, directory);

	delete[] directory;

	return 0;
}
