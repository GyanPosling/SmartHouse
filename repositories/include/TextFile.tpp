#include "TextFile.hpp"
#include "../../exceptions/include/FileException.hpp"
#include <sstream>
#include <type_traits>

template <typename T>
void TextFile<T>::clearFile() const
{
    ofstream file(filename, ios::out | ios::trunc);
    if (!file.is_open())
    {
        throw FileException(60, "Error clear file: " + filename);
    }
    file.close();
}

template <typename T>
void TextFile<T>::openFile(fstream& file, ios_base::openmode mode) const
{
    file.open(filename, mode);
    if (!file.is_open())
    {
        throw FileException(61, "Error open file: " + filename);
    }
}

template <typename T>
void TextFile<T>::saveRecord(const T& object)
{
    fstream file;
    openFile(file, ios::out | ios::app);
    
    file << object << endl; 
    
    file.close();
}

template <typename T>
T* TextFile<T>::readRecord()
{
    fstream file;
    openFile(file, ios::in);
    
    T* object = new T(); 
    
    if (file >> *object)
    {
        file.close();
        return object;
    }
    
    delete object;
    file.close();
    return nullptr;
}

template <typename T>
Deque<T*> TextFile<T>::readAllRecords()
{
    Deque<T*> records;
    fstream file;
    openFile(file, ios::in);

    while (file.peek() != EOF)
    {
        T* object = new T();
        if (file >> *object)
        {
            records.pushBack(object);
        }
        else
        {
            delete object;
            break;
        }
    }
    
    file.close();
    return records;
}
