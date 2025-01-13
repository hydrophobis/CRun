#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>

int main(int argc, char* argv[]){
	char tmp[106] = "/tmp/";
	char fileExtension[6] = ".crun";
	char* inFile = argv[1];
	std::strcat(tmp, inFile);
	std::strcat(tmp, fileExtension);
	std::strcat(tmp, " ");
	std::strcat(tmp, inFile);
	
	for(int i = 1; i < argc; i++){
		
	}

	char compileCommand[100] = "gcc -o ";

	std::strcat(compileCommand, tmp);
	char buffer[121] = "";
	std::strcat(buffer, compileCommand);			
	system(buffer);
	std::cout << "Starting child process" << std::endl;
	system(tmp);
	std::cout << std::endl << "Child process finished." << std::endl;
	return 0;
}
