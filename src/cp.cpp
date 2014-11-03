#include "Timer.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include<string.h>
using namespace std;
int main(int argc, char **argv)
{

if(argc<3){
 cout<<"Not enough args"<<endl;
 return 1;
}

DIR *dirp;
if(!(dirp = opendir("."))){
	perror("Opendir erro");
	return 1;
}

dirent *direntp;

while((direntp = readdir(dirp))){

	if(errno!=0){
		perror("Readdir erro");
		return 1;
	}			
		
	 if(strcmp(direntp->d_name,argv[2])==0){		
			cout<<"FILE EXIST"<<endl;
		return 1;
		}
}
closedir(dirp);

   int flag = 2;
    Timer t1,t2,t3;
    double eTime,sTime,wTime;
   // t.start();
  
ofstream outfile(argv[2]);
ifstream is(argv[1]);

if(argc==4) flag = 1;
 
switch (flag){

	case 1:	
//---------------STARTING METHOD 1
	t1.start();
	 while(is.good()){
		char out = is.get();
		if(is.good()){
			outfile.put(out);
		}
	}

	  is.close();
	  t1.elapsedUserTime(eTime);
	  t1.elapsedSystemTime(sTime);
	  t1.elapsedWallclockTime(wTime);
	    cout <<"Method 1"<<endl;
	    cout <<"UserTime: "<< eTime << endl;
	    cout <<"SystemTime: "<< sTime << endl;
	    cout <<"WallclockTime: "<< wTime << endl<<endl;
	 flag = 2;
//-------------END OF M1
//-------------STARTING METHOD 2
	t2.start();
		int input2;
		int output2;

		ssize_t ret_in2;
		ssize_t ret_out2;

		char buffer2[1];

		input2 = open(argv[1], O_RDONLY);
		if(input2 == -1){
			perror("open");
			return 2;
		}

		output2 = open(argv[2], O_WRONLY | O_CREAT);
		 if(output2 == -1){
			perror("open");
			return 3;
		}

		while((ret_in2 = read (input2, &buffer2, 1))>0){
			ret_out2 = write(output2, &buffer2, (ssize_t) ret_in2);
			if(ret_out2 != ret_in2){
				perror("write");
				return 4;
				}
		}


		close(input2);
		close(output2);

		  t2.elapsedUserTime(eTime);
		  t2.elapsedSystemTime(sTime);
		  t2.elapsedWallclockTime(wTime);
		  cout <<"Method 2"<<endl;
		  cout <<"UserTime: "<< eTime << endl;
		  cout <<"SystemTime: "<< sTime << endl;
		  cout <<"WallclockTime: "<< wTime << endl<<endl;
//--------END OF M2
//--------STARTING METHOD 3
		t3.start();
		int input3;
		int output3;

		ssize_t ret_in3;
		ssize_t ret_out3;

		char buffer3[BUFSIZ];
		input3 = open(argv[1], O_RDONLY);
		if(input3 == -1){
			perror("open");
			return 1;
		}

		output3 = open(argv[2], O_WRONLY | O_CREAT);
		 if(output3 == -1){
			perror("open");
			return 1;
		}

		while((ret_in3 = read (input3, &buffer3, BUFSIZ))>0){
			ret_out3 = write(output3, &buffer3, (ssize_t) ret_in3);
			if(ret_out3 != ret_in3){
				perror("write");
				return 1;
				}
		}


		close(input3);
		close(output3);

		  t3.elapsedUserTime(eTime);
		  t3.elapsedSystemTime(sTime);
		  t3.elapsedWallclockTime(wTime);
		  cout <<"Method 3"<<endl;
		  cout <<"UserTime: "<< eTime << endl;
		  cout <<"SystemTime: "<< sTime << endl;
		  cout <<"WallclockTime: "<< wTime << endl<<endl;
//--------------- END OF M3
	break;

	case 2:
		t3.start();
		int input;
		int output;

		ssize_t ret_in;
		ssize_t ret_out;

		char buffer[BUFSIZ];
		input = open(argv[1], O_RDONLY);
		if(input == -1){
			perror("open");
			return 2;
		}

		output = open(argv[2], O_WRONLY | O_CREAT);
		 if(output == -1){
			perror("open");
			return 3;
		}

		while((ret_in = read (input, &buffer, BUFSIZ))>0){
			ret_out = write(output, &buffer, (ssize_t) ret_in);
			if(ret_out != ret_in){
				perror("write");
				return 1;
				}
		}


		close(input);
		close(output);

		  t3.elapsedUserTime(eTime);
		  t3.elapsedSystemTime(sTime);
		  t3.elapsedWallclockTime(wTime);
		  cout <<"Method 3"<<endl;
		  cout <<"UserTime: "<< eTime << endl;
		  cout <<"SystemTime: "<< sTime << endl;
		  cout <<"WallclockTime: "<< wTime << endl<<endl;
	break;
}
}
