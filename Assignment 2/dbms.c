// Name : Kevin Lee 
// ID : 300563771
// user ID : leekevi
#include <stdio.h> // standard C libraries
#include <stdlib.h>
#include "dbms.h" // header file

// Task 1 (CORE)
int db_show_row(const struct db_table *db, unsigned int row)
{
    struct album *album_ptr= (db->table) + row;
    if (db == NULL || row > db->rows_used || row < 0 || album_ptr == NULL) 
        return 0;

    printf("%6lu:%20.20s:%20.20s:%4d\n", // Print format (6, 20, 20, 4)
        album_ptr->id, album_ptr->title, album_ptr->artist, album_ptr->year);   
    return 1;

}


// Task 2 (COMPLETION)
int db_add_row(struct db_table *db, struct album *a)
{
    struct album *album_ptr = (db->table); // copy of pointer
    
    if (db->rows_used == db->rows_total) // add extra 5 space
    {
        int new_length = db->rows_total + 5;
        album_ptr = (struct album *)realloc(album_ptr, new_length*sizeof(struct album));
        if (album_ptr == NULL) // check if allocation was succesful
            return 0; 
        db->table = album_ptr; //update original
        db->rows_total += 5;
        db->table[db->rows_used] = *a;
        db->rows_used += 1;
        return 1;
    }
    else if (db->rows_used < db->rows_total)
    {
        db->table[db->rows_used] = *a;
        db->rows_used += 1;
        return 1;
    }
    return 0;
}


// Task 3 (CHALLENGE)
int db_remove_row(struct db_table *db, unsigned long id)
{
    struct album *album_ptr = (db->table); // copy of pointer
    unsigned int removeRow = -1;
    for (int i = 0; i < db->rows_used -1; i++) 
    {
        if (db->table[i].id == id)
            removeRow = i; // matching row with id
    }

   
    if (removeRow < 0|| removeRow > db->rows_used) // id not found
        return 0;

    for (int i=removeRow; i < db->rows_used-1; i++)
        db->table[i] = db->table[i + 1];
    
    db->rows_used -= 1; // update the used rows    

    if (db->rows_total - db->rows_used > 4)
    {
        int new_length = db->rows_total - 5;
        album_ptr = (struct album *)realloc(album_ptr, new_length*sizeof(struct album));
        if (album_ptr == NULL) // check if allocation was succesful
            return 0; 
        db->table = album_ptr; // updating original
        db->rows_total -= 5;
    }
    return 1;
}


