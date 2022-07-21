/**
 * abstractdb.cpp
 * C++ source file that should contain implementation for member functions
 * - loadCSV()
 * - saveCSV()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Tasks 4 and 5)
 */ 

#include "abstractdb.hpp"
#include "vectordb.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;
namespace nwen {
    
    /** Task 5 - Challenge
     *  Load CSV loads the file with the given CSV
     */ 
    bool AbstractDbTable::loadCSV (string fileName) 
    {
        ifstream fsIn(fileName); 
        if (!fsIn.is_open())
            return false;
        // Temporary variables to read from file:
        string id, title, year, director;
        toload.clear(); // clear any existing movies in vector
        while (getline(fsIn, id, ',')) {
            getline(fsIn, title, ',');
            getline(fsIn, year, ',');
            getline(fsIn, director, '\n');

            if (!fsIn.good()) // if there is formatting error in file
                return false;
            movie m;
            // update values to struct movie.
            m.id = (unsigned long)strtoul(id.c_str(), NULL, 10);
            strcpy(m.title, title.c_str());
            m.year = (unsigned short)strtoul(year.c_str(), NULL, 10);
            strcpy(m.director, director.c_str());
            toload.push_back(m); 
        }
        if (!fsIn.eof()) { // if whole file has not been read.
            fsIn.close();
            return false;
        }
        fsIn.close();
        for (int i = 0; i < toload.size(); i++) { // add to database
            add(toload[i]);
        }
        return true;
    }

    /** Task 4 - Completion
     *  Save CSV function - saves the data base into a file with given name 
     */  
    bool AbstractDbTable::saveCSV (string fileName)
    {
        ofstream fOut (fileName, ios::out);
        if (fOut.is_open()) {
            for (int i = 0; i < rows(); i++){ // adds all the movies in data base to file
                movie *m = get(i);
                fOut<<(m)->id<<", \""<<(m)->title<<"\", "<<(m)->year<<", \""<<(m)->director<<"\""<<endl;
                if (!fOut.good()) {
                    cout<<"error writing to File"<<endl;
                    fOut.close();
                    return false; 
                }
            }
            fOut.close();
            return true;
        }
        fOut.close();
        return false;
    } 
}