template<typename T>
TextFileRepository<T>::TextFileRepository(const string& file) : FileRepository(file) {}

template<typename T>
void TextFileRepository<T>::write(const vector<T>& data) {
    openForWrite();
    for (const auto& item : data) {
        outputFile << item << "\n";
    }
    close();
}

template<typename T>
vector<T> TextFileRepository<T>::readVector() {
    vector<T> result;
    if (!fileExists()) {
        return result;
    }
    openForRead();
    T item;
    while (inputFile >> item) {
        result.push_back(item);
    }
    close();
    return result;
}

template<typename T>
void TextFileRepository<T>::append(const T& item) {
    openForAppend();
    outputFile << item << "\n";
    close();
}

