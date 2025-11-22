#pragma once
#include "FileRepository.hpp"
#include <vector>
using namespace std;

template<typename T>
class BinaryFileRepository : public FileRepository {
public:
    BinaryFileRepository(const string& file);
    virtual ~BinaryFileRepository() = default;
    
    void write(const vector<T>& data);
    vector<T> readVector();
    void append(const T& item);
};

#include "BinaryFileRepository.tpp"
