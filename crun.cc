#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>

int runProcess(const char* ps){
	system(ps);
}

int main(int argc, char* argv[]){
	char tmp[50] = "/tmp/";
	char fileExtension[6] = ".crun";
	char inFile[9] = "source.c";
	std::strcat(tmp, inFile);
	std::strcat(tmp, fileExtension);
	std::strcat(tmp, " ");

	char compileCommand[100] = "gcc -o ";

	std::strcat(compileCommand, tmp);
	char buffer[121] = "cd ~; cd /; ";
	std::strcat(buffer, compileCommand);	

	std::cout << "Commands ready" << std::endl;
		
	std::cout << "Switching directories" << std::endl;
	system(buffer);
	std::cout << "C file compiled into /tmp successfully" << std::endl;
	std::cout << "Starting child process" << std::endl;
	runProcess(tmp);
	std::cout << std::endl << "Child process finished." << std::endl;
	return 0;
}
