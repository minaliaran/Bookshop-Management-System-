// Standard library headers used in this program:
// <iostream>   - for input/output stream operations (cin, cout)
// <fstream>    - for file handling (reading/writing files)
// <iomanip>    - for output formatting (e.g., setw, setprecision)
// <string>     - for string manipulation
// <limits>     - for numeric limits (e.g., clearing input buffer)
// <sstream>    - for string stream operations (converting strings to numbers)
// <cstdlib>    - for general utilities (e.g., system calls, random numbers)
// <algorithm>  - for common algorithms (e.g., sorting, searching)
// <ctime>      - for date and time functions (e.g., current time)

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <ctime> 


using namespace std;
const string filePath = "C:\\Users\\Public\\Documents\\Bookshop.zip\\Book shop";

string toLowerCase(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Function declarations
void addBook();
void viewBooks();
void updateBook();
void deleteBook();
void manageBook();
void searchBook();
void purchaseBook();
void manageSales();
void deleteSale();
void displayBooks();
void recordSale();
void viewSales();
void viewMonthlySales();
void viewCompanyDetails();
void exitSystem();
void clearInput();

int main() {
    int choice, id;
    string name;

    // Get user name with validation
    do {
        cout << "\n\n======= Welcome to Genius Books Management System =======\n\n\n";
        cout << "Enter your name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Name cannot be empty. Please try again.\n";
        }
    } while (name.empty());

    // Get user ID with validation
    do {
        cout << "Enter your ID (must be >= 1000): ";
        cin >> id;

        if (cin.fail() || id < 1000) {
            cout << "Invalid ID. Please enter a number >= 1000.\n";
            clearInput();
            id = 0; // reset id to ensure loop continues if invalid
        } else {
            clearInput(); // clear newline from input buffer
        }
    } while (id < 1000);

    cout << "\n===== Welcome, " << name << "! =====\n";

    // Main menu loop
    while (true) {
        cout << "\n======= System Menu =======\n\n";
        cout << "1. Manage Book Details\n";
        cout << "2. Manage Sales\n";
        cout << "3. Search Books\n";
        cout << "4. Purchase Books\n";
        cout << "5. View Company Details\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

      // Check if input is valid (must be an integer)
            if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }

        switch (choice) {
           
            case 1: manageBook(); break;
            case 2: manageSales(); break;
            case 3: searchBook(); break;
            case 4: purchaseBook(); break;
            case 5: viewCompanyDetails(); break;
            case 6: exitSystem();
 
            default:
                cout << "Invalid choice. Please choose from the menu.\n";
        }
    }

    return 0; // Technically unreachable but good practice
}

// Clears input buffer after invalid input
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard remaining input
}
void addBook() {
    
     system("cls");// Clears previous content for focused display
    int bookid, quantity;
    string title, author;
    double price;

    cout << "\n\n====== Add Book to GENIUS BOOKS Inventory ======\n\n";

    cout << "Enter Book ID: ";
    cin >> bookid;
    if (cin.fail() || bookid <= 0) {
        cout << "Book ID must be a positive number.\n";
        clearInput();
        return;
    }
    cin.ignore(); // clear leftover newline

    cout << "Enter Book Title: ";
    getline(cin, title);

    cout << "Enter Book Author: ";
    getline(cin, author);

    cout << "Enter Book Quantity: ";
    cin >> quantity;
    if (cin.fail() || quantity <= 0) {
        cout << "Quantity must be a positive number.\n";
        clearInput();
        return;
    }

    cout << "Enter Book Price: $ ";
    cin >> price;
    if (cin.fail() || price <= 0) {
        cout << "Price must be a positive number.\n";
        clearInput();
        return;
    }
    cin.ignore(); // clear buffer again just in case

    ofstream file(filePath, ios::app);
    if (!file) {
        cout << "Failed to open file. Please check the path.\n";
        return;
    }

    file << bookid << "|" << title << "|" << author << "|" << quantity << "|" << price << "\n";
    file.close();

    cout << "\nBook added successfully!\n";
    system("pause");  // Pauses before returning to menu
}

// Function to display all available book details
void viewBooks() {

    system("cls");
    // Clears previous content for focused display
    ifstream file(filePath);
    if (!file) {
        cout << "Unable to open the book file.\n";
        return;
    }

    int id, quantity, count = 0;
    double price;
    string title, author, line;

    cout << "\n====== View Books in GENIUS BOOKS System ======\n\n";
    cout << "------------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "ID"
         << "| " << setw(25) << "Title"
         << "| " << setw(20) << "Author"
         << "| " << setw(10) << "Qty"
         << "| " << setw(10) << "Price $"
         << endl;
    cout << "------------------------------------------------------------------------------------------\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, qtyStr, priceStr;

        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');

        if (idStr.empty() || title.empty() || author.empty() || qtyStr.empty() || priceStr.empty()) {
            continue;
        }

        try {
            id = stoi(idStr);
            quantity = stoi(qtyStr);
            price = stod(priceStr);
        } catch (...) {
            continue;
        }

        cout << left << setw(10) << id
             << "| " << setw(25) << title
             << "| " << setw(20) << author
             << "| " << setw(10) << quantity
             << "| " << setw(10) << price 
             << endl;
        count++;
    }

    if (count == 0) {
        cout << "\nNo records found in the inventory.\n";
    }

    file.close();
    system("pause");
}



void updateBook() {
    
      system("cls");
    ifstream inFile(filePath);
    ofstream tempFile("temp.txt");

    if (!inFile || !tempFile) {
        cout << "Error opening file(s)." << endl;
        return;
    }

    string line, idStr, title, author, qtyStr, priceStr;
    int targetId, bookid, quantity;
    double price;
    bool found = false;

    cout << "\nEnter Book ID to update: ";
    while (!(cin >> targetId)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a valid Book ID: ";
    }
    cin.ignore();

    while (getline(inFile, line)) {
        stringstream ss(line);
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');

        if (idStr.empty() || title.empty() || author.empty() || qtyStr.empty() || priceStr.empty())
            continue;

        try {
            bookid = stoi(idStr);
            quantity = stoi(qtyStr);
            price = stod(priceStr);
        } catch (...) {
            continue;
        }

        if (bookid == targetId) {
            found = true;

            cout << "Enter new Title: ";
            getline(cin, title);

            cout << "Enter new Author: ";
            getline(cin, author);

            cout << "Enter new Quantity: ";
            while (!(cin >> quantity) || quantity < 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Enter a positive number: ";
            }

            cout << "Enter new Price: $ ";
            while (!(cin >> price) || price < 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Enter a valid price: ";
            }

            cin.ignore();  // Clean up newline character
        }

        tempFile << bookid << "|" << title << "|" << author << "|" << quantity << "|" 
                 << fixed << setprecision(2) << price << "\n";
    }

    inFile.close();
    tempFile.close();

    remove(filePath.c_str());
    rename("temp.txt", filePath.c_str());

    if (found)
        cout << " Book updated successfully.\n";
    else
        cout << " Book ID not found.\n";
       system("pause");
}


void deleteBook() {
    system("cls");
    string line;
    int bookid, targetId, quantity;
    string title, author;
    double price;
    bool found = false;

    cout << "\nEnter Book ID to delete: ";
    cin >> targetId;

    ifstream inFile(filePath);
    ofstream tempFile("temp.txt");

    if (!inFile || !tempFile) {
        cout << "Error opening file.\n";
        return;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, qtyStr, priceStr;

        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');

        if (idStr.empty() || title.empty() || author.empty() || qtyStr.empty() || priceStr.empty()) {
            continue;
        }

        try {
            bookid = stoi(idStr);
            quantity = stoi(qtyStr);
            price = stod(priceStr);
        } catch (...) {
            continue;
        }

        if (bookid != targetId) {
            tempFile << bookid << "|" << title << "|" << author << "|" << quantity << "|" << price << "\n";
        } else {
            found = true;
        }
    }

    inFile.close();
    tempFile.close();

    remove(filePath.c_str());
    rename("temp.txt", filePath.c_str());

    if (found)
        cout << "Book deleted successfully.\n";
    else
        cout << "Book ID not found.\n";
       system("pause");
}

// Function to add, update, or delete book details
void manageBook() {
     system("cls");
    int choice;
    bool stayInBookMenu = true;

    while (stayInBookMenu) {
        cout << "\n====== Manage Books ======\n";
        cout << "1. View Book Details\n";
        cout << "2. Add New Books\n";
        cout << "3. Update Books\n";
        cout << "4. Delete Books\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:viewBooks();break;
            case 2:addBook();break;
            case 3:updateBook();break;
            case 4:deleteBook();break;
            case 5: {
                char confirm;
                cout << "Are you sure you want to return to Main Menu? (Y/N): ";
                cin >> confirm;
                if (confirm == 'Y' || confirm == 'y') {
                    cout << "Returning to Main Menu...\n";
                    stayInBookMenu = false;
                } else {
                    cout << "Staying in Book Menu...\n";
                    // stayInBookMenu stays true, loop continues
                }
                break;
            }
            default:
                cout << "Invalid choice! Please try again.\n";
                system("pause");
        }
    }
}


    // Function to search for books by title
void searchBook() {
      system("cls");
    // Ensure the file exists before trying to open it
    ofstream createFile(filePath, ios::app); // creates file if it doesn't exist
    createFile.close();

    ifstream file(filePath);
    if (!file.is_open()) {
        cout << " Unable to open book file at path: " << filePath << endl;
        return;
    }

    string title;
    cout << "\nEnter Book Title to search: ";
    cin.ignore();
    getline(cin, title);

    string searchKey = toLowerCase(title);

    string line, idStr, titleStr, authorStr, quantityStr, priceStr;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, idStr, '|');
        getline(ss, titleStr, '|');
        getline(ss, authorStr, '|');
        getline(ss, quantityStr, '|');
        getline(ss, priceStr, '|');

        if (toLowerCase(titleStr).find(searchKey) != string::npos) {
            found = true;
            cout << "\n Book Found:\n";
            cout << "ID: " << idStr << "\n";
            cout << "Title: " << titleStr << "\n";
            cout << "Author: " << authorStr << "\n";
            cout << "Quantity: " << quantityStr << "\n";
            cout << "Price: $ " << priceStr << "\n";
            break;
        }
    }

    if (!found) {
        cout << "Book not found.\n";
    }

    file.close();
}
float getPriceFromFile(string bookID) {
    // Dummy function — replace with your actual file lookup
    if (bookID == "B101") return 150.0;
    if (bookID == "B102") return 200.0;
    return 100.0; // default price if not found
    system("pause");
}

// Function to process book purchases (stock replenishment)

void purchaseBook() {
     system("cls");
    string purchaserName;
    cout << "Enter purchaser's name: ";
    cin.ignore();
    getline(cin, purchaserName);

    double totalPrice = 0.0;
    char choice = 'Y';

    while (choice == 'Y' || choice == 'y') {
        ifstream file(filePath);
        ofstream temp("temp.txt");

        if (!file || !temp) {
            cout << "Error opening files.\n";
            return;
        }

        string targetId, line;
        bool found = false;

        cout << "Enter Book ID to purchase: ";
        cin >> targetId;

        int purchaseQty;
        cout << "Enter quantity to purchase: ";
        while (!(cin >> purchaseQty) || purchaseQty <= 0) {
            cout << "Invalid quantity. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore();

        while (getline(file, line)) {
            stringstream ss(line);
            string id, title, author, quantityStr, priceStr;

            getline(ss, id, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, quantityStr, '|');
            getline(ss, priceStr, '|');

            if (id == targetId) {
                found = true;
                int availableQty = stoi(quantityStr);

                if (purchaseQty > availableQty) {
                    cout << "Insufficient stock for book " << id << ". Available: " << availableQty << "\n";
                    temp << line << "\n"; // write original line
                } else {
                    int newQty = availableQty - purchaseQty;
                    totalPrice += purchaseQty * stod(priceStr);
                    cout << "Added " << purchaseQty << " copies of " << title << " to cart. Price per book: $" << priceStr << "\n";
                    temp << id << "|" << title << "|" << author << "|" << newQty << "|" << priceStr << "\n";
                }
            } else {
                temp << line << "\n";
            }
        }

        file.close();
        temp.close();

        remove(filePath.c_str());
        rename("temp.txt", filePath.c_str());

        if (!found) {
            cout << "Book ID not found.\n";
            char retry;
            cout << "Do you want to try again? (Y/N): ";
            cin >> retry;
            cin.ignore();

            if (retry != 'Y' && retry != 'y') {
                cout << "Thank you.\n";
                break; // Exit the purchase loop
            } else {
                continue; // Retry from the top of the loop
            }
        }

        cout << "Add another book? (Y/N): ";
        cin >> choice;
        cin.ignore();  // to avoid skipping input next time
    }

    if (totalPrice > 0.0) {
        cout << "\n===== PURCHASE RECEIPT =====\n";
        cout << "Purchaser: " << purchaserName << endl;
        cout << "Total amount to pay: $" << fixed << setprecision(2) << totalPrice << endl;
        cout << "Thank you for your purchase!\n";
    } else {
        cout << "\nNo valid purchases were made. No receipt generated.\n";
        system("pause");
    }
}

// Function to manage sales transactions and record them
void manageSales() {
    
     system("cls");
    int menuChoice;
    bool stayInSalesMenu = true;

    while (stayInSalesMenu) {
        cout << "\n=============================\n";
        cout << "        SALES MANAGEMENT      \n";
        cout << "=============================\n";
        cout << "1. Record a New Sale\n";
        cout << "2. View All Sales Records\n";
        cout << "3. View Monthly Sales Report\n";
        cout << "4. Delete a Sale\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice  ";
        cin >> menuChoice;

        switch (menuChoice) {
            case 1: recordSale();break;
            case 2: viewSales();break;
            case 3: viewMonthlySales();break;
            case 4: deleteSale();break;
            case 5: {  
                char confirm;
                cout << "\nAre you sure you want to return to the Main Menu? (Y/N): ";
                cin >> confirm;
                if (confirm == 'Y' || confirm == 'y') {
                    cout << "Returning to Main Menu...\n";
                    stayInSalesMenu = false;
                } else {
                    cout << "No problem! Staying in Sales Menu.\n";
                }
                break;
            }
            default:
                cout << "Invalid option. Please choose between 1 and 5.\n";
                system("pause");
        }
    }
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

void deleteSale() {

     system("cls");
    ifstream inFile("sales.txt");
    ofstream tempFile("temp_sales.txt");

    if (!inFile || !tempFile) {
        cout << "Error opening sales file.\n";
        return;
    }

    string targetId;
    cout << "\nEnter Sale ID to delete: ";
    cin.ignore();
    getline(cin, targetId);
    targetId = trim(targetId);  // Clean user input

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string date, saleId;

        // Read first two fields: date and saleId
        getline(ss, date, '|');      // Skip date
        getline(ss, saleId, '|');    // Get saleId
        saleId = trim(saleId);

        if (saleId != targetId) {
            tempFile << line << endl;
        } else {
            found = true;
        }
    }

    inFile.close();
    tempFile.close();

    remove("sales.txt");
    rename("temp_sales.txt", "sales.txt");

    if (found)
        cout << " Sale deleted successfully.\n";
    else
        cout << " Sale ID not found.\n";
       system("pause");
}
void recordSale() {

     system("cls");
    int saleid, bookid, quantitysold;
    double saleprice;
    string date;

    cout << "\n====== Record New Sale ======\n\n";
    cin.ignore();
    cout << "Enter Date (DD/MM/YYYY): ";
    getline(cin, date);
    cout << "Enter Sale ID: ";
    cin >> saleid;
    cout << "Enter Book ID: ";
    cin >> bookid;
    cout << "Enter Quantity Sold: ";
    cin >> quantitysold;
    cout << "Enter Sale Price: ";
    cin >> saleprice;

    double total = quantitysold * saleprice;

    ofstream salesFile("sales.txt", ios::app);
    if (!salesFile) {
        cout << "Error opening sales file.\n";
        return;
    }

    salesFile << date << "|" << saleid << "|" << bookid << "|" << quantitysold << "|" << saleprice << "|" << total << "\n";
    salesFile.close();

    cout << "Sale recorded successfully!\n";
   system("pause");
}



void viewSales() {

     system("cls");
    ifstream salesFile("sales.txt");
    if (!salesFile) {
        cout << "No sales records found.\n";
        return;
    }

    string line;
    cout << "\n===============  Sales Records  ===============\n";
    cout << left << setw(12) << "Date"
         << setw(10) << "SaleID"
         << setw(10) << "BookID"
         << setw(12) << "Qty Sold"
         << setw(12) << "Price"
         << setw(12) << "Total" << "\n";
    cout << string(68, '-') << "\n";

    while (getline(salesFile, line)) {
        if (line.empty()) continue; // Skip blank lines

        string date, saleid, bookid;
        int qty;
        double price, total;

        stringstream ss(line);
        getline(ss, date, '|');
        getline(ss, saleid, '|');
        getline(ss, bookid, '|');
        string qtyStr, priceStr, totalStr;
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');
        getline(ss, totalStr, '|');

          

        // Attempt to convert quantity, price, and total strings to numeric values.
       // If any conversion fails, catch the exception, notify, and skip the invalid line.
        try {
            qty = stoi(qtyStr);
            price = stod(priceStr);
            total = stod(totalStr);
        } catch (...) {
            cout << "Skipping invalid line: " << line << "\n";
            continue;
        }

        cout << left << setw(12) << date
             << setw(10) << saleid
             << setw(10) << bookid
             << setw(12) << qty
             << setw(12) << fixed << setprecision(2) << price
             << setw(12) << fixed << setprecision(2) << total << "\n";
    }

    cout << string(68, '-') << "\n";
    salesFile.close();
    system("pause");
}

void viewMonthlySales() {

     system("cls");
    ifstream salesFile("sales.txt");
    if (!salesFile) {
        cout << "No sales records found.\n";
        return;
    }

    string year, month;
    cout << "=== View Monthly Sales ===\n";
    cout << "Enter year (YYYY): ";
    cin >> year;
    cout << "Enter month (MM): ";
    cin >> month;

    // Pad month with 0 if needed
    if (month.length() == 1) month = "0" + month;

    string line;
    bool found = false;

    cout << "\n======= Sales for " << year << "-" << month << " =======\n";
    cout << left << setw(12) << "Date"
         << setw(10) << "SaleID"
         << setw(10) << "BookID"
         << setw(12) << "Qty Sold"
         << setw(12) << "Price"
         << setw(12) << "Total" << "\n";
    cout << string(68, '-') << "\n";

    while (getline(salesFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string date, saleid, bookid, qtyStr, priceStr, totalStr;

        getline(ss, date, '|');
        getline(ss, saleid, '|');
        getline(ss, bookid, '|');
        getline(ss, qtyStr, '|');
        getline(ss, priceStr, '|');
        getline(ss, totalStr, '|');

        if (date.length() < 10) continue;

        // Expecting DD/MM/YYYY format, extract month and year
        string fileDay = date.substr(0, 2);
        string fileMonth = date.substr(3, 2);
        string fileYear = date.substr(6, 4);

        if (fileYear == year && fileMonth == month) {
            int qty;
            double price, total;

            try {
                qty = stoi(qtyStr);
                price = stod(priceStr);
                total = stod(totalStr);
            } catch (...) {
                cout << "Skipping invalid line: " << line << "\n";
                continue;
            }

            found = true;
            cout << left << setw(12) << date
                 << setw(10) << saleid
                 << setw(10) << bookid
                 << setw(12) << qty
                 << setw(12) << fixed << setprecision(2) << price
                 << setw(12) << fixed << setprecision(2) << total << "\n";
        }
    }

    if (!found) {
        cout << "No sales records found for " << year << "-" << month << ".\n";
    }

    cout << string(68, '-') << "\n"; // Print a horizontal separator line of 68 dashes for visual clarity
    salesFile.close();
    system("pause");
}

 

void exitSystem() {

     system("cls");
    char confirm;
    cout << "\nAre you sure you want to exit? (Y/N): ";
    cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        cout << "\nThank you for using Genius Books Management System. Goodbye!\n";
        cout << "Press Enter to exit the program...";
        cin.ignore(); // clear newline from previous input
        cin.get();    // wait for user to press Enter
        exit(0);      // Now safely exit
    } else {
        cout << "\nExit cancelled. Returning to main menu...\n";
        system("pause");
    }
}


// Function to view company information such as address, contact, etc.
void viewCompanyDetails() {

     system("cls");
    cout << "\n======= Company Details =======\n";
    cout << "Company Name : Genius Books\n";
    cout << "Founded      : 2020\n";
    cout << "Location     : Colombo, Sri Lanka\n";
    cout << "Specialty    : Educational & Fiction Books\n";
    cout << "Mission      : To spread the joy of reading\n";
    cout << "Contact      : info@geniusbooks.com\n";
    system("pause");
}