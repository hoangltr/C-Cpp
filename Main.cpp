
#define MaxBookID 1000000
#define NumBookID 6
#define MaxReaderID 10000
#define NumReaderID 4

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>
#include <set>
#include <cstdlib>
#include <Windows.h>
#include <algorithm>


template<typename T>
std::string IDconvert(T number, int x) {
    std::ostringstream oss;
    oss << std::setw(x) << std::setfill('0') << number;
    std::string string_with_zeros = oss.str();

    return string_with_zeros;
}

std::string currenttime(bool condition) {
    //condition true -> return DD/MM/YYYY
    //condition flase -> return YYYY
    std::time_t currentTime;
    std::time(&currentTime);

    // Convert the current time to a struct tm
    std::tm currentDate;
    localtime_s(&currentDate, &currentTime);

    // Extract the components of the date
    int month = currentDate.tm_mon + 1; // tm_mon ranges from 0 to 11
    int day = currentDate.tm_mday;
    int year = currentDate.tm_year + 1900; // tm_year counts years since 1900

    // Create a string stream to format the date
    std::stringstream ss;
    if (condition)
    {
        ss << day << "/" << month << "/" << year;
        std::string date = ss.str();
        return date;
    }
    else
    {
        ss << year;
        std::string date = ss.str();
        return date;
    }
}

std::string formatName(const std::string& input) {
    std::stringstream ss(input);
    std::string name;
    std::string word;

    while (ss >> word) {
        if (!word.empty()) {
            // Capitalize the first letter
            word[0] = std::toupper(word[0]);

            // Convert remaining letters to lowercase
            for (size_t i = 1; i < word.length(); ++i) {
                word[i] = std::tolower(word[i]);
            }

            // Append the formatted word to the name
            name += word + " ";
        }
    }

    // Remove trailing space
    if (!name.empty()) {
        name.pop_back();
    }

    return name;
}

bool compareDates(const std::string& dateStr1, const std::string& dateStr2, bool condition) {
    // Define the date format
    std::string dateFormat = "%d/%m/%Y"; // Assuming the format is "DD-MM-YYYY"

    // Create tm objects from the date strings
    std::tm tm1 = {}, tm2 = {};
    std::istringstream ss1(dateStr1), ss2(dateStr2);
    ss1 >> std::get_time(&tm1, dateFormat.c_str());
    ss2 >> std::get_time(&tm2, dateFormat.c_str());

    // Convert tm objects to std::chrono::time_point
    auto tp1 = std::chrono::system_clock::from_time_t(std::mktime(&tm1));
    auto tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm2));

    // Compare the time_points
    //condition true -> "<="
    //condition false -> "=="
    if (condition)
    {
        return tp1 <= tp2;
    }
    else
    {
        return tp1 == tp2;
    }

}

bool canConvertToInt(const std::string& str) {
    try {
        std::size_t pos = 0;
        int num = std::stoi(str, &pos);
        return pos == str.length();  // Check if entire string was converted
    }
    catch (const std::exception&) {
        return false;
    }
}

bool compareNumString(const std::string& num1, const std::string& num2)
{
    if (canConvertToInt(num1) && canConvertToInt(num2))
    {
        return std::stoi(num1) <= std::stoi(num2);
    }
    else
    {
        return false;
    }
}

bool isDateValid(const std::string& dateStr) {
    // Define the date format
    std::string dateFormat = "%d/%m/%Y"; // Assuming the format is "DD/MM/YYYY"

    // Create a tm object from the date string
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, dateFormat.c_str());

    // Check if the parsing was successful
    if (ss.fail()) {
        return false;
    }

    // Check if the parsed values are within valid range
    int day = tm.tm_mday;
    int month = tm.tm_mon + 1; // tm_mon is zero-based
    int year = tm.tm_year + 1900; // tm_year is years since 1900

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
        return false;
    }

    // Additional check for February and leap years
    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !isLeapYear)) {
            return false;
        }
    }

    return true;
}

COORD getCurrentCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
    return bufferInfo.dwCursorPosition;
}

void TextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoXY(int x, int y) { // x la ngang, y la doc
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Ox(int x1, int x2, int y) {
    for (int j = x1; j <= x2; j += 2) {
        gotoXY(j, y);
        std::cout << "-";
    }
}

void Oy(int y1, int y2, int x, int d) {
    for (int j = y1; j <= y2; j++) {
        gotoXY(x, j);
        if ((j - y1) % d == 0)
            std::cout << "+";
        else
            std::cout << "|";
    }
}

void KhungChu(int x, int y, int* a, int b, int m, int n, int size, int color) {
    TextColor(color);
    // dong
    for (int i = 0; i <= m; i++)
        Ox(x, x + size, y + b * i);
    // cot
    Oy(y, y + m * b, x, b);
    Oy(y, y + m * b, x + size, b);
    int d = 0;
    for (int i = 0; i < n; i++) {
        Oy(y, y + m * b, x + d + a[i], b);
        d += a[i];
    }
}

void title() {
    std::cout << "+--------------------------------------------------------------------------+----------------------------------------+" << std::endl;
    std::cout << "|                                                                          |               Team Members             |" << std::endl;
    std::cout << "|                                                                          |     Le Trong Hoang - 20119132          |" << std::endl;
    std::cout << "|                           Library Manager                                |     Duong Hoang Gia - 20119129         |" << std::endl;
    std::cout << "|                                                                          |     Nguyen Thi Lam Truc - 20119172     |" << std::endl;
    std::cout << "|                                                                          |             Group : 4                  |" << std::endl;
    std::cout << "+--------------------------------------------------------------------------+----------------------------------------+" << std::endl;
}



class NodeBook {
public:
    std::string ID_Book;
    std::string Book_Name;
    std::string author;
    std::string category;
    std::string publish_year;
    int numBooks;
    int price;
    std::string dateWrite;

    NodeBook(const std::string& ID_Book, const std::string& Book_Name, const std::string& author,
        const std::string& category, const std::string& publish_year, int numBooks, int price, const std::string& dateWrite)
        : ID_Book(ID_Book), Book_Name(Book_Name), author(author), category(category), publish_year(publish_year),
        numBooks(numBooks), price(price), dateWrite(dateWrite), next(nullptr) {}

    NodeBook* next;
};

class Book {
private:
    NodeBook* head;
public:
    Book() : head(nullptr) {}

    bool isFull() {
        NodeBook* current = head;
        int i = 0;
        while (current != nullptr)
        {
            i++;
            current = current->next;
        }
        return i < MaxBookID;
    }

    bool CheckID(const std::string& ID) {
        NodeBook* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->ID_Book == ID) {
                found = true;
                break;
            }
            current = current->next;
        }

        if (found) {
            return true;
        }
        else {
            return false;
        }

    }

    void addBook() {
        if (!isFull())
        {
            std::cout << "List is full, can't add new book!";
        }
        else
        {
            std::string ID;
            std::string name;
            std::string author;
            std::string category;
            std::string publish_year;
            int numBooks;
            int price;
            std::string dateWrite;
            unsigned int i = 1;

            while (CheckID(IDconvert(i, NumBookID)))
            {
                i++;
            }
            ID = IDconvert(i, NumBookID);
            //fflush(stdin);
            std::cout << "Enter Book Name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            while (any_of(name.begin(), name.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                std::cout << "The Name of Book You Enter is Wrong, Please Enter Again: ";
                std::getline(std::cin, name);
            }
            name = formatName(name);
            std::cout << "Enter Author: ";
            std::getline(std::cin, author);
            while (any_of(author.begin(), author.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                std::cout << "The Author of Book You Enter is Wrong, Please Enter Again: ";
                std::getline(std::cin, author);
            }
            author = formatName(author);
            std::cout << "Enter Category  : ";
            std::getline(std::cin, category);
            while (any_of(category.begin(), category.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                std::cout << "The Category of Book You Enter is Wrong, Please Enter Again: ";
                std::getline(std::cin, category);
            }
            category = formatName(category);
            do {
                std::cout << "Enter Publishing Year: ";
                std::cin >> publish_year;
                if ((!compareNumString(publish_year, currenttime(false))) || (compareNumString(publish_year, "0")))
                {
                    std::cout << "The Publishing Year You Enter is Wrong, Please Enter Again";
                }
            } while ((!compareNumString(publish_year, currenttime(false))) || (compareNumString(publish_year, "0")));
            do {
                std::cout << "Enter Number of Book: ";
                std::cin >> numBooks;
                if (numBooks < 0)
                {
                    std::cout << "The Number of Book You Enter is Wrong, Please Enter Again";
                }
            } while (numBooks < 0);
            do {
                std::cout << "Enter Price of Book: ";
                std::cin >> price;
                if (price < 0)
                {
                    std::cout << "The Price of Book You Enter is Wrong, Please Enter Again";
                }
            } while (price < 0);


            dateWrite = currenttime(true);

            NodeBook* p = new NodeBook(ID, name, author, category, publish_year, numBooks, price, dateWrite);
            p->next = NULL;
            if (head == NULL)
            {
                head = p;
            }
            else {
                NodeBook* q = head;
                while (q->next != NULL) q = q->next;
                q->next = p;
            }
        }
    }

    void removeBook(const std::string& id) {
        NodeBook* current = head;
        NodeBook* prev = nullptr;
        bool found = false;

        while (current != nullptr) {
            if (current->ID_Book == id) {
                found = true;
                if (prev == nullptr) {
                    // Removing the first node
                    head = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (found) {
            std::cout << "Book with ID " << id << " removed successfully." << std::endl;
        }
        else {
            std::cout << "Book with ID " << id << " not found." << std::endl;
        }
    }

    void modifyBook(const std::string& id) {
        
        NodeBook* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->ID_Book == id) {
                found = true;
                std::cin.ignore();
                std::cout << "Enter the updated details for the book with ID " << id << ":" << std::endl;

                std::cout << "The old name : " << current->Book_Name << std::endl;
                std::cout << "Enter new Book name : ";
                std::getline(std::cin, current->Book_Name);
                while (any_of(current->Book_Name.begin(), current->Book_Name.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                    std::cout << "The Name of Book You Enter is Wrong, Please Enter Again: ";
                    std::getline(std::cin, current->Book_Name);
                }
                current->Book_Name = formatName(current->Book_Name);
                std::cout << "The old author : " << current->author << std::endl;
                std::cout << " Enter New Author : ";
                std::getline(std::cin, current->author);
                while (any_of(current->author.begin(), current->author.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                    std::cout << "The Author of Book You Enter is Wrong, Please Enter Again: ";
                    std::getline(std::cin, current->author);
                }
                current->author = formatName(current->author);
                std::cout << "The old category : " << current->category << std::endl;
                std::cout << " Enter new category : ";
                std::getline(std::cin, current->category);
                while (any_of(current->category.begin(), current->category.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                    std::cout << "The Category of Book You Enter is Wrong, Please Enter Again: ";
                    std::getline(std::cin, current->category);
                }
                std::cout << "The old publish year : " << current->publish_year << std::endl;
                std::cout << "Enter New Publishing Year: ";
                do {
                    std::cin >> current->publish_year;
                    if ((!compareNumString(current->publish_year, currenttime(false))) || (compareNumString(current->publish_year, "0")))
                    {
                        std::cout << "The Publishing Year You Enter is Wrong, Please Enter Again :";
                    }
                } while ((!compareNumString(current->publish_year, currenttime(false))) || (compareNumString(current->publish_year, "0")));
                std::cout << "The old number of book : " << current->numBooks << std::endl;
                std::cout << "Enter New Number of Book: ";
                do {

                    std::cin >> current->numBooks;
                    if (current->numBooks < 0)
                    {
                        std::cout << "The Number of Book You Enter is Wrong, Please Enter Again :";
                    }
                } while (current->numBooks < 0);
                std::cout << "The old price of book : " << current->price << std::endl;
                std::cout << "Enter New Price of Book: ";
                do {
                    
                    std::cin >> current->price;
                    if (current->price < 0)
                    {
                        std::cout << "The Price of Book You Enter is Wrong, Please Enter Again : ";
                    }
                } while (current->price < 0);

                std::cout << "Book details updated successfully." << std::endl;
                break;
            }
            current = current->next;
        }

        if (!found) {
            std::cout << "Book with ID " << id << " not found." << std::endl;
        }
    }

    void arrange() {
        if (head == nullptr || head->next == nullptr) {
            // Empty list or list with a single element
            return;
        }

        bool swapped;
        NodeBook* current;
        NodeBook* previous = nullptr;

        do {
            swapped = false;
            current = head;
            NodeBook* nextNode = current->next;

            if (current->ID_Book > nextNode->ID_Book) {
                // Update head to the second node
                head = nextNode;
                current->next = nextNode->next;
                nextNode->next = current;
                previous = nextNode;
                swapped = true;
            }
            else {
                previous = current;
                current = current->next;
            }

            while (current != nullptr && current->next != nullptr) {
                nextNode = current->next;

                if (current->ID_Book > nextNode->ID_Book) {
                    previous->next = nextNode;
                    current->next = nextNode->next;
                    nextNode->next = current;
                    previous = nextNode;
                    swapped = true;
                }
                else {
                    previous = current;
                    current = current->next;
                }
            }
        } while (swapped);
    }

    std::string findBookNameByID(const std::string& id) {
        NodeBook* current = head;
        while (current != nullptr) {
            if (current->ID_Book == id) {
                return current->Book_Name;
            }
            current = current->next;
        }
        return "";
    }

    int findBooktotal(const std::string& id) {
        NodeBook* current = head;
        while (current != nullptr) {
            if (current->ID_Book == id) {
                return current->numBooks;
            }
            current = current->next;
        }
        return 0;
    }

    void PrintOneBookbyID(const std::string& id) {
        NodeBook* current = head;
        while (current != nullptr) {
            if (current->ID_Book == id)
            {
                COORD cursorPosition = getCurrentCursorPosition();
                int x = cursorPosition.X;
                int y = cursorPosition.Y;

                int size[] = { 9,20,20,15,7,11,10,13 };
                gotoXY(x + 2, y + 1);
                std::cout
                    << std::left << std::setw(9) << "ID" << std::setw(20) << " Book Name"
                    << std::setw(20) << "Author Name" << std::setw(15) << "Category"
                    << std::setw(7) << "Year" << std::setw(11) << "Quantity"
                    << std::setw(10) << "Price" << std::setw(13) << "Date Added" << std::endl;

                gotoXY(x + 2, y + 3);
                std::cout
                    << std::left << std::setw(9) << current->ID_Book << std::setw(20) << current->Book_Name
                    << std::setw(20) << current->author << std::setw(15) << current->category
                    << std::setw(7) << current->publish_year << std::setw(11) << current->numBooks
                    << std::setw(10) << current->price << std::setw(13) << current->dateWrite << std::endl;

                KhungChu(x, y, size, 2, 2, 8, 105, 7);

                gotoXY(0, y + 6);

                return;
            }
            current = current->next;
        }
    }

    void print() {
        int size[] = { 9,20,20,15,7,11,10,13 };
        NodeBook* current = head;
        int num = 0;
        while (current != nullptr) {
            num = num + 1;
            current = current->next;
        }
        current = head;
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        int temp = 1;
        gotoXY(x + 2, y + temp);
        std::cout
            << std::left << std::setw(9) << "ID" << std::setw(20) << " Book Name"
            << std::setw(20) << "Author Name" << std::setw(15) << "Category"
            << std::setw(7) << "Year" << std::setw(11) << "Quantity"
            << std::setw(10) << "Price" << std::setw(13) << "Date Added";

        while (current != nullptr)
        {
            temp = temp + 2;
            gotoXY(x + 2, y + temp);
            std::cout
                << std::left << std::setw(9) << current->ID_Book << std::setw(20) << current->Book_Name
                << std::setw(20) << current->author << std::setw(15) << current->category
                << std::setw(7) << current->publish_year << std::setw(11) << current->numBooks
                << std::setw(10) << current->price << std::setw(13) << current->dateWrite;
            current = current->next;
        }
        KhungChu(x, y, size, 2, num + 1, 8, 105, 7);
        gotoXY(0, y + temp + 4);
    }

    void printBookList() {
        if (head == nullptr) {
            std::cout << "No data to print." << std::endl;
            return;
        }
        std::cout << "+--------------------------------------------------------------------------+----------------------------------------+" << std::endl;
        std::cout << "+                                                     Book List                                                     |" << std::endl;
        std::cout << "+-------------------------------------------------------------------------------------------------------------------+" << std::endl;
        int size[] = { 9,20,9,20,9,20,9,20 };
        NodeBook* current = head;
        int num = 0;
        while (current != nullptr) {
            num = num + 1;
            current = current->next;
        }
        if (num % 4 != 0)
        {
            num = static_cast<int>(num / 4) + 1;
        }
        else
        {
            num = static_cast<int>(num / 4);
        }
        current = head;
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        int temp = 1;
        gotoXY(x + 2, y + temp);
        std::cout
            << std::left << std::setw(9) << "ID" << std::setw(20) << " Book Name"
            << std::setw(9) << "ID" << std::setw(20) << "Book Name"
            << std::setw(9) << "ID" << std::setw(20) << "Book Name"
            << std::setw(9) << "ID" << std::setw(20) << "Book Name";

        while (current != nullptr)
        {
            temp = temp + 2;
            gotoXY(x + 2, y + temp);
            for (int i = 0; i < 4; i++)
            {
                if (current != nullptr)
                {
                    std::cout
                        << std::left << std::setw(9) << current->ID_Book << std::setw(20) << current->Book_Name;
                    current = current->next;

                }
            }
        }
        KhungChu(x, y, size, 2, num + 1, 8, 116, 7);
        gotoXY(0, y + temp + 3);
    }

    void loadDataFromFile(const std::string& filename) {
        std::ifstream inputFile(filename);
        std::string token;
        if (inputFile.is_open()) {
            if (inputFile.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "The file is empty." << std::endl;
                inputFile.close();
                return;
            }
            else {
                std::string line;
                while (std::getline(inputFile, line)) {
                    std::stringstream ss(line);
                    std::string ID;
                    std::string name;
                    std::string author;
                    std::string category;
                    std::string publish_year;
                    int numBooks;
                    int price;
                    std::string dateWrite;

                    // Read the data from each line
                    std::getline(ss, ID, '-');
                    std::getline(ss, name, '-');
                    std::getline(ss, author, '-');
                    std::getline(ss, category, '-');
                    std::getline(ss, publish_year, '-');
                    ss >> numBooks;
                    ss.ignore();
                    ss >> price;
                    ss.ignore();
                    std::getline(ss, dateWrite);

                    // Create a new NodeBook object
                    NodeBook* newNode = new NodeBook(ID, name, author, category, publish_year, numBooks, price, dateWrite);

                    // Add the new node to the book list
                    if (!head) {
                        head = newNode;
                    }
                    else {
                        NodeBook* current = head;
                        while (current->next) {
                            current = current->next;
                        }
                        current->next = newNode;
                    }
                }
            }
            inputFile.close();
            if (inputFile.peek() != std::ifstream::traits_type::eof())
            {
                std::cout << "Data loaded from file successfully." << std::endl;
            }
        }
        else {
            std::cout << "Unable to open file: " << filename << std::endl;
        }
    }

    void saveDataToFile(const std::string& filename) {
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            NodeBook* current = head;
            while (current != nullptr) {
                outputFile << current->ID_Book << "-" << current->Book_Name << "-" << current->author << "-"
                    << current->category << "-" << current->publish_year << "-" << current->numBooks << "-" << current->price << "-"
                    << current->dateWrite << std::endl;
                current = current->next;
            }
            outputFile.close();
            std::cout << "Data saved to file successfully." << std::endl;
        }
        else {
            std::cout << "Unable to open file for writing: " << filename << std::endl;
        }
    }

    bool checkEmpty() {
        if (head == nullptr)
        {
            std::cout << "There is no book." << std::endl;
            return true;
        }
        return false;
    }
};



class NodeReader {
public:
    std::string ID_Reader;
    std::string Reader_Name;
    std::string gender;
    std::string status;
    std::string dateWrite;
    std::string dateEnd;


    NodeReader(const std::string& ID_Reader, const std::string& Reader_name, const std::string& gender,
        const std::string& status, const std::string& datewrite, const std::string& dateend)
        : ID_Reader(ID_Reader), Reader_Name(Reader_name), gender(gender), status(status), dateWrite(datewrite), dateEnd(dateend), prev(nullptr), next(nullptr) {}

    NodeReader* prev;
    NodeReader* next;
};

class Reader {
private:
    NodeReader* head;
    NodeReader* tail;

public:
    Reader() : head(nullptr), tail(nullptr) {}

    bool checkFull() {
        NodeReader* current = head;
        int i = 0;
        while (current != nullptr)
        {
            i++;
            current = current->next;
        }
        return i < MaxReaderID;
    }

    bool CheckID(const std::string& ID) {
        NodeReader* current = head;

        while (current != nullptr) {
            if (current->ID_Reader == ID) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    void addReader() {
        std::string ID;
        std::string name;
        std::string gender;
        std::string status;
        std::string datewrite;
        std::string dateend;
        int temp = 0;
        unsigned int i = 1;

        while (CheckID(IDconvert(i, NumReaderID)))
        {
            i++;
        }
        ID = IDconvert(i, NumReaderID);

        std::cout << "Enter Reader Name: ";
        std::getline(std::cin, name);
        while (any_of(name.begin(), name.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
            std::cout << "The Reader Name You Enter is Wrong, Please Enter Again: ";
            std::getline(std::cin, name);
        }
        name = formatName(name);
        do {
            std::cout << "Enter gender (1.Male,2.Female,3.Unknow): ";
            std::cin >> temp;
            if ((temp < 1) || (temp > 3))
            {
                std::cout << "The Gender of Reader You Enter is Wrong, Please Enter Again";
            }
        } while ((temp < 1) || (temp > 3));
        if (temp == 1)
        {
            gender = "Male";
        }
        else
        {
            if (temp == 2)
            {
                gender = "Female";
            }
            else
            {
                gender = "Unknown";
            }
        }
        datewrite = currenttime(false);
        do
        {
            std::cout << "Enter Date Expired: ";
            std::cin >> dateend;
            if (!isDateValid(dateend) && !compareDates(currenttime(true), dateend, true))
            {
                std::cout << "The Date Expired You Enter is Wrong, Please Enter Again";
            }
        } while (!isDateValid(dateend) && !compareDates(currenttime(true), dateend, true));

        if (compareDates(currenttime(true), dateend, true))
        {
            status = "Normal";
        }
        else
        {
            status = "Expired";
        }

        NodeReader* newNode = new NodeReader(ID, name, gender, status, datewrite, dateend);

        if (head == nullptr) {
            // List is empty
            head = newNode;
            tail = newNode;
        }
        else {
            // Append new node to the end of the list
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void removeReader(const std::string& ID) {
        NodeReader* current = head;
        NodeReader* prevNode = nullptr;
        bool found = false;

        while (current != nullptr) {
            if (current->ID_Reader == ID) {
                found = true;
                break;
            }
            prevNode = current;
            current = current->next;
        }

        if (found) {
            if (current == head) {
                // Node to be removed is the head
                head = current->next;
                if (head)
                    head->prev = nullptr;
            }
            else if (current == tail) {
                // Node to be removed is the tail
                tail = current->prev;
                if (tail)
                    tail->next = nullptr;
            }
            else {
                // Node to be removed is in between
                prevNode->next = current->next;
                current->next->prev = prevNode;
            }

            delete current;
            std::cout << "Reader with ID " << ID << " has been removed." << std::endl;
        }
        else {
            std::cout << "Reader with ID " << ID << " not found." << std::endl;
        }
    }

    void modifyReader(const std::string& ID) {
        NodeReader* current = head;
        bool found = false;
        int temp;

        while (current != nullptr) {
            if (current->ID_Reader == ID) {
                found = true;
                break;
            }
            current = current->next;
        }

        if (found) {
            std::string name;
            std::string gender;
            std::string status;
            std::string dateend;

            std::cout << "Enter New Reader Name: ";
            std::getline(std::cin, name);
            while (any_of(name.begin(), name.end(), [](char c) { return !isalpha(c) && !isspace(c); })) {
                std::cout << "The Name of Book You Enter is Wrong, Please Enter Again: ";
                std::getline(std::cin, name);
            }
            name = formatName(name);
            do {
                std::cout << "Enter New Gender (1.Male,2.Female,3.Unknow): ";
                std::cin >> temp;
                if ((temp < 1) || (temp > 3))
                {
                    std::cout << "The Gender of Reader You Enter is Wrong, Please Enter Again";
                }
            } while ((temp < 1) || (temp > 3));
            if (temp == 1)
            {
                gender = "Male";
            }
            else
            {
                if (temp == 2)
                {
                    gender = "Female";
                }
                else
                {
                    gender = "Unknown";
                }
            }
            std::cout << "Enter new status: ";
            std::cin.ignore();
            std::getline(std::cin, status);

            do
            {
                std::cout << "Enter New Date Expired: ";
                std::cin >> dateend;
                if (!isDateValid(dateend))
                {
                    std::cout << "The Date Expired You Enter is Wrong, Please Enter Again";
                }
            } while (!isDateValid(dateend));

            // Modify the data of the found node
            current->Reader_Name = name;
            current->gender = gender;
            current->status = status;
            current->dateEnd = dateend;
            current->dateWrite = currenttime(true);

            std::cout << "Reader data modified successfully." << std::endl;
        }
        else {
            std::cout << "Reader with ID " << ID << " not found." << std::endl;
        }
    }

    void findReader(const std::string& id) {
        if (head == nullptr) {
            std::cout << "No data to print." << std::endl;
            return;
        }
        NodeReader* current = head;
        while (current != nullptr) {
            if (current->ID_Reader == id)
            {
                COORD cursorPosition = getCurrentCursorPosition();
                int x = cursorPosition.X;
                int y = cursorPosition.Y;

                int size[] = { 9,20,10,15,13,15 };
                gotoXY(x + 2, y + 1);
                std::cout
                    << std::left << std::setw(9) << "ID" << std::setw(20) << " Reader Name"
                    << std::setw(10) << "Gender" << std::setw(15) << "Status"
                    << std::setw(13) << "Date Added" << std::setw(15) << "Date Expired" << std::endl;

                gotoXY(x + 2, y + 3);
                std::cout
                    << std::left << std::setw(9) << current->ID_Reader << std::setw(20) << current->Reader_Name
                    << std::setw(10) << current->gender << std::setw(15) << current->status
                    << std::setw(13) << current->dateWrite << std::setw(15) << current->dateEnd << std::endl;

                KhungChu(x, y, size, 2, 2, 6, 82, 7);

                gotoXY(0, y + 6);

                return;
            }
            current = current->next;
        }

        std::cout << "Don't have Reader with ID : " << id << std::endl;
    }

    bool checkReaderID(const std::string& keysearch) {
        NodeReader* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->ID_Reader == keysearch) {
                found = true;
                break;
            }
            current = current->next;
        }
    }

    std::string getReaderName(const std::string& keysearch) {
        NodeReader* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->ID_Reader == keysearch) {

                return current->Reader_Name;
            }
            current = current->next;
        }

    }

    void arrangeByID() {
        if (!head || !head->next) {
            // No need to arrange if there are no nodes or only one node
            return;
        }

        bool swapped;
        NodeReader* current;
        NodeReader* last = nullptr;

        do {
            swapped = false;
            current = head;

            while (current && current->next != last) {
                if (current->next && current->ID_Reader > current->next->ID_Reader) {
                    // Swap the nodes
                    NodeReader* temp = current->next;
                    current->next = temp->next;
                    if (temp->next) {
                        temp->next->prev = current;
                    }
                    temp->prev = current->prev;
                    current->prev = temp;
                    temp->next = current;

                    if (current->prev) {
                        current->prev->next = temp;
                    }
                    else {
                        head = temp;
                    }

                    swapped = true;
                }

                current = current->next;
            }

            last = current;
        } while (swapped);
    }

    void printList() {
        if (head == nullptr) {
            std::cout << "No data to print." << std::endl;
            return;
        }
        int size[] = { 9,20,10,15,13,15 };
        NodeReader* current = head;
        int num = 0;
        while (current != nullptr) {
            num = num + 1;
            current = current->next;
        }
        current = head;
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        int temp = 1;
        gotoXY(x + 2, y + temp);
        std::cout
            << std::left << std::setw(9) << "ID" << std::setw(20) << " Reader Name"
            << std::setw(10) << "Gender" << std::setw(15) << "Status"
            << std::setw(13) << "Date Added" << std::setw(15) << "Date Expired";

        while (current != nullptr)
        {
            temp = temp + 2;
            gotoXY(x + 2, y + temp);
            std::cout
                << std::left << std::setw(9) << current->ID_Reader << std::setw(20) << current->Reader_Name
                << std::setw(10) << current->gender << std::setw(15) << current->status
                << std::setw(13) << current->dateWrite << std::setw(15) << current->dateEnd;
            current = current->next;
        }
        KhungChu(x, y, size, 2, num + 1, 6, 82, 7);
        gotoXY(0, y + temp + 4);

    }

    void loadDataFromFile(const std::string& filename) {
        std::ifstream inputFile(filename);
        if (inputFile.is_open()) {
            if (inputFile.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "File is empty." << std::endl;
                inputFile.close();
                return;
            }
            else {
                std::string line;
                while (std::getline(inputFile, line)) {
                    std::stringstream ss(line);
                    std::string ID;
                    std::string name;
                    std::string gender;
                    std::string status;
                    std::string datewrite;
                    std::string dateend;

                    // Read the data from each line
                    std::getline(ss, ID, '-');
                    std::getline(ss, name, '-');
                    std::getline(ss, gender, '-');
                    std::getline(ss, status, '-');
                    std::getline(ss, datewrite, '-');
                    std::getline(ss, dateend);

                    if (!compareDates(currenttime(true), dateend, true))
                    {
                        if (status == "Normal")
                        {
                            status = "Expired";
                        }
                    }

                    NodeReader* newNode = new NodeReader(ID, name, gender, status, datewrite, dateend);

                    if (!head) {
                        head = newNode;
                        tail = newNode;
                    }
                    else {
                        newNode->prev = tail;
                        tail->next = newNode;
                        tail = newNode;
                    }
                }
            }
            inputFile.close();
            std::cout << "Data loaded from file successfully." << std::endl;
        }
        else {
            std::cout << "Unable to open file: " << filename << std::endl;
        }
    }

    void saveDataToFile(const std::string& filename) {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cout << "Failed to open the file: " << filename << std::endl;
            return;
        }

        NodeReader* current = head;

        while (current != nullptr) {
            file << current->ID_Reader << "-" << current->Reader_Name << "-" << current->gender << "-"
                << current->status << "-" << current->dateWrite << "-" << current->dateEnd << std::endl;

            current = current->next;
        }

        file.close();
    }
};


class Books {
public:
    std::string ID;
    std::string name;
    std::string dateBorrow;
    std::string dateReturn;
    std::string status;
    Books* next;
};

class NodeBorrower {
public:
    std::string ID_reader;
    std::string Name_Reader;
    Books* books[3];
    NodeBorrower* next;
};

class BorrowerList {
private:
    NodeBorrower* head;

public:
    BorrowerList() {
        head = nullptr;
    }

    void addBorrower(const std::string& ID, const std::string& name) {
        NodeBorrower* newBorrower = new NodeBorrower();
        newBorrower->ID_reader = ID;
        newBorrower->Name_Reader = name;
        for (int i = 0; i < 3; ++i) {
            newBorrower->books[i] = nullptr;
        }
        newBorrower->next = nullptr;

        if (head == nullptr) {
            head = newBorrower;
        }
        else {
            NodeBorrower* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newBorrower;
        }
    }

    void removeBorrower(const std::string& borrowerID) {
        if (head == nullptr) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        if (head->ID_reader == borrowerID) {
            // The borrower to be removed is the head of the list
            NodeBorrower* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        NodeBorrower* current = head;
        NodeBorrower* previous = nullptr;

        while (current != nullptr && current->ID_reader != borrowerID) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            // Borrower not found in the list
            return;
        }

        if (previous != nullptr) {
            previous->next = current->next;
        }
        else {
            head = current->next;
        }

        delete current;
    }

    void modifyBorrower(const std::string& borrowerID, const std::string& newName) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            if (current->ID_reader == borrowerID) {
                current->Name_Reader = newName;
                return;
            }
            current = current->next;
        }
    }

    bool addBookForBorrower(const std::string& borrowerID, const std::string& bookID, const std::string& bookName, const std::string& borrowDate, const std::string& returnDate) {
        NodeBorrower* current = head;
        std::string currentdate = currenttime(true);
        while (current != nullptr) {
            if (current->ID_reader == borrowerID) {
                int borrowedCount = 0;
                for (int i = 0; i < 3; ++i) {
                    if (current->books[i] != nullptr) {
                        borrowedCount++;
                    }
                }
                if (borrowedCount == 3) {
                    std::cout << "The book borrowing limit has been reached." << std::endl;
                    return false;
                }
                for (int i = 0; i < 3; ++i) {
                    if (current->books[i] == nullptr) {
                        Books* newBook = new Books();
                        newBook->ID = bookID;
                        newBook->name = bookName;
                        newBook->dateBorrow = borrowDate;
                        newBook->dateReturn = returnDate;

                        newBook->status = (compareDates(currentdate, returnDate, true)) ? "Borrow" : "Expired";
                        newBook->next = nullptr;

                        current->books[i] = newBook;
                        return true;
                    }
                }
                break;
            }
            current = current->next;
        }
        return false;
    }

    bool removeBookForBorrower(const std::string& borrowerID, const std::string& bookID) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            if (current->ID_reader == borrowerID) {
                for (int i = 0; i < 3; ++i) {
                    if (current->books[i] != nullptr && current->books[i]->ID == bookID) {
                        delete current->books[i];
                        current->books[i] = nullptr;
                        return true;
                    }
                }
                break;
            }
            current = current->next;
        }
        return false;
    }

    void modifyBookForBorrower(const std::string& borrowerID, const std::string& bookID, const std::string& newBorrowDate, const std::string& newReturnDate) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            if (current->ID_reader == borrowerID) {
                for (int i = 0; i < 3; ++i) {
                    if (current->books[i] != nullptr && current->books[i]->ID == bookID) {
                        current->books[i]->dateBorrow = newBorrowDate;
                        current->books[i]->dateReturn = newReturnDate;
                        current->books[i]->status = (newReturnDate < newBorrowDate) ? "warning" : "borrow";
                        return;
                    }
                }
                break;
            }
            current = current->next;
        }
        return;
    }

    void modifyBookNameForAll(const std::string& BookID, const std::string& newBookName) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr && current->books[i]->ID == BookID) {
                    current->books[i]->name = newBookName;
                }
            }
            current = current->next;
        }
    }

    void removeBookForAll(const std::string& bookID) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr && current->books[i]->ID == bookID) {
                    delete current->books[i];
                    current->books[i] = nullptr;
                }
            }
            break;
            current = current->next;
        }
    }

    void displayOneBrowwer(const std::string& ID)
    {
        int num = 0;
        int temp = 0;
        NodeBorrower* current = head;
        while (current != nullptr)
        {

            if (current->ID_reader == ID)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (current->books[i] != nullptr) {
                        num++;
                    }
                }
                COORD cursorPosition = getCurrentCursorPosition();
                int x = cursorPosition.X;
                int y = cursorPosition.Y;
                int size[] = { 10,20 };
                int size2[] = { 10,20,13,13,10 };
                gotoXY(x + 1, y + 1);
                std::cout << std::left << std::setw(10) << "ID Reader" << std::setw(20) << "Reader Name"
                    << std::setw(10) << "Book ID" << std::setw(20) << "Book Name" << std::setw(13) << "Date Borrow"
                    << std::setw(13) << "Date Expired" << std::setw(10) << "Status";
                KhungChu(x, y, size, 2, 1, 2, 30, 7);
                KhungChu(x + 30, y, size2, 2, 1, 5, 66, 7);
                gotoXY(x + 1, y + 3);
                std::cout << std::left << std::setw(10) << current->ID_reader << std::setw(20) << current->Name_Reader;
                for (int i = 0; i < 3; i++)
                {

                    if (current->books[i] != nullptr) {
                        gotoXY(x + 33, y + 3 + temp);
                        std::cout << std::left << std::setw(10) << current->books[i]->ID
                            << std::setw(20) << current->books[i]->name << std::setw(13) << current->books[i]->dateBorrow
                            << std::setw(13) << current->books[i]->dateReturn << std::setw(10) << current->books[i]->status;

                    }
                    temp = temp + 2;
                }
                KhungChu(x, y + 2, size, 2 * num, 1, 2, 30, 7);
                KhungChu(x + 30, y + 2, size2, 2, num, 5, 66, 7);
                return;
            }
            current = current->next;
        }

    }

    void displayList() {
        int num = 0;
        int temp = 2;
        int temp2 = 2;
        NodeBorrower* current = head;
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        int size[] = { 10,20 };
        int size2[] = { 10,20,13,13,10 };
        gotoXY(x + 1, y + 1);
        std::cout << std::left << std::setw(10) << "ID Reader" << std::setw(20) << "Reader Name"
            << std::setw(10) << "Book ID" << std::setw(20) << "Book Name" << std::setw(13) << "Date Borrow"
            << std::setw(13) << "Date Expired" << std::setw(10) << "Status";
        KhungChu(x, y, size, 2, 1, 2, 30, 7);
        KhungChu(x + 30, y, size2, 2, 1, 5, 66, 7);
        gotoXY(x, y + temp);

        while (current != nullptr)
        {
            num = 0;
            for (int i = 0; i < 3; i++)
            {
                if (current->books[i] != nullptr) {
                    num++;
                }
            }
            gotoXY(x + 1, y + temp + 1);
            std::cout << std::left << std::setw(10) << current->ID_reader << std::setw(20) << current->Name_Reader;

            gotoXY(x + 31, y + temp + 1);

            for (int i = 0; i < 3; i++)
            {
                if (current->books[i] != nullptr)
                {
                    std::cout << std::left << std::setw(10) << current->books[i]->ID
                        << std::setw(20) << current->books[i]->name << std::setw(13) << current->books[i]->dateBorrow
                        << std::setw(13) << current->books[i]->dateReturn << std::setw(10) << current->books[i]->status;
                    temp = temp + 2;
                }
                gotoXY(x + 31, y + temp + 1);
            }
            if (num == 0) num = 1;
            
            KhungChu(x, y + temp2, size, 2 * num, 1, 2, 30, 7);
            KhungChu(x + 30, y + temp2, size2, 2, num, 5, 66, 7);
            temp2 = temp;

            current = current->next;
        }
        
        //cursorPosition = getCurrentCursorPosition();
        //x = cursorPosition.X;
        //y = cursorPosition.Y;
        gotoXY(0, y + temp + 3);
    }

    void loadDataFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Failed to open the file." << std::endl;
            return;
        }

        std::string line;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(file, line)) {
            
            
            // Read the ID_reader and Name_Reader
            std::getline(iss, token, '-');
            std::string ID_reader = token;
            std::getline(iss, token, '-');
            std::string Name_Reader = token;
            // Create a new borrower node
            addBorrower(ID_reader, Name_Reader);
            
            // Read the book details until the end of the line
            while (std::getline(iss, token, '-')) {
                std::string ID_Book = token;
                std::getline(iss, token, '-');
                std::string Name_Book = token;
                std::getline(iss, token, '-');
                std::string dateborrow = token;
                std::getline(iss, token, '-');
                std::string datefinish = token;
                std::getline(iss, token, '-');
                std::string status = token;

                // Add the book for the borrower
                addBookForBorrower(ID_reader, ID_Book, Name_Book, dateborrow, datefinish);
            }
        }

        file.close();
        std::cout << "Data loaded from file successfully." << std::endl;
    }

    void saveDataToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cout << "Failed to create the file." << std::endl;
            return;
        }

        NodeBorrower* current = head;
        while (current != nullptr) {
            file << current->ID_reader << "-" << current->Name_Reader;
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr) {
                    file << "-" << current->books[i]->ID << "-"
                        << current->books[i]->name << "-"
                        << current->books[i]->dateBorrow << "-"
                        << current->books[i]->dateReturn << "-"
                        << current->books[i]->status;
                }
            }
            file << std::endl;
            current = current->next;
        }

        file.close();
    }

    std::set<std::string> findUniqueBookIDs() {
        std::set<std::string> uniqueIDs;

        NodeBorrower* current = head;
        while (current != nullptr) {
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr) {
                    uniqueIDs.insert(current->books[i]->ID);
                }
            }
            current = current->next;
        }

        return uniqueIDs;
    }

    int countBookOccurrences(const std::string& bookID) {
        int count = 0;
        NodeBorrower* current = head;
        while (current != nullptr) {
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr && current->books[i]->ID == bookID) {
                    count++;
                }
            }
            current = current->next;
        }
        return count;
    }

    void arrangeByID() {
        if (head == nullptr || head->next == nullptr) {
            // The list is empty or contains only one element, no need to arrange
            return;
        }

        bool sorted = false;
        while (!sorted) {
            sorted = true;
            NodeBorrower* current = head;
            NodeBorrower* nextNode = current->next;

            while (nextNode != nullptr) {
                if (current->ID_reader > nextNode->ID_reader) {
                    // Swap the borrowers' positions
                    std::swap(current->ID_reader, nextNode->ID_reader);
                    std::swap(current->Name_Reader, nextNode->Name_Reader);
                    std::swap(current->books, nextNode->books);
                    sorted = false;
                }

                current = nextNode;
                nextNode = nextNode->next;
            }
        }
    }

    NodeBorrower* findBorrower(const std::string& borrowerID) {
        NodeBorrower* current = head;
        while (current != nullptr) {
            if (current->ID_reader == borrowerID) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // Borrower not found
    }

    void findAndPrintData(const std::string& borrowerID) {
        NodeBorrower* borrower = findBorrower(borrowerID);
        if (borrower != nullptr) {
            std::cout << "ID Reader: " << borrower->ID_reader << std::endl;
            std::cout << "Name Reader: " << borrower->Name_Reader << std::endl;

            for (int i = 0; i < 3; ++i) {
                if (borrower->books[i] != nullptr) {
                    std::cout << "Book ID: " << borrower->books[i]->ID << std::endl;
                    std::cout << "Book Name: " << borrower->books[i]->name << std::endl;
                    std::cout << "Borrow Date: " << borrower->books[i]->dateBorrow << std::endl;
                    std::cout << "Return Date: " << borrower->books[i]->dateReturn << std::endl;
                    std::cout << "Status: " << borrower->books[i]->status << std::endl;
                }
            }
        }
        else {
            std::cout << "Borrower with ID " << borrowerID << " not found." << std::endl;
        }
    }

    bool checkBorrowerID(const std::string& borrowerID) {
        NodeBorrower* borrower = findBorrower(borrowerID);
        return (borrower != nullptr);
    }

    int findBorrowerBookNum(std::string& ID) {
        NodeBorrower* current = head;
        int dem = 0;
        while (current != nullptr) {
            if (current->ID_reader == ID) {
                for (int i = 0; i < 3; ++i) {
                    if (current->books[i] != nullptr) {
                        dem++;
                    }
                }
                return dem;
            }
            current = current->next;
        }

    }

    std::string getBookIDofBorrower(std::string& ID, int i)
    {
        NodeBorrower* current = head;
        while (current != nullptr)
        {
            if (current->ID_reader == ID)
            {
                return current->books[i]->ID;
            }
        }
    }

    void printExpiredBorrower()
    {
        int num = 0;
        int temp = 3;
        std::cout << "Borrowers has overdue books :" << std::endl;
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        int size[] = { 10,20,10 };
        gotoXY(x + 1, y + 1);
        std::cout << std::left << std::setw(10) << "ID Reader" << std::setw(20) << "Reader Name" << std::setw(10) << "Status";
        KhungChu(x, y, size, 2, 1, 3, 40, 7);

        NodeBorrower* current = head;
        while (current != nullptr)
        {
            for (int i = 0; i < 3; ++i) {
                if (current->books[i] != nullptr) {


                    if (compareDates(currenttime(true), current->books[i]->dateReturn, true)) {
                        gotoXY(x + 1, y + temp);
                        std::cout << std::left << std::setw(10) << current->ID_reader << std::setw(20) << current->Name_Reader << std::setw(10) << "Expired";
                        num++;
                        break;
                    }
                }
            }
            temp += 2;
            current = current->next;
        }
        KhungChu(x, y + 2, size, 2, num, 3, 40, 7);
    }
};


class BookBorrow {
public:
    std::string ID_Book;
    std::string Book_Name;
    unsigned int NumBorrow;
};

class TreeNode {
public:
    BookBorrow data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const BookBorrow& book) {
        data = book;
        left = nullptr;
        right = nullptr;
    }
};

class BinaryTree {
private:
    TreeNode* root;

    TreeNode* addNodeHelper(TreeNode* node, const BookBorrow& book) {
        if (node == nullptr) {
            return new TreeNode(book);
        }

        if (compareNumString(book.ID_Book, node->data.ID_Book)) {
            node->left = addNodeHelper(node->left, book);
        }
        else {
            node->right = addNodeHelper(node->right, book);
        }

        return node;
    }

    TreeNode* removeNodeHelper(TreeNode* node, const std::string& ID) {
        if (node == nullptr) {
            return nullptr;
        }

        if (ID < node->data.ID_Book) {
            node->left = removeNodeHelper(node->left, ID);
        }
        else if (ID > node->data.ID_Book) {
            node->right = removeNodeHelper(node->right, ID);
        }
        else {
            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            TreeNode* minNode = findMinNode(node->right);
            node->data = minNode->data;
            node->right = removeNodeHelper(node->right, minNode->data.ID_Book);
        }

        return node;
    }

    TreeNode* findMinNode(TreeNode* node) {
        if (node == nullptr) {
            return nullptr;
        }

        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    TreeNode* findMaxNode(TreeNode* node) {
        if (node == nullptr) {
            return nullptr;
        }

        while (node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

    int findMax(TreeNode* node) {
        if (node == nullptr) {
            return INT_MIN;  // Assuming the data type is int
        }

        int currentMax = node->data.NumBorrow;  // Assuming data2 is the field you want to find the maximum of
        int leftMax = findMax(node->left);
        int rightMax = findMax(node->right);

        if (leftMax > currentMax) {
            currentMax = leftMax;
        }
        if (rightMax > currentMax) {
            currentMax = rightMax;
        }

        return currentMax;
    }

    int findMin(TreeNode* node) {
        if (node == nullptr) {
            return INT_MAX;  // Assuming the data type is int
        }

        int currentMin = node->data.NumBorrow;  // Assuming data2 is the field you want to find the minimum of
        int leftMin = findMin(node->left);
        int rightMin = findMin(node->right);

        if (leftMin < currentMin) {
            currentMin = leftMin;
        }
        if (rightMin < currentMin) {
            currentMin = rightMin;
        }

        return currentMin;
    }

    void findNode(TreeNode* node, int Num) {
        if (node == nullptr)
        {
            return;
        }

        if (node->data.NumBorrow == Num) {

            printNodeData(node);
        }


        findNode(node->left, Num);

        findNode(node->right, Num);
    }

    void printTreeHelper(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        int size[] = { 10,20,20 };
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        gotoXY(x + 1, y + 1);
        std::cout << std::left << std::setw(10) << node->data.ID_Book << std::setw(20) << node->data.Book_Name << std::setw(20) << node->data.NumBorrow;
        KhungChu(x, y, size, 2, 1, 3, 50, 7);
        gotoXY(x, y + 2);
        // Traverse left and right subtrees recursively
        printTreeHelper(node->left);
        printTreeHelper(node->right);
    }

    void saveToFileHelper(std::ofstream& file, TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        // Save current node's data to the file
        file << node->data.ID_Book << "-" << node->data.Book_Name << "-" << node->data.NumBorrow << std::endl;

        // Save data of left and right subtrees recursively
        saveToFileHelper(file, node->left);
        saveToFileHelper(file, node->right);
    }

    void findNodeByIDBookHelper(TreeNode* node, const std::string& ID_Book) {
        if (node == nullptr)
        {
            std::cout << "Book with ID :"<< ID_Book << "is not borrowed." << std::endl;
            return;
        }
        if (node->data.ID_Book == ID_Book) {
            int size[] = { 10,20,20 };
            std::cout << "The Book with " << ID_Book << " information :  " << std::endl;
            COORD cursorPosition = getCurrentCursorPosition();
            int x = cursorPosition.X;
            int y = cursorPosition.Y;
            gotoXY(x + 1, y + 1);
            std::cout << std::left << std::setw(10) << "ID Book" << std::setw(20) << "Book Name" << std::setw(20) << "Number Borrower ";
            KhungChu(x, y, size, 2, 1, 3, 50, 7);
            gotoXY(x, y + 2);
            printNodeData(node);
            cursorPosition = getCurrentCursorPosition();
            x = cursorPosition.X;
            y = cursorPosition.Y;
            gotoXY(x, y + 2);
            return;
        }

        if (compareNumString(ID_Book, node->data.ID_Book)) {
            return findNodeByIDBookHelper(node->left, ID_Book);
        }
        else {
            return findNodeByIDBookHelper(node->right, ID_Book);
        }
    }


public:
    BinaryTree() {
        root = nullptr;
    }

    void addNode(const BookBorrow& book) {
        root = addNodeHelper(root, book);
    }

    void removeNode(const std::string& ID) {
        root = removeNodeHelper(root, ID);
    }

    void findMaxNumBorrow() {
        TreeNode* maxNode = findMaxNode(root);
        if (maxNode != nullptr) {
            int size[] = { 10,20,20 };
            std::cout << "The Book most number of borrow information :  " << std::endl;
            COORD cursorPosition = getCurrentCursorPosition();
            int x = cursorPosition.X;
            int y = cursorPosition.Y;
            gotoXY(x + 1, y + 1);
            std::cout << std::left << std::setw(10) << "ID Book" << std::setw(20) << "Book Name" << std::setw(20) << "Number Borrower ";
            KhungChu(x, y, size, 2, 1, 3, 50, 7);
            gotoXY(x, y + 2);
            findNode(root, findMax(root));
            cursorPosition = getCurrentCursorPosition();
            x = cursorPosition.X;
            y = cursorPosition.Y;
            gotoXY(x, y + 2);
            return;
        }
    }

    void findMinNumBorrow() {
        TreeNode* minNode = findMinNode(root);
        if (minNode != nullptr) {
            int size[] = { 10,20,20 };
            std::cout << "The Book least number of borrow information :  " << std::endl;
            COORD cursorPosition = getCurrentCursorPosition();
            int x = cursorPosition.X;
            int y = cursorPosition.Y;
            gotoXY(x + 1, y + 1);
            std::cout << std::left << std::setw(10) << "ID Book" << std::setw(20) << "Book Name" << std::setw(20) << "Number Borrower ";
            KhungChu(x, y, size, 2, 1, 3, 50, 7);
            gotoXY(x, y + 2);
            findNode(root, findMin(root));
            cursorPosition = getCurrentCursorPosition();
            x = cursorPosition.X;
            y = cursorPosition.Y;
            gotoXY(x, y + 2);
            return;
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            saveToFileHelper(file, root);
            file.close();
            std::cout << "Data saved to file successfully." << std::endl;
        }
        else {
            std::cout << "Unable to open the file." << std::endl;
        }
    }

    void printTree() {
        int size[] = { 10,20,20 };
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        gotoXY(x + 1, y + 1);
        std::cout << std::left << std::setw(10) << "ID Book" << std::setw(20) << "Book Name" << std::setw(20) << "Number Borrower ";
        KhungChu(x, y, size, 2, 1, 3, 50, 7);
        gotoXY(x, y + 2);
        printTreeHelper(root);
    }

    void findNodeByIDBook(const std::string& ID_Book) {
        return findNodeByIDBookHelper(root, ID_Book);
    }

    void printNodeData(TreeNode* node) {
        int size[] = { 10,20,20 };
        COORD cursorPosition = getCurrentCursorPosition();
        int x = cursorPosition.X;
        int y = cursorPosition.Y;
        gotoXY(x + 1, y + 1);
        std::cout << std::left << std::setw(10) << node->data.ID_Book << std::setw(20) << node->data.Book_Name << std::setw(20) << node->data.NumBorrow;
        KhungChu(x, y, size, 2, 1, 3, 50, 7);
        gotoXY(x, y + 2);
    }

    int getNumborrow(std::string& bookID)
    {
        TreeNode* node = root;
        if (node == nullptr) {
            return 0;
        }

        if (node->data.ID_Book == bookID)
        {
            return node->data.NumBorrow;
        }


        printTreeHelper(node->left);
        printTreeHelper(node->right);
    }

    void ModifyNumBorrrowNode(const std::string& ID_Book, bool condition)
    {
        TreeNode* node = root;
        if (node == nullptr)
        {
            return;
        }
        if (node->data.ID_Book == ID_Book) {
            if (condition)
            {
                node->data.NumBorrow++;
                return;
            }
            else
            {
                node->data.NumBorrow--;
                return;
            }


        }

        if (ID_Book < node->data.ID_Book) {
            findNodeByIDBookHelper(node->left, ID_Book);
        }
        else {
            findNodeByIDBookHelper(node->right, ID_Book);
        }
    }

    void ModifyNameBook(const std::string& ID_Book, const std::string& Name)
    {
        TreeNode* node = root;
        if (node == nullptr)
        {
            return;
        }
        if (node->data.ID_Book == ID_Book) {
            node->data.Book_Name = Name;
        }

        if (ID_Book < node->data.ID_Book) {
            findNodeByIDBookHelper(node->left, ID_Book);
        }
        else {
            findNodeByIDBookHelper(node->right, ID_Book);
        }
    }

};



int main()
{
    std::string bookdata = "D:\\Textbook\\AandD\\FinalProject\\Data\\Book.txt";
    std::string readerdata = "D:\\Textbook\\AandD\\FinalProject\\Data\\Reader.txt";
    std::string borrowerdata = "D:\\Textbook\\AandD\\FinalProject\\Data\\Borrower.txt";
    std::string bookborrowdata = "D:\\Textbook\\AandD\\FinalProject\\Data\\BookBorrow.txt";

    Book BookList;
    Reader ReaderList;
    BorrowerList Borrower;
    BinaryTree Bookborrows;   
    BookBorrow Bookinfo;

    int choice = 0;
    bool exit = false;
    bool back = false;
    std::string BookID;
    std::string ReaderID;
    std::string temp;

    BookList.loadDataFromFile(bookdata);
    //Sleep(1000);
    ReaderList.loadDataFromFile(readerdata);
    //Sleep(1000);
    Borrower.loadDataFromFile(borrowerdata);
    //Sleep(1000);
    std::system("cls");

    std::set<std::string> uniqueBookIDs = Borrower.findUniqueBookIDs();
    for (const std::string& bookID : uniqueBookIDs) {
        Bookinfo.ID_Book = bookID;
        Bookinfo.Book_Name = BookList.findBookNameByID(bookID);
        Bookinfo.NumBorrow = Borrower.countBookOccurrences(bookID);
        Bookborrows.addNode(Bookinfo);
    }

    while (exit == false)
    {
        title();
        std::cout << "What you want to do ?" << std::endl;
        std::cout << "1.Book Manager" << std::endl;
        std::cout << "2.Reader Manager" << std::endl;
        std::cout << "3.Borrower Manager" << std::endl;
        std::cout << "4.Book Borrower Manager" << std::endl;
        std::cout << "5.Exit" << std::endl;
        std::cout << "Enter your choice : ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            back = false;
            while (back == false)
            {
                std::system("cls");
                title();
                std::cout << "Choose the function of Book Manager : " << std::endl;
                std::cout << "1.Add Book." << std::endl;
                std::cout << "2.Remove Book." << std::endl;
                std::cout << "3.Modify Book." << std::endl;
                std::cout << "4.Arrange Book List." << std::endl;
                std::cout << "5.Find Book Data." << std::endl;
                std::cout << "6.Print Book list." << std::endl;
                std::cout << "7.Exit" << std::endl;
                std::cout << "Enter your choice : ";
                std::cin >> choice;
                switch (choice)
                {
                case 1:
                    std::system("cls");
                    title();
                    BookList.addBook();
                    std::system("cls");
                    break;
                case 2:
                    std::system("cls");
                    title();
                    if (!BookList.checkEmpty())
                    {
                    BookList.printBookList();
                    fflush(stdin);
                        std::cout << "Enter Book ID You Want to Remove : ";
                        std::cin >> BookID;
                        while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxBookID))))
                        {
                            std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                            std::cin >> BookID;
                        }
                        BookID = IDconvert(BookID, NumBookID);
                        BookList.removeBook(BookID);
                        Borrower.removeBookForAll(BookID);
                        Bookborrows.removeNode(BookID);
                    }
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 3:
                    std::system("cls");
                    title();
                    if (!BookList.checkEmpty())
                    {
                        BookList.printBookList();
                        std::cout << "Enter Book ID You Want to Modify : ";
                        std::cin >> BookID;
                        while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxBookID))))
                        {
                            std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                            std::cin >> BookID;
                        }
                        BookID = IDconvert(BookID, NumBookID);
                        BookList.modifyBook(BookID);
                        Borrower.modifyBookNameForAll(BookID, BookList.findBookNameByID(BookID));
                        Bookborrows.ModifyNameBook(BookID, BookList.findBookNameByID(BookID));
                    }
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 4:
                    std::system("cls");
                    title();
                    if (!BookList.checkEmpty())
                    {
                        std::cout << "Arranging........." << std::endl;
                        BookList.arrange();
                        std::cout << "Successful Arrangement!" << std::endl;
                    }                    
                    Sleep(1000);
                    std::system("cls");
                    break;
                case 5:
                    std::system("cls");
                    title();
                    if (!BookList.checkEmpty())
                    {
                        BookList.printBookList();
                        std::cout << "Enter Book ID You Want to Find Data : ";
                        std::cin >> BookID;
                        while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxBookID))))
                        {
                            std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                            std::cin >> BookID;
                        }
                        BookID = IDconvert(BookID, NumBookID);
                        BookList.PrintOneBookbyID(BookID);
                    }
                    std::system("pause");
                    std::system("cls");
                    break;
                case 6:
                    std::system("cls");
                    title();
                    if (!BookList.checkEmpty())
                    {
                        BookList.print();
                    }
                    std::system("pause");
                    std::system("cls");
                    break;
                case 7:
                    back = true;
                    break;
                default:
                    std::cout << "You choice is wrong, please enter again.";
                    Sleep(3000);
                    std::system("cls");
                    break;
                }
            }
            std::system("cls");
            break;
        case 2:
            back = false;
            while (back == false)
            {
                std::system("cls");
                title();
                std::cout << "Choose the function of Book Manager : " << std::endl;
                std::cout << "1.Add Reader." << std::endl;
                std::cout << "2.Remove Reader." << std::endl;
                std::cout << "3.Modify Reader." << std::endl;
                std::cout << "4.Arrange Reader List." << std::endl;
                std::cout << "5.Find Reader." << std::endl;
                std::cout << "6.Print Reader list." << std::endl;
                std::cout << "7.Exit" << std::endl;
                std::cout << "Enter your choice : ";
                std::cin >> choice;
                switch (choice)
                {
                case 1:
                    std::system("cls");
                    title();
                    ReaderList.addReader();
                    std::system("cls");
                    break;
                case 2:
                    std::system("cls");
                    title();
                    std::cout << "Enter Reader ID You Want to Remove : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    ReaderList.removeReader(ReaderID);

                    for (int i = 0; i < Borrower.findBorrowerBookNum(ReaderID); i++)
                    {

                        Bookborrows.ModifyNumBorrrowNode(Borrower.getBookIDofBorrower(ReaderID, i), false);
                    }
                    Borrower.removeBorrower(ReaderID);
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 3:
                    std::system("cls");
                    title();
                    std::cout << "Enter Reader ID You Want to Modify : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    std::cin.ignore();
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    ReaderList.modifyReader(ReaderID);
                    Borrower.modifyBorrower(ReaderID, ReaderList.getReaderName(ReaderID));
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 4:
                    std::system("cls");
                    title();
                    std::cout << "Arranging........." << std::endl;
                    ReaderList.arrangeByID();
                    std::cout << "Successful Arrangement!" << std::endl;
                    Sleep(1000);
                    std::system("cls");
                    break;
                case 5:
                    std::system("cls");
                    title();
                    std::cout << "Enter Reader ID You Want to Find Data : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    ReaderList.findReader(ReaderID);
                    std::system("pause");
                    std::system("cls");
                    break;
                case 6:
                    std::system("cls");
                    title();
                    ReaderList.printList();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 7:
                    back = true;
                    break;
                default:
                    std::cout << "You choice is wrong, please enter again.";
                    Sleep(3000);
                    std::system("cls");
                    break;
                }
            }
            std::system("cls");
            break;
        case 3:
            back = false;
            while (back == false)
            {
                std::system("cls");
                title();
                std::cout << "Choose the function of Book Manager : " << std::endl;
                std::cout << "1.Add Borrower." << std::endl;
                std::cout << "2.Remove Borrower." << std::endl;
                std::cout << "3.Arrange Borrower List." << std::endl;
                std::cout << "4.Find Borrower." << std::endl;
                std::cout << "5.Print Borrower list." << std::endl;
                std::cout << "6.Add Book Borrowed." << std::endl;
                std::cout << "7.Remove Book Borrowed." << std::endl;
                std::cout << "8.Print Borrower has overdue books." << std::endl;
                std::cout << "9.Exit" << std::endl;
                std::cout << "Enter your choice : ";
                std::cin >> choice;
                switch (choice)
                {
                case 1:
                    std::system("cls");
                    title();
                    std::cout << "Enter Reader ID : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    if (!ReaderList.CheckID(ReaderID))
                    {
                        std::cout << "The Reader ID does not exist" << std::endl;
                    }
                    else
                    {
                        Borrower.addBorrower(ReaderID, ReaderList.getReaderName(ReaderID));
                    }
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 2:
                    std::system("cls");
                    title();
                    std::cout << "Enter Borrower ID You Want to Remove : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    Borrower.removeBorrower(ReaderID);
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 3:
                    std::system("cls");
                    title();
                    std::cout << "Arranging........." << std::endl;
                    Borrower.arrangeByID();
                    std::cout << "Successful Arrangement!" << std::endl;
                    Sleep(1000);
                    std::system("cls");
                    break;
                case 4:
                    std::system("cls");
                    title();
                    std::cout << "Enter Reader ID You Want to Find Data : ";
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    Borrower.findAndPrintData(ReaderID);
                    std::system("pause");
                    std::system("cls");
                    break;
                case 5:
                    std::system("cls");
                    title();
                    Borrower.displayList();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 6:
                    std::system("cls");
                    title();
                    BookList.printBookList();
                    std::cout << "Enter Borrower ID" << std::endl;
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    if (!Borrower.checkBorrowerID(ReaderID))
                    {
                        std::cout << "The Borrower ID does not exist" << std::endl;
                    }
                    else {
                        std::cout << "Enter Book ID" << std::endl;
                        std::cin >> BookID;
                        while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxReaderID))))
                        {
                            std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                            std::cin >> BookID;
                        }
                        BookID = IDconvert(BookID, NumBookID);
                        if (!BookList.CheckID(BookID))
                        {
                            std::cout << "The Book ID does not exist" << std::endl;
                        }
                        else
                        {
                            if (Bookborrows.getNumborrow(BookID) < BookList.findBooktotal(BookID))
                            {
                                do {
                                    std::cout << "Enter Date Expired : ";
                                    std::cin >> temp;
                                    if (!isDateValid(temp))
                                    {
                                        std::cout << "The Date Expired You Enter is Wrong, Please Enter Again";
                                    }
                                } while (!isDateValid(temp));
                                Borrower.addBookForBorrower(ReaderID, BookID, BookList.findBookNameByID(BookID), currenttime(true), temp);
                                Bookborrows.ModifyNumBorrrowNode(BookID, true);
                            }
                            else
                            {
                                std::cout << "The Book "<<BookList.findBookNameByID(BookID)<< " is over." << std::endl;
                            }
                        }
                    }
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 7:
                    std::system("cls");
                    title();
                    BookList.printBookList();
                    std::cout << "Enter Borrower ID" << std::endl;
                    std::cin >> ReaderID;
                    while ((compareNumString(ReaderID, "0")) || (!compareNumString(ReaderID, std::to_string(MaxReaderID))))
                    {
                        std::cout << "Reader ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> ReaderID;
                    }
                    ReaderID = IDconvert(ReaderID, NumReaderID);
                    if (!Borrower.checkBorrowerID(ReaderID))
                    {
                        std::cout << "The Borrower ID does not exist" << std::endl;
                    }
                    else {
                        std::cout << "Enter Book ID You Want to Remove : ";
                        while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxReaderID))))
                        {
                            std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                            std::cin >> BookID;
                        }
                        BookID = IDconvert(ReaderID, NumReaderID);
                        if (!BookList.CheckID(BookID))
                        {
                            std::cout << "The Book ID does not exist" << std::endl;
                        }
                        else
                        {
                            Borrower.removeBookForBorrower(ReaderID, BookID);
                            Bookborrows.ModifyNumBorrrowNode(BookID, false);
                        }
                    }
                    Sleep(2000);
                    std::system("cls");
                    break;
                case 8:
                    std::system("cls");
                    title();
                    Borrower.printExpiredBorrower();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 9:
                    back = true;
                    break;
                default:
                    std::cout << "You choice is wrong, please enter again.";
                    Sleep(3000);
                    std::system("cls");
                    break;
                }
            }
            std::system("cls");
            break;
        case 4:
            back = false;
            while (back == false)
            {
                std::system("cls");
                title();
                std::cout << "Choose the function of Book Manager : " << std::endl;
                std::cout << "1.Check the number borrow of book" << std::endl;
                std::cout << "2.Find the book most borrowed" << std::endl;
                std::cout << "3.Find the book borrowed at leastr." << std::endl;
                std::cout << "4.Print Book Borrowed List." << std::endl;
                std::cout << "5.Exit" << std::endl;
                std::cout << "Enter your choice : ";
                std::cin >> choice;
                switch (choice)
                {
                case 1:
                    std::system("cls");
                    title();
                    std::cout << "Enter Book ID" << std::endl;
                    std::cin >> BookID;
                    while ((compareNumString(BookID, "0")) || (!compareNumString(BookID, std::to_string(MaxBookID))))
                    {
                        std::cout << "Book ID You Enter is Wrong, Please Enter Again : ";
                        std::cin >> BookID;
                    }
                    BookID = IDconvert(BookID, NumBookID);
                    Bookborrows.findNodeByIDBook(BookID);

                    Sleep(2000);
                    break;
                case 2:
                    std::system("cls");
                    title();
                    Bookborrows.findMaxNumBorrow();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 3:
                    std::system("cls");
                    title();
                    Bookborrows.findMinNumBorrow();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 4:
                    std::system("cls");
                    title();
                    Bookborrows.printTree();
                    std::system("pause");
                    std::system("cls");
                    break;
                case 5:
                    back = true;
                    break;
                default:
                    std::cout << "You choice is wrong, please enter again.";
                    Sleep(3000);
                    std::system("cls");
                    break;
                }
            }
            std::system("cls");
            break;
        case 5:
            std::cout << "Goodbye! See you again.";
            Sleep(3000);
            exit = true;
            break;
        default:
            std::cout << "You choice is wrong, please enter again.";
            Sleep(3000);
            std::system("cls");
            break;
        }
    }

    BookList.saveDataToFile(bookdata);
    Bookborrows.saveToFile(bookborrowdata);
    ReaderList.saveDataToFile(readerdata);
    Borrower.saveDataToFile(borrowerdata);
}
