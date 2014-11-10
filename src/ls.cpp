#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

void getpath(char *pathname, char *path, char name[]){
	strcpy(pathname, path);
	strcat(pathname, "/");
	strcat(pathname, name);
}

void changeColor(int background, struct stat fileStat){
	if(background == 1) cout<<"\x1b[47m";

	if(S_ISDIR(fileStat.st_mode))
		cout<<"\x1b[34m";
	else if(fileStat.st_mode & S_IXUSR)
		cout<<"\x1b[32m";
}

void resetColor(){
	cout<<"\x1b[0m";
}

void head(char path[], char name[], struct stat sb, int background){
	struct passwd *log;
	struct group *grp;
	struct tm * timeinfo;
	char buffer [80];

	log = getpwuid(sb.st_uid);
	if(log == '\0') perror("Login error");

	grp = getgrgid(sb.st_gid);
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
	    <<grp->gr_name<<" "
	    <<sb.st_size<<" "
	    <<buffer<<" ";
	    changeColor(background,sb);
	    cout<<name;
	    resetColor();
	    cout<<endl;
}

void ls(int flag, char directory[]){
	bool showFiles = false;
	bool showTotal = false;
	int total=0;
	DIR *dirp;

	if(!(dirp = opendir(directory))){
		perror("Opendir erro");
		return;
	}

	dirent *direntp;
	while((direntp = readdir(dirp))){

		if(errno != 0){
			 perror ("Readdir erro");					
		}

		char fullpath[512];

		getpath(fullpath, directory, direntp->d_name);

		struct stat fileStat;
		if(stat(fullpath, &fileStat) == -1){
			perror("Stat error");
			exit(1);	
		}

		switch(flag){
			//ls		
			case 0:
				//avoid files starting with .
				if(direntp->d_name[0]!='.'){
					changeColor(0,fileStat);
					cout<<direntp->d_name<<"  ";	
					resetColor();
				}
			break;

			//ls -a
			case 1:			
				if(direntp->d_name[0]=='.')
					changeColor(1,fileStat);
				else
					changeColor(0,fileStat);

				cout<<direntp->d_name;
				resetColor();
				cout<<"  ";
			break;		

			//ls -l
			case 2:
				if(!showTotal){
					ls(8,directory);
					showTotal = true;
				}
				//avoid files starting with .
				if(direntp->d_name[0]!='.')
					head(fullpath,direntp->d_name, fileStat,0);
			break;

			//ls -al
			case 3:	
				if(!showTotal){
					ls(9,directory);
					showTotal = true;
				}
				if(direntp->d_name[0]=='.')
					head(fullpath, direntp->d_name,fileStat,1);
				else
					head(fullpath, direntp->d_name,fileStat,0);
			break;

			//ls -R
			case 4:
				//has this directory shown its files?
				if(!showFiles){
					showFiles = true;
					//show actual directory
					cout<<directory<<":"<<endl;
					//display its files
                                        ls(0, directory);
                                        cout<<endl;	
				}
				//after showing the files, apply -R function
				//calling ls() to display the son directory
				if(direntp->d_type==DT_DIR){	
					if(direntp->d_name[0]!='.'){
                                                cout<<endl;					
						//entry a level and call ls again
						ls(flag,fullpath);
                                        }										
				}
			break;

			//ls -aR
			case 5:
				//has this directory shown its files?
				if(!showFiles){
					showFiles = true;
					//show actual directory
					cout<<directory<<":"<<endl;
					//display its files
                                        ls(1, directory);
                                        cout<<endl<<endl;	
				}
				//after showing the files, apply -R function
				//calling ls() to display the son directory
				if(direntp->d_type==DT_DIR){	
					if((direntp->d_name[0]=='.'&& direntp->d_name[1]=='\0') || (direntp->d_name[0]=='.' && direntp->d_name[1]=='.' && direntp->d_name[2]=='\0'));
                                        else{					
						//entry a level and call ls again
						ls(flag,fullpath);	
					}					
				}
			break;

			//ls -lR
			case 6:	
			//has this directory shown its files?
				if(!showFiles){
					showFiles = true;
					//show actual directory
					cout<<directory<<":"<<endl;
					//display its files
                                        ls( 2, directory);
                                        cout<<endl;	
				}
				//after showing the files, apply -R function
				//calling ls() to display the son directory
				if(direntp->d_type==DT_DIR){	
					if(direntp->d_name[0]!='.'){					
						//entry a level and call ls again
						ls(flag,fullpath);	
					}					
				}
			break;

			//ls -laR
			case 7:
			//has this directory shown its files?
				if(!showFiles){
					showFiles = true;
					//show actual directory
					cout<<directory<<":"<<endl;
					//display its files
                                        ls(3, directory);
                                        cout<<endl;	
				}
				//after showing the files, apply -R function
				//calling ls() to display the son directory
				if(direntp->d_type==DT_DIR){	
					if((direntp->d_name[0]=='.'&& direntp->d_name[1]=='\0') || (direntp->d_name[0]=='.' && direntp->d_name[1]=='.' && direntp->d_name[2]=='\0'));
                                        else{					
						//entry a level and call ls again
						ls(flag,fullpath);	
					}					
				}
			break;
			//case 8 and 9 only calculates nd print total in case of flag -l
			case 8:				
				if(direntp->d_name[0]!='.')	
					total+=fileStat.st_blocks;
			break;
			case 9:				
					total+=fileStat.st_blocks;	
			break;
		}

	}
	closedir(dirp);
	if(flag==8 || flag == 9) cout<<"total "<<total/2<<endl;
}

int main(int argc, char **argv){

	char * find_flag;
	int flag=0;
	string path_folder="";	

	for(int i=1; i<argc; i++){
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

	ls(flag, directory);
        if(flag == 0 || flag ==1) cout<<endl;
	delete[] directory;

	return 0;
}
