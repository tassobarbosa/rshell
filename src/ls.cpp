#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(int argc, char **argv){

char * find_flag;
int flag=0;
string path_folder="";

for(int i=2; i<argc; i++){
	//checking how many flag are passed
	if(memcmp(argv[i],"-",1)==0){
		find_flag = strchr(argv[i],'l');
		if(find_flag != NULL){
			flag+= 1;
		}
		find_flag = strchr(argv[i],'a');
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

return 0;
}
