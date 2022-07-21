/**
 * abstractdb.hpp
 * C++ header file that should contain declaration for
 * - struct movie (given)
 * - AbstractDbTable abstract class
 * 
 * You need to modify this file to declare AbstractDbTable abstract class 
 * as specified in the hand-out (Task 1)
 */ 

// Name : Kevin Lee
// ID : 300563771


#ifndef __ABSTRACT_DB_HPP__
#define __ABSTRACT_DB_HPP__

#include <string>
#include <vector> // to store movies that were loaded from CSV.

using namespace std;

namespace nwen 
{
    struct movie {
        unsigned long id;
        char title[50];
        unsigned short year;
        char director[50];
    };


    /** Task 1 - Core
     * Declaration for the AbstractDbTable Function
     */ 
    class AbstractDbTable {
    public: 
        virtual int rows() const = 0;
        virtual movie* get(int index) const = 0;
        virtual bool add(movie &film) = 0;
        virtual bool update(unsigned long int id, movie &film) = 0; 
        virtual bool remove(unsigned long int id) = 0;

        // Non- pure virtual Functions
        bool loadCSV (string filenName);
        bool saveCSV (string fileName);
    private:
        std::vector<movie> toload; // vector for all movies retrived from the loadCSV file.
        //This is needed as my implementation of the code uses pointers and if I do not store these movies the local variables will be destroyed 
        // and garbage values will be printed. 
    };
}

#endif /* __ABSTRACT_DB_HPP__ */
