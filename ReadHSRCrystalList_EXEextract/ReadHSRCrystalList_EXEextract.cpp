#include <iostream>
#include <fstream>  // For file input
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    // Check for proper usage
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string inputFileName = argv[1];

    // Open the file
    std::ifstream file(inputFileName);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << inputFileName << "\n";
        return 1;  // Exit if the file can't be opened
    }

    // Read the entire file into a string
    std::string html((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // The target string to find
    const std::string targetVALUE = "class=\"record-item__content-right__value\">";
    const std::string targetDATE = "class=\"record-item__content-right__date\">";
    const std::string targetTYPE = "class=\"record-item__content-left__text\">";
    std::vector<std::string> valuesVALUE, valuesDATE, valuesTYPE;

    size_t pos = 0;
    while ((pos = html.find(targetVALUE, pos)) != std::string::npos) {
        size_t start = pos + targetVALUE.length();
        size_t end = html.find("<", start);
        if (end != std::string::npos) {
            valuesVALUE.push_back(html.substr(start, end - start));
            pos = end;
        }
        else break;
    }

    pos = 0;
    while ((pos = html.find(targetDATE, pos)) != std::string::npos) {
        size_t start = pos + targetDATE.length();
        size_t end = html.find("<", start);
        if (end != std::string::npos) {
            valuesDATE.push_back(html.substr(start, end - start));
            pos = end;
        }
        else break;
    }

    pos = 0;
    while ((pos = html.find(targetTYPE, pos)) != std::string::npos) {
        size_t start = pos + targetTYPE.length();
        size_t end = html.find("<", start);
        if (end != std::string::npos) {
            valuesTYPE.push_back(html.substr(start, end - start));
            pos = end;
        }
        else break;
    }

    std::ofstream outputFileVALUE("outputVALUE.txt");
    std::ofstream outputFileDATE("outputDATE.txt");
    std::ofstream outputFileTYPE("outputTYPE.txt");

    if (!outputFileVALUE || !outputFileDATE || !outputFileTYPE) {
        std::cerr << "Failed to open one of the output files.\n";
        return 1;
    }

    for (const auto& val : valuesVALUE) {
        std::cout << "Extracted: " << val << '\n';
        outputFileVALUE << val << '\n';
    }
    for (const auto& val : valuesDATE) {
        std::cout << "Extracted: " << val << '\n';
        outputFileDATE << val << '\n';
    }
    for (const auto& val : valuesTYPE) {
        std::cout << "Extracted: " << val << '\n';
        outputFileTYPE << val << '\n';
    }

    return 0;
}
