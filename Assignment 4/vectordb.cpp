

/**
 * vectordb.cpp
 * C++ source file that should contain implementation for member functions
 * - rows()
 * - get()
 * - add()
 * - update()
 * - remove()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Task 3)
 */ 

// Name : Kevin Lee 
// ID : 300563771

#include "vectordb.hpp"
#include <vector>
#include <string>
#include <cstring>

using namespace std;
using namespace nwen;

/** Task 3 - Core
 * Implementation and definitions to the functions in the VectorDbTable class
 */ 

VectorDbTable::VectorDbTable() {} // default Contructor

VectorDbTable::~VectorDbTable() {} // destructor

// Returns the number of rows in database
int VectorDbTable::rows() const 
{
    return table.size();
}

// returns a pointer to the movie at given row number
movie* VectorDbTable::get(int rowNum) const
{
    if (rowNum > table.size() || rowNum < 0) // checking for within bounds
        return NULL;

    return table.at(rowNum);

}

// returns a boolean describing whether given movie was sucessfully added to database
bool VectorDbTable::add(movie &film)
{
    for (vector<movie*>:: iterator i = table.begin(); i != table.end(); i++){
        if ((*i)->id == film.id) {  
            return false; // if Id already Exists
        }
    }
    movie *m = &film;
    table.push_back(m);
    return true;
}

// returns a boolean on whether the given movie id is updated or not
bool VectorDbTable::update(unsigned long int id, movie &film) 
{
    for (int i = 0; i < table.size(); i++) {
        if ((table.at(i))->id == id) { // id found
            movie *m = &film;
            table[i] = m;
            return true;
        } 
    }
    return false; // id never found 
}


bool VectorDbTable::remove(unsigned long int id)
{
    for (int i = 0; i < table.size(); i++) {
        if ((table[i])->id == id) {
            table.erase(table.begin() + i); // remove movie record
            return true;
        }
    }
    return false; // id never found
}