#include "../include/FileRepository.hpp"

FileRepository::FileRepository(const string& file) : filename(file) {}

FileRepository::~FileRepository() {
    close();
}

bool FileRepository::fileExists() const {
    ifstream file(filename);
    return file.good();
}

void FileRepository::openForRead() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    inputFile.open(filename, ios::in);
    if (!inputFile.is_open()) {
        throw FileException(100, "Failed to open file " + filename + " for reading");
    }
}

void FileRepository::openForWrite() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
    outputFile.open(filename, ios::out | ios::trunc);
    if (!outputFile.is_open()) {
        throw FileException(101, "Failed to open file " + filename + " for writing");
    }
}

void FileRepository::openForAppend() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
    outputFile.open(filename, ios::out | ios::app);
    if (!outputFile.is_open()) {
        throw FileException(102, "Failed to open file " + filename + " for appending");
    }
}

void FileRepository::close() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    if (outputFile.is_open()) {
        outputFile.close();
    }
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

string FileRepository::getFilename() const {
    return filename;
}

void FileRepository::setFilename(const string& file) {
    filename = file;
}

