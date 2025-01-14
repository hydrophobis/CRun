#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>
#include <string>

int main(int argc, char* argv[]){
	std::string outfilename = "";
	for(int i = 0; i < argc; i++){
		if(argv[i] == "-o"){
			outfilename = argv[i + 1];
			i += 1;
		}
	}
	if(outfilename == ""){
		outfilename = argv[1];
	}

	char tmp[106] = "/tmp/";
	char fileExtension[6] = ".crun";
	char** inFile = argv;
	std::string files = "";
	for(int i = 0; i < sizeof(inFile)/8; i++){
		std::string file = "";
		for(int j = 0; inFile[i + 1][j] != '\0'; ++j){
			if(inFile[i + 1][j] != ';'){
				file += inFile[i + 1][j];
			}
		}
		files = files + file + " ";
	}
	std::strcat(tmp, outfilename.c_str());
	std::strcat(tmp, fileExtension);
	std::strcat(tmp, " ");
	std::strcat(tmp, files.c_str());

	char compileCommand[100] = "gcc -march=native -std=c99 -Wall -o ";

	std::strcat(compileCommand, tmp);
	std::cout << "Starting child process" << std::endl;
	system(tmp);
	std::cout << std::endl << "Child process finished." << std::endl;
	return 0;
}
