#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sstream>

#define COLOR_RED "\033[31m"
#define COLOR_GRN "\033[32m"
#define COLOR_WHT "\033[0m"

bool isSafeFileName(const std::string& filename) {
    for (char c : filename) {
        if (c == ';' || c == '&' || c == '|' || c == '`' || c == '$' || c == '<' || c == '>') {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Usage:\ncrun <filenames>\n";
        return 1;
    }

    std::string extra_options = "";
    std::string standard = "c99"; // Default standard
    std::string march = "native"; // Default architecture
    std::string outfilename = "crun_compiled"; // Default output filename
    std::string files = ""; // To hold input files
    
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-o") {
            if (i + 1 < argc) {
                outfilename = argv[i + 1];
                i++; // Skip next argument, its already used for output filename
            } else {
                std::cerr << COLOR_RED << "Error: " << COLOR_WHT << "Output file name missing after '-o'.\n";
                return 1;
            }
		} else if (std::string(argv[i]) == "-u") {
            system("crunu");
		} else if ((std::string(argv[i]).find('-') == 0)){
			extra_options += argv[i + 1];
            extra_options += " ";
			i++;	
        } else {
            if (!isSafeFileName(argv[i])) {
                std::cerr << COLOR_RED << "Error: " << COLOR_WHT << "Unsafe filename detected: " << argv[i] << "\n";
                return 1;
            }
            files += argv[i]; // Collect input files
            files += " ";
        }
    }

    // If no output filename is specified, use the first input file as output
    if (outfilename.empty() && !files.empty()) {
        size_t lastSlash = files.find_last_of('/');
        outfilename = (lastSlash != std::string::npos) ? files.substr(lastSlash + 1) : files;
        outfilename += ".out"; // Default extension if no filename is provided
    }

    // file path for output
    std::stringstream tmp;
    tmp << "/tmp/" << outfilename << ".crun";

    // Build the command string
    std::stringstream compileCommand;
    compileCommand << "gcc -march=" << march << " -std=" << standard << " -Wall " << extra_options <<" -o " << tmp.str() << " " << files;
    std::string runCommand = ";" + tmp.str();
    // Open a pipe to the process
    FILE* fp = popen(std::string(compileCommand.str() + runCommand).c_str(), "r");
    if (fp == nullptr) {
        std::cerr << COLOR_RED << "Error " << COLOR_WHT << " opening pipe for command: " << compileCommand.str() << std::endl;
        return 1;
    }

    // Capture and print the output of the command
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        std::cout << buffer;
    }

    // Close the pipe and check the exit status
    int status = pclose(fp);
    if (status == -1) {
        std::cerr << COLOR_RED << "Error" << COLOR_WHT << " closing pipe.\n";
        return 1;
    }

    if (WEXITSTATUS(status) == 0) {
        std::cout << "\n" << COLOR_GRN << "\033[1m" << "Child process finished successfully.\n" << COLOR_WHT;
    } else {
        std::cerr << COLOR_RED << "Error" << COLOR_WHT << " occurred during compilation. Exit code: " << WEXITSTATUS(status) << "\n";
        return 1;
    }

    return 0;
}