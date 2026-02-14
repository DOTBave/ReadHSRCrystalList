#include <iostream>
#include <fstream>  // For file input
#include <string>
#include <vector>
#include <algorithm>

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

    // find starting pos, extract the data that's to the right of it
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
    std::ofstream outputFileEXCEL("outputEXCEL.csv");

    if (!outputFileVALUE || !outputFileDATE || !outputFileTYPE || !outputFileEXCEL) {
        std::cerr << "Failed to open one of the output files.\n";
        return 1;
    }
    if (valuesVALUE.size() != valuesDATE.size() || valuesVALUE.size() != valuesTYPE.size() || valuesDATE.size() != valuesTYPE.size()) {
        std::cerr << "Warning, length of three files don't match, data may misallign." << std::endl;
    }
    // Force the compiler to build this string as UTF-8
    outputFileEXCEL << "\xEF\xBB\xBF";
    outputFileEXCEL << (const char*)u8"星穹,日期,获取方式" << '\n';
    for (size_t i = 0; i < std::min({ valuesVALUE.size(), valuesDATE.size(), valuesTYPE.size() }); i++) {
        std::cout << "Extracted: " << valuesVALUE[i] << '\n';
        std::cout << "Extracted: " << valuesDATE[i] << '\n';
        std::cout << "Extracted: " << valuesTYPE[i] << '\n';
        outputFileVALUE << valuesVALUE[i] << '\n';
        outputFileDATE << valuesDATE[i] << '\n';
        outputFileTYPE << valuesTYPE[i] << '\n';
        outputFileEXCEL << valuesVALUE[i] << ',' << valuesDATE[i] << ',' << valuesTYPE[i] << '\n';
    }
    return 0;
}
