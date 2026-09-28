#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
protected:
    std::string firstName;
    std::string surname;

public:
    Person(std::string f = "", std::string s = "") : firstName(f), surname(s) {}
    virtual ~Person() = default;
    std::string getFirstName() const { return firstName; }
    std::string getSurname() const { return surname; }
};

#endif // PERSON_H