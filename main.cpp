//
//  Marie-Claire Salha
//
//  This program will read in either roman or arabic numbers from a file, put it into a linked list, and will then ask the user
//  if they want to search for a certain number, or sort the numbers. When they ask to sort the numbers, they will be asked whether
//  or not they want to sort the arabic numbers or the roman numbers.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

struct numbers
{
    string arabic = "";
    string roman = "";
    numbers* next = nullptr;
};


// prototypes
int romanToArabic (string line);
string arabicToRoman (int number);
void fillList(numbers* &head, int, string);
bool searchArabic (numbers* head, string);
bool searchRoman (numbers* head, string);
void sortArabic (numbers* &head, int);
void sortRoman (numbers* &head, int);
int counting (numbers* head);
void display (numbers* head);


int main()
{
    fstream file;
    string line = "", roman = "", numSearch = "";
    bool endOfFile = false;
    long long int position = 0;
    int number = 0, arabic = 0, sum = 0;
    char menuOption = ' ', sortOption = ' ';
    numbers* head = nullptr;
    
    
    // opens the file in both input and output mode
    file.open("numbers.txt", ios::in | ios::out | ios::binary);
    
    // if the file opens correctly
    if(file)
    {
        // while it is not the end of the file
        while(!endOfFile)
        {
            
            // looks at the first character on the line, and if it's a space, then it's automatically seen as an arabic number,
            // but if it's anything else, then it's counted as a roman numeral
            if(file.peek() == ' ')
            {
                // gets position, changes position to where the number is, reads in the number, and then gets the rest of the line
                // so it can go to the next line
                position = file.tellg();
                
                //position += 17;
                
                file.seekg(position + 17); //WAS 17, DON'T FORGET TO CHANGE IT BACK
                file >> number;
                
                //if the person entered in an invalid arabic number, it will go in here, and then it will close the file, re-open it, and then skip the line
                if(file.fail())
                {
                    file.close();
                    file.open("numbers.txt", ios::in | ios::out | ios::binary);
                    file.seekg(position + 23);      //WAS 23, DON'T FORGET TO CHANGE IT BACK
                    endOfFile = file.eof();
                    //continue;     //LOGICAL FIX
                }
                
                else
                {
                    getline(file, line);
                
                    // checks if it's the end of the file yet
                    endOfFile = file.eof();
                
                    // sets the string roman to what the function returns
                    roman = arabicToRoman(number);
                
                    //this will call the function to put the nodes into the linked list
                    fillList (head, number, roman);
                
                    // this will take the position back to the beginning of the line, and write the roman number
                    // that was converted from the arabic number
                    //position -= 16;
                    
                    file.seekg(position);
                    file << roman;
                    file.flush();   //LOGICAL FIX
                
                    // reads the rest of the line just so it can go to the next line and continue the program
                    getline(file, line);
                
                    // if the file is reading in an arabic number and it's at the end of the file, it will clear the buffer
                    // so that it's able to go back to the correct position and write in the converted roman numeral
                    if(endOfFile)
                    {
                        file.clear();
                        file.seekg(position);
                        file << roman;
                    }
                }
                
                
            }
            else
            {
                bool valid = false;
                // checks position, reads in the line, and sends the line into the function
                position = file.tellg();
                file >> roman;
                
                //this for loop will check through the string, and will set a bool to false if at any point in the string, there is an invalid character
                //otherwise, the bool is set to true, and we continue the program
                for(int i = 0; i < roman.length(); i++)
                {
                    if(roman.at(i) != 'I' || roman.at(i) != 'V' || roman.at(i) != 'X' || roman.at(i) !='L' || roman.at(i) != 'C' || roman.at(i) != 'D' || roman.at(i) != 'M')
                    {
                        getline(file, line);
                        valid = false;
                        endOfFile = file.eof();
                        //continue;
                    }
                    else
                    {
                        valid = true;
                        endOfFile = file.eof();
                    }
                }
                
                
                
                if(valid == true)
                {
                    getline(file, line);
                
                    // checks if it's the end of the file
                    endOfFile = file.eof();
                
                    // sets the converted arabic number to whatever the function returns
                    arabic = romanToArabic(roman);
                
                    //calls the function to put nodes into the linked list
                    fillList(head, arabic, roman);
                
                
                    // moves the file position to where the arabic number would be written, writes the number,
                    // and then moves on to the next line by reading the rest of the line
                    
                    position += 17;     //WAS 17, DON'T FORGET TO CHANGE IT BACK
                    file.seekg(position);
                
                    file << arabic;
                    getline(file, line);
                    if(endOfFile)
                    {
                        file.clear();
                        file.seekg(position);
                        file << arabic;
                    }
                }
            }
        }
    }
    
    // error message pops up if the file doesn't open
    else
    {
        cout << "The file does not exist.";
    }
    
    // closes the file so that there are no errors
    file.close();
    
    sum = counting(head);
    
    //this do-while loop will ask the user to enter in something from the menu.
    // it will keep looping until they enter "3" for quit
    do
    {
        // this will display the menu to the user
        cout << "What do you want to do with the file? Please enter in the number you want:\n";
        cout << "1. Search\n";
        cout << "2. Sort\n";
        cout << "3. Exit\n";
        cin >> menuOption;      //user enters in what menu option they want to do
        cout << endl;           //enters an endline into the console so that the menu doesn't look ugly
        
        //this will go through  each menu option
        switch(menuOption)
        {
            case '1':
                cout << "Please enter in the number that you want to search for:\n";       //this will ask them what number they want to search for, if they chose that
                cin >> numSearch;                                                          //they will then enter in the number they want to search
                
                // if what they entered is a digit at the beginning, then we know it's an arabic number that they want to search for
                if(isdigit(numSearch.at(0)))
                {
                    //the function returns true, then that means that it found the number the user was looking for
                    if(searchArabic(head, numSearch))
                    {
                        cout << "The number " << numSearch << " was found.\n";      //tells the user we found the number
                    }
                    //if it returned false, we obviously didn't find the number
                    else
                    {
                        cout << "The number " << numSearch << "was NOT found.\n";   //tells the user that we didn't find the number
                    }
                }
                // if what they entered is a letter at the beginning, then it is a roman number
                else if(isalpha(numSearch.at(0)))
                {
                    //if the function returns true, then it will output that we found the number
                    if(searchRoman (head, numSearch))
                    {
                        cout << "The number " << numSearch << " was found.\n";      //number was found
                    }
                    //otherwise, we will say that we didn't find the number
                    else
                    {
                        cout << "The number " << numSearch << " was NOT found.\n";    //number not found
                    }
                }
                break;
            case '2':
                //here, we will ask how they want to sort by
                cout << "What do you want to sort by? Please enter in the number you want:\n";
                cout << "1. Sort by Roman numeral\n";
                cout << "2. Sort by Arabic numeral\n";
                cin >> sortOption;
                //if they choose my roman, then we will call a specific funtion to sort that
                if(sortOption == '1')
                {
                    sortRoman(head, sum);
                }
                //if they want to sort by arabic, then we will call a specific function to sort that
                else if(sortOption == '2')
                {
                    sortArabic(head, sum);
                }
                break;
            case '3':
                break;
        }
        
    }while(menuOption != '3');
    
    // if they entered 3 as the menu option, meaning exit, then it is now time to display the contents of the linked list
    if(menuOption == 3)
    {
        //calls the function to display the linked list
        display(head);
    }
    
    return 0;
}


// Roman to Arabic numerals converter. Looks at the current character, and if it's +100, +10, or +1, it will look
// to the character ahead of it so that it can know if it should actually subtract or add to the total value.
int romanToArabic(string line)
{
    int arabic = 0;
    
    // this for loop will go through each position. If it has a letter, it will go and
    // convert, and if it has as space in it, it will just add zero
    for(int i = 0; i < line.length(); i++)
    {
        if(line.at(i) == 'M')
        {
            arabic += 1000;
        }
        
        else if(line.at(i) == 'D')
        {
            arabic += 500;
        }
        
        //  If the letter ahead of it is either M or D, then it will subtract 100 from the total, since
        // having this letter before those two means the value is subtracted by 100
        else if(line.at(i) == 'C')
        {
            if(line.at(i + 1) == 'M' || line.at(i + 1) == 'D')
            {
                arabic -= 100;
            }
            else
            {
                arabic += 100;
            }
        }
        
        else if(line.at(i) == 'L')
        {
            arabic += 50;
        }
        
        // if anything ahead of it isn't X, and is greater than X, then it will subtract 10
        else if(line.at(i) == 'X')
        {
            if(line.at(i + 1) == 'L' || line.at(i + 1) == 'C' || line.at(i) == 'D' || line.at(i) == 'M')
            {
                arabic -= 10;
            }
            else
            {
                arabic += 10;
            }
        }
        
        else if(line.at(i) == 'V')
        {
            arabic += 5;
        }
        
        // if anything ahead of it isn't I, and is greater than I, then it will subtract 1
        else if(line.at(i) == 'I')
        {
            if(line.at(i + 1) == 'V' || line.at(i + 1) == 'X' || line.at(i) == 'L' || line.at(i) == 'C' || line.at(i) == 'D' || line.at(i) == 'M')
            {
                arabic -= 1;
            }
            else
            {
                arabic += 1;
            }
        }
        
        // this will add zero when there is a space, just so there is something so I can make sure my program doesn't have any errors
        else
        {
            arabic += 0;
        }
    }
    
    // returns the converted number back to main
    return arabic;
    
}


// Converts from Arabic to Roman numbers. This will use division, mod, and string concatenation to figure out how to convert it to Roman numerals.
// Returns the number to the main function so that it can be written to the file
string arabicToRoman (int number)
{
    string roman = "";
    int fakeNumber = 0;
    
    // This series of if statements goes through each number possibility backwards, starting from 1000, until it reaches 1
    // If the number can be divide by 1000, then it will loop as many times as necessary to add M to the string
    if(number / 1000 > 0)
    {
        // uses a fake variable to use in the loop, so that we don't change the actual number
        fakeNumber = number / 1000;
        for(int i = 0; i < fakeNumber; i++)
        {
            // adds M to the string
            roman = roman + "M";
        }
        // mods the number so we don't have the thousands place still there
        number = number % 1000;
    }
    
    // if the number is greater than or equal to 500, and less than 900, then it will go into a loop to add D
    if(number / 500 == 1 && number < 900)
    {
        //uses fake variable for the loop
        fakeNumber = number / 500;
        for(int i = 0; i < fakeNumber; i++)
        {
            // adds D to the string
            roman = roman + "D";
        }
        // mods number to get rid of the 500
        number = number % 500;
    }
    
    // if the number is less than 500 but can still be divided by 100, it will go into this if statement
    if(number / 100 > 0)
    {
        // if the number is 400, then it will add CD
        if(number / 100 == 4)
        {
            // adds "CD" to the string
            roman = roman + "CD";
            // gets rid of the 400 and moves on to the next big block of If statements
            number = number % 100;
        }
        // if the number is 900, then it will add CM
        if(number / 100 == 9)
        {
            // adds "CM" to the string
            roman = roman + "CM";
            // gets rid of the 900 and moves on to the next block of If statements
            number = number % 100;
        }
        // if the number is not 400 or 900, then it will go through a loop to add C as many times as needed
        if(number / 100 != 4 && number / 100 != 9)
        {
            // fake variable for the loop
            fakeNumber = number / 100;
            for(int i = 0; i < fakeNumber; i++)
            {
                // adds "C" to the string
                roman = roman + "C";
            }
            // gets rid of the hundreds place
            number = number % 100;
        }
    }
    
    // if the number is 50, then it goes into a loop to add L
    if(number / 50 == 1 && number < 90)
    {
        // fake variable for the loop
        fakeNumber = number / 50;
        for(int i = 0; i < fakeNumber; i++)
        {
            // adds "L" to the string
            roman = roman + "L";
        }
        // gets rid of the 50
        number = number % 50;
    }
    
    // if the number is something minus 10, it will go into this if block
    if(number / 10 > 0)
    {
        // if the number is 40, then it will add "XL"
        if(number / 10 == 4)
        {
            // adds "XL" to the strings
            roman = roman + "XL";
            // gets rid of the tens place
            number = number % 10;
        }
        // if the number is 90, then it will add "XC"
        if(number / 10 == 9)
        {
            // adds "XC" to the string
            roman = roman + "XC";
            // gets rid of the tens place
            number = number % 10;
        }
        //if the number is not 40 and not 90, then it will add "X" as many times as needed
        if(number / 10 != 4 && number / 10 != 9)
        {
            // fake variable for the loop
            fakeNumber = number / 10;
            for(int i = 0; i < fakeNumber; i++)
            {
                // adds "X" to the string
                roman = roman + "X";
            }
            // gets rid of the tens place
            number = number % 10;
        }
    }
    
    // if the number is 5 and not 9, it will add a "V"
    if(number / 5 == 1 && number < 9)
    {
        // fake variable for the loop
        fakeNumber = number / 5;
        for(int i = 0; i < fakeNumber; i++)
        {
            // adds "V" to the string
            roman = roman + "V";
        }
        // mods 5 from the remaining number
        number = number % 5;
    }
    
    // if the number is greater than zero, then it will go into this block
    if(number / 1 > 0)
    {
        // if the number is 4, then it adds "IV" to the string
        if(number / 1 == 4)
        {
            // adds "IV" to the string
            roman = roman + "IV";
            // makes the number 0
            number = number % 4;
        }
        // if the number equals 9, then it will add "IX"
        if(number / 1 == 9)
        {
            // adds "IX" to the string
            roman = roman + "IX";
            // makes the number 0
            number = number % 9;
        }
        // if the number is not 4 or 9, then it will go into a loop and add as many I's as necessary
        if(number / 1 != 4 && number != 9)
        {
            // fake variable for the loop
            fakeNumber = number / 1;
            for(int i = 0; i < fakeNumber; i++)
            {
                // adds "I" to the string
                roman = roman + "I";
            }
            // makes the number 0
            number = number % number;
        }
    }
    
    // returns the converted number back to main
    return roman;
    
}

//
// This will fill the linked list with all of the numbers in the file
//
void fillList(numbers* &head, int arabic, string roman)
{
    // this will dynamically create a new pointer every time this function is called
    numbers* ptr = new numbers;
    
    // this will put in the data for the arabic and the roman numbers inside of that node in the struct
    ptr->arabic = arabic;
    ptr->roman = roman;
    
    // if head is empty, then we will set head to whatever is in ptr
    // we input the data into ptr first because if we did that in the else statement, then ptr would contain nothing
    // when we set head equal to it the first time
    if(!head)
    {
        // this sets head to whatever is in ptr
        head = ptr;
    }
    else
    {
        // points to a next and then sets that equal to head
        ptr->next = head;
        //sets head to whatever the pointer was originally holding
        head = ptr;
    }
    
}

//
// This function will go through the linked list and will look inside each node to check if the number inside
// is what the user is trying to look for. If the number they want is found, then the function will return
// true, otherwise it will return false.
//
bool searchArabic (numbers* head, string numSearch)
{
    bool found = false;
    numbers* ptr = head;
    
    // this loop will go on unti the pointer reaches the end of the list
    while(ptr != nullptr)
    {
        // if the value for arabic inside this node equals what the user wants then:
        if(ptr->arabic == numSearch)
        {
            //found is set to true and returns to the main function
            found = true;
            return found;
        }
        //otherwise:
        else
        {
            // pointer will equal the next node to continue moving on through the list
            ptr = ptr->next;
        }
    }

    // this will return false if the number was never found
    return found;
}

//
// This function will do the exact same thing as the searchArabic function. It will move through
// the linked list to see if it can find the number that the user is looking for.
//
bool searchRoman (numbers* head, string numSearch)
{
    bool found = false;
    numbers* ptr = head;
    
    // while the pointer has not reached the end of the list
    while(ptr != nullptr)
    {
        // if the roman number in the node is what the user is looking for
        if(ptr->roman == numSearch)
        {
            // the found is set to true, and returns that value
            found = true;
            return found;
        }
        //otherwise:
        else
        {
            // the pointer is set to the next node to continue searching throughout the list
            ptr = ptr->next;
        }
    }
    
    //if the number is never found, then it will return false
    return found;
}

//
// This function will sort the arabic numbers in the linked list
//
void sortArabic (numbers* &head, int sum)
{
    //this sets prev and ptr to the head of the linked list, and sets ptr2 to the second node in the list
    numbers* prev = head;
    numbers* ptr = head;
    numbers* ptr2 = head->next;
    int maxsize;                //this will be the max size of how many times we go through the linked list
    int i;
    
    // this will be the outer loop that will dictate how many times we go through the linked list
    for(maxsize = sum -1; maxsize > 0; maxsize--)
    {
        //this for loop will go through the linked list
        for(i = 0; i < (maxsize); i++)
        {
            //if ptr is equal to the head, then we will check to see if we need to swap the the first two nodes
            if(ptr == head)
            {
                //if the first node is larger than the second node, then we will swap them
                if(ptr->arabic > ptr2->arabic)
                {
                    ptr->next = ptr2->next;     //this points ptr to the third node
                    ptr2->next = prev;          //this points ptr2 to the first node
                    head = ptr2;                //the next three statements reset all the values and make sure we have everything in the linked list in order again
                    ptr = ptr2;
                    prev = ptr2;
                }
                ptr = ptr->next;                //these next two lines will move ptr and ptr2 forward so that we can now compare the things in the body of the linked list
                ptr2 = ptr2->next->next;
            }
            
            //if ptr is not pointing to the head, then:
            else
            {
                //this will check if the first node is larger than the second node (in terms of the body)
                if(ptr->arabic > ptr2->arabic)
                {
                    prev->next = ptr2;          //this will set the prev pointer to where ptr2 is
                    ptr->next = ptr2->next;     //this will set ptr one node ahead of ptr2
                    ptr2->next = ptr;           //then this will point ptr2 to ptr so that the list connects and we successfully swap the nodes
                    
                }
                
                ptr = ptr->next;                //these next three lines will move all three nodes forward
                ptr2 = ptr2->next;
                prev = prev->next;
            }
        }
        
        
        prev = head;                            //next three lines set pointers back to the beginning so that we can start the process all over again
        ptr = head;
        ptr2 = head->next;
    }
    
    
}

//
//This function will sort the roman characters in the linked list
//
void sortRoman (numbers* &head, int sum)
{
    //this sets prev and ptr to the head of the linked list, and sets ptr2 to the second node in the list
    numbers* prev = head;
    numbers* ptr = head;
    numbers* ptr2 = head->next;
    int maxsize;                //this will be the max size of how many times we go through the linked list
    int i;
    
    // this will be the outer loop that will dictate how many times we go through the linked list
    for(maxsize = sum -1; maxsize > 0; maxsize--)
    {
        //this for loop will go through the linked list
        for(i = 0; i < (maxsize); i++)
        {
            //if ptr is equal to the head, then we will check to see if we need to swap the the first two nodes
            if(ptr == head)
            {
                //if the first node is larger than the second node, then we will swap them
                if(ptr->roman > ptr2->roman)
                {
                    ptr->next = ptr2->next;     //this points ptr to the third node
                    ptr2->next = prev;          //this points ptr2 to the first node
                    head = ptr2;                //the next three statements reset all the values and make sure we have everything in the linked list in order again
                    ptr = ptr2;
                    prev = ptr2;
                }
                ptr = ptr->next;                //these next two lines will move ptr and ptr2 forward so that we can now compare the things in the body of the linked list
                ptr2 = ptr2->next->next;
            }
            
            //if ptr is not pointing to the head, then:
            else
            {
                //this will check if the first node is larger than the second node (in terms of the body)
                if(ptr->roman > ptr2->roman)
                {
                    prev->next = ptr2;          //this will set the prev pointer to where ptr2 is
                    ptr->next = ptr2->next;     //this will set ptr one node ahead of ptr2
                    ptr2->next = ptr;           //then this will point ptr2 to ptr so that the list connects and we successfully swap the nodes
                    
                }
                
                ptr = ptr->next;                //these next three lines will move all three nodes forward
                ptr2 = ptr2->next;
                prev = prev->next;
            }
        }
        
        
        prev = head;                            //next three lines set pointers back to the beginning so that we can start the process all over again
        ptr = head;
        ptr2 = head->next;
    }
    
    
    
}

//
// This function will count how many nodes are in the linked list
//
int counting(numbers* head)
{
    int sum = 1;            //sets the sum to 1 because we will start off assuming that we have at least one node
    numbers* ptr = head;    //sets ptr to the beginning of the linked list
    
    //while ptr is not pointing to nullptr, it will continue to count how many nodes there are
    while(ptr)
    {
        ptr = head->next;      //the first thing it does is set ptr to the next node, because we have already assumed there is one node, so we are now going to the next
                               //node to figure out if there even *is* another one
        sum += 1;              //we will increase sum by 1
    }
    
    return sum;     //we will return the sum
}

// This will display the contents of the linked list to the file, erasing whatever was there before, and showing what is now in the list, sorted either
// by Roman or by Arabic
void display (numbers* head)
{
    fstream file;           //creates a new fstream so we can open the file again
    numbers* ptr = head;    //creates a fake pointer to go through the list with so that we don't have to deal with moving head
    
    //opens the file only in output mode
    file.open("numbers.txt", ios::out | ios::binary);
    
    //while the ptr is not pointing to null, then it will continue to output the contents of the linked list
    while(ptr)
    {
        file << setw(17) << ptr->roman;     // this will set the correct amount of space for the roman number to be written in
        file << ptr->arabic;                // by the time the roman number is written, the arabic number will be written in the correct spot bc of the setw(17)
        file << endl;                       // this goes to the next line in the file so that not everything is written on one line
        ptr = ptr->next;                    // moves the pointer to the next node in the list so it doesn't keep displaying the same value, and so that we don't get an infinite loop
    }
    
    file.close();       //closes the file
}



























