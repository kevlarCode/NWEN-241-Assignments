/**
 * vectordb.hpp
 * C++ header file that should contain declaration for
 * - VectorDbTable class
 * 
 * You need to modify this file to declare VectorDbTable class 
 * as specified in the hand-out (Task 2)
 */ 
#include "abstractdb.hpp"
#include <vector>
using namespace std;

/** Task 2 - Core
 * Declarations for the VectorDbTable class
 */ 
namespace nwen 
{
    class VectorDbTable : public AbstractDbTable {
    public:
        VectorDbTable();  // default Constructor
        ~VectorDbTable();// default destructor
        int rows() const;
        movie *get(int rowNum) const;
        bool add(movie &film);
        bool update(unsigned long int id, movie &film);
        bool remove(unsigned long int id);

    private: 
        std::vector<movie*> table;
    };
}
