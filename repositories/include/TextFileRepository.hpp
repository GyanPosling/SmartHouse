#pragma once
#include "FileRepository.hpp"
#include <vector>
using namespace std;

template<typename T>
class TextFileRepository : public FileRepository {
public:
    TextFileRepository(const string& file);
    virtual ~TextFileRepository() = default;
    
    void write(const vector<T>& data);
    vector<T> readVector();
    void append(const T& item);
};

#include "TextFileRepository.tpp"
