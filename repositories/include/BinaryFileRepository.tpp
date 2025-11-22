template<typename T>
BinaryFileRepository<T>::BinaryFileRepository(const string& file) : FileRepository(file) {}

template<typename T>
void BinaryFileRepository<T>::write(const vector<T>& data) {
    outputFile.open(filename, ios::binary | ios::out | ios::trunc);
    if (!outputFile.is_open()) {
        throw FileException(103, "Failed to open file " + filename + " for binary writing.");
    }
    for (const auto& item : data) {
        outputFile.write(reinterpret_cast<const char*>(&item), sizeof(T));
    }
    outputFile.close();
}

template<typename T>
vector<T> BinaryFileRepository<T>::readVector() {
    vector<T> data;
    inputFile.open(filename, ios::binary | ios::in);
    if (!inputFile.is_open()) {
        throw FileException(105, "Failed to open file " + filename + " for binary reading.");
    }
    T item;
    while (inputFile.read(reinterpret_cast<char*>(&item), sizeof(T))) {
        data.push_back(item);
    }
    inputFile.close();
    return data;
}

template<typename T>
void BinaryFileRepository<T>::append(const T& item) {
    outputFile.open(filename, ios::binary | ios::out | ios::app);
    if (!outputFile.is_open()) {
        throw FileException(107, "Failed to open file " + filename + " for binary appending.");
    }
    outputFile.write(reinterpret_cast<const char*>(&item), sizeof(T));
    outputFile.close();
}

