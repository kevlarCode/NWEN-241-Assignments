
#include <stdio.h>
#include "editor.h"
#include <string.h>
#include <stdbool.h>



// Task 1 (Core)

int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos)
{
    if (pos<0 || pos > editing_buflen -1){ // if not in the range return 0 
        return 0;
    }
    for (int i = editing_buflen - 1; i > pos; i--){
        char char_replace = editing_buffer[i-1];
        editing_buffer[i] = char_replace;
  
        }
    editing_buffer[pos] = to_insert;
    if (editing_buffer[pos] == to_insert){
        return 1;
    }
    return 0;
}



// Task 2 (Core)

int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset)
{
    int index_delete = editing_buflen + 1; //initialising value out of the length
    for (int i = offset; i < editing_buflen -1; i++){
        if (editing_buffer[i] == to_delete){
            index_delete = i;
        }
    }
    if (index_delete > editing_buflen){ // checking if the index has been updated
        return 0;
    }
    for (int i = index_delete; i < editing_buflen-1; i++){ // move all the other characters back
        char char_replace = editing_buffer[i+1];
        editing_buffer[i] = char_replace;
    }
    return 1;
}

// Method used for task 3 (Completion)
// Method that looks for str in the buffer returns the index of where str begins, or returns -1
// if str is not found.
int match_string(char editing_buffer[], int editing_buflen, const char *str, const char *replacement, int offset) 
{
    for(int i = offset; i < editing_buflen - 1; i++) {
        for(int x = 0; x < strlen(str); x++){
            if(editing_buffer[i+x] != str[x]){
                break;
            }         
        //if word found break out
            if(x == strlen(str)-1) {
                return i;
                }
        }
    }
    return -1;
}
 
// Task 3 (Completion)
int editor_replace_str(char editing_buffer[], int editing_buflen, const char *str, const char *replacement, int offset)
{
    int str_len = strlen(str);
    int replacement_len = strlen(replacement);
    int match_index = match_string(editing_buffer, editing_buflen, str, replacement, offset); // method to find str index

    //Case 1: Check String is empty or if str not found
    if (str_len == 0 || match_index == -1) {
        return -1;
    }

    // Step 1 - Delete the Str 
    for (int i = 0; i < str_len; i++) {
        editor_delete_char(editing_buffer, editing_buflen, str[i], match_index);
    }
    
    // Step 2 - add the replacement into the position
    for (int i = 0; i < replacement_len;i++) {
        if (match_index < editing_buflen){
            editor_insert_char(editing_buffer, editing_buflen, replacement[i], match_index);
            match_index ++;
        }
    }
    // Step 3 return last index value
    int last_index = match_index - 1; // get last index of replacment in buffer
    return last_index;
}



// Task 4 (Challenge)
void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap)
{    
    int row, col; // Declaring Rows and Colums
    char* tempList[rows];
    
    //Creating a 2D array with nulls 
    for(row = 0; row < rows; row++) {
        tempList[row] = (char*) viewing_buffer + cols * row;
        for(col = 0; col < cols; col++){
            tempList[row][col] ='\0'; 
        }
    }
    
    // Initialising rows and columns to starting position 
    row = 0;
    col = 0;
    
    for (int i = 0; i < editing_buflen; i++) {
        char character = editing_buffer[i];
        //1 - If the character is null - end of editing buffer.
        if (character == '\0'){
            break;
        } 
        
        //2 - if pointer is at the end of the line
        if (character == '\n') { 
            row++; // set position to colum 0 on next row
            col = 0; 
            continue;
        }
        
        //3 - if wrap and col is at the end of the col move it to next line
        if (wrap >= cols-1 && col >= cols - 1){
            row++;
            col = 0;
        }
        

        if (col >= cols - 1){
            continue;

        } else if (row >= rows) {
            break;
        }
        tempList[row][col] = character; //update character
        col++;
    }
}