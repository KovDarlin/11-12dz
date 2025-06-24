#include <iostream>
#include <filesystem>
#include <string> 
#include <map>

using namespace std;
namespace fs = std::filesystem;

bool isTextFile(const fs::path& file) {
    return file.extension() == ".txt";
}

bool isImageFile(const fs::path& file) {
    return file.extension() == ".png" ||
        file.extension() == ".jpg" ||
        file.extension() == ".jpeg" ||
        file.extension() == ".gif" ||
        file.extension() == ".bmp";
}

string getFileType(const fs::path& file) {
    if (isTextFile(file)) return "Text file";
    if (isImageFile(file)) return "Image file";
    return "Other";
}

int main() {
    try {
        string dirPath;
        cout << "Enter the directory path: ";
        getline(cin, dirPath);

        fs::path directory(dirPath);

        if (!fs::exists(directory) || !fs::is_directory(directory)) {
            cerr << "The specified path is not a valid directory.\n";
            return 1;
        }
        size_t totalFiles = 0;
        uintmax_t totalSize = 0;
        map<string, int> typeCount;

        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry)) {
                totalFiles++;
                totalSize += fs::file_size(entry);
                string type = getFileType(entry.path());
                typeCount[type]++;
            }
        }

        cout << "\nDirectory report: " << directory << "\n";
        cout << "Total number of files: " << totalFiles << "\n";
        cout << "Overall size: " << totalSize << " bite\n";
        cout << "\nFile distribution by type:\n";

        for (const auto& [type, count] : typeCount) {
            cout << "- " << type << ": " << count << "\n";
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
