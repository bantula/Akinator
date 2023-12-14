#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

using namespace std;

class FileOpenException : public exception {

public:
    FileOpenException() : exception("Izuzetak: Losa putanja ili nije ucitan fajl") {}

    // Uzimamo ime datoteke vezano za izuzetak
    const string& getPutanja() const {
        return putanja;
    }

private:
    string putanja;

};

#endif

