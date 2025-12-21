#pragma once
#include "File.hpp"
#include "../../templates/Deque.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template <typename T>
class TextFile : public File
{
public:
    TextFile(const string& fname) : File(fname) {}
    ~TextFile() = default;

    void saveRecord(const T& object);
    T* readRecord();
    Deque<T*> readAllRecords();
    void clearFile() const;
    void openFile(fstream& file, ios_base::openmode mode) const;
};

#include "TextFile.tpp"
