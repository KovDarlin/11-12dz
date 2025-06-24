#include <iostream>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

string generateRandomName(size_t length = 8) {
    const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string name;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, chars.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        name += chars[dist(gen)];
    }
    return name + ".txt";
}

string generateRandomContent(size_t length = 50) {
    const string chars = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    string content;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, chars.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        content += chars[dist(gen)];
    }
    return content;
}

int main() {
    try {
        string dirName;
        cout << "Enter the directory name: ";
        getline(cin, dirName);

        fs::path mainDir(".\\" + dirName);
        fs::path backupDir = mainDir / "backup";

        if (!fs::exists(mainDir)) {
            fs::create_directory(mainDir);
            cout << "The directory is created: " << mainDir << "\n";
        }

        for (int i = 0; i < 5; ++i) {
            string fileName = generateRandomName();
            fs::path filePath = mainDir / fileName;
            ofstream out(filePath);
            if (!out) throw runtime_error("Failed to create file: " + filePath.string());
            out << generateRandomContent(100);
            out.close();
        }

        cout << "\nThe contents of the directory:\n";
        for (const auto& entry : fs::directory_iterator(mainDir)) {
            if (fs::is_regular_file(entry)) {
                cout << "File: " << entry.path().filename()
                    << ", Size: " << fs::file_size(entry.path()) << " bite\n";
            }
        }

        if (!fs::exists(backupDir)) {
            fs::create_directory(backupDir);
        }

        for (const auto& entry : fs::directory_iterator(mainDir)) {
            if (fs::is_regular_file(entry)) {
                fs::path destination = backupDir / entry.path().filename();
                fs::copy_file(entry.path(), destination, fs::copy_options::overwrite_existing);
            }
        }

        for (const auto& entry : fs::directory_iterator(mainDir)) {
            if (fs::is_regular_file(entry)) {
                fs::remove(entry.path());
            }
        }

        cout << "\nFiles copied to subdirectory 'backup', originals deleted.\n";

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
