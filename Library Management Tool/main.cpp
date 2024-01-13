/*Code Written and maintained by Harsh Santoshi*/
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip> 
using namespace std;

class Book{
public:
    int ISBN;
    string title;
    string author;
    int quantity;
    int price;
    Book() {}
    Book(int ISBN, const string& title, const string& author, int price, int quantity) {
        this->ISBN = ISBN;
        this->title = title;
        this->author = author;
        this->price = price;
        this->quantity = quantity;
    }
    
};
class Borrower{
public:
    int borrowerId;
    string borrowerName;
    string borrowerMobileNo;
    Borrower(){}

    Borrower(int borrowerId , const string & borrowerName , const string & borrowerMobileNo){
        this->borrowerId = borrowerId;
        this->borrowerName = borrowerName;
        this->borrowerMobileNo = borrowerMobileNo;
        
    }
};

class Transaction {
    public:
    bool isPaid = false;
    int calculateFine(const string &borrowedDate){
        tm tm1 = {}, tm2 = {};
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string currDate = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
        
        if (sscanf(borrowedDate.c_str(), "%d/%d/%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year) != 3 ||
            sscanf(currDate.c_str(), "%d/%d/%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year) != 3) {
            return -1; 
        }

        tm1.tm_mon -= 1;
        tm1.tm_year -= 1900;
        tm2.tm_mon -= 1;
        tm2.tm_year -= 1900;
        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);

        double difference = difftime(time2, time1);

        // Converting seconds to days
        int daysDifference = static_cast<int>(difference / (24 * 60 * 60));
        int fine = 0;
        if (daysDifference > 7) {
            fine = (daysDifference - 7) * 2;
        }
        return fine;
    }
    void finePaid(int borrowerID){

    }  
};

class Library {
public:
    // Database to borrowers with their books
    map<int, set<pair<int,string>>> borrowerBooks;

    // Database to store all books in the store
    set<int> allBooks;

    // Books information mapped by its ISBN
    map<int, Book> bookInformation;
    //store all borrowers
    set<int>allBorrowers;
    //store borrowers details mapped by their ID
    map<int, Borrower> borrowerInformation;
    //store borrowers Fine by their ID
    map<int,int>borrowersFine;
    set<int>allFines;

    // File name for storing book information
    const string bookDatabase = "book_database.txt";
    //File name for storing borrower information
    const string borrowerDatabase = "borrower_database.txt";

    // File name for storing issued books information
    const string issuedBooksDatabase = "issued_books.txt";

    // File name for storing borrowers' fines
    const string borrowersFineDatabase = "borrowers_fine.txt";


    Library() {
        // Load book information from the file on program start
        loadBookData();
        //Load borrower information from the file
        loadBorrowerData();
        //load issued books information
        loadIssuedBooksData();
        // load borrowers' fines
        loadBorrowersFineData();
    }

    void addBookToLibrary(int ISBN, const string& title, const string& author, int price, int quantity) {
        if (allBooks.find(ISBN) != allBooks.end()) {
            cout << "<----------Book with the same ISBN already exists----------->" << endl;
            return;
        }

        Book newBook(ISBN, title, author, price, quantity);
        bookInformation[ISBN] = newBook;
        allBooks.insert(ISBN);
    }

    void getAllBooksDetails() {
        int bookNumber = 1;
        for (auto& bookPair : bookInformation) {
            cout << "<----------Book " << bookNumber++ << " details:--------------->" << endl;
            Book& detail = bookPair.second;
            cout << "Book's ISBN: " << detail.ISBN << endl;
            cout << "Book's Title: " << detail.title << endl;
            cout << "Book's Author: " << detail.author << endl;
            cout << "Book's Price: " << detail.price << endl;
            cout << "Book's Quantity: " << detail.quantity << endl;
            cout << endl;
        }
    }

    void searchByISBN(int providedISBN) {
        if (allBooks.find(providedISBN) == allBooks.end()) {
            cout << "NO SUCH BOOK EXISTS!!!" << endl;
            return;
        }
        cout << "Book Details are : " << endl;
        Book& detail = bookInformation[providedISBN];
        cout << "Book's Title is : " << detail.title << endl;
        cout << "Book's Author is : " << detail.author << endl;
        cout << "Book's Price is : " << detail.price << endl;
        cout << "Remaining Book's Quantity is : " << detail.quantity << endl;
        cout << endl;
    }

    void searchByTitle(const string& providedTitle) {
        bool found = false;
        for (auto& book : bookInformation) {
            Book& detail = book.second;
            if (detail.title == providedTitle) {
                found = true;
                cout << "Book Details are : " << endl;
                cout << "Book's ISBN is : " << detail.ISBN << endl;
                cout << "Book's Title is : " << detail.title << endl;
                cout << "Book's Author is : " << detail.author << endl;
                cout << "Book's Price is : " << detail.price << endl;
                cout << "Remaining Book's Quantity is : " << detail.quantity << endl;
                cout << endl;
            }
        }

        if (!found) {
            cout << "NO SUCH BOOK EXISTS!!!" << endl;
        }
    }

    void searchByAuthor(const string& providedAuthor) {
        bool found = false;
        for (auto& book : bookInformation) {
            Book& detail = book.second;
            if (detail.author == providedAuthor) {
                found = true;
                cout << "Book Details are : " << endl;
                cout << "Book's ISBN is : " << detail.ISBN << endl;
                cout << "Book's Title is : " << detail.title << endl;
                cout << "Book's Author is : " << detail.author << endl;
                cout << "Book's Price is : " << detail.price << endl;
                cout << "Remaining Book's Quantity is : " << detail.quantity << endl;
                cout << endl;
            }
        }

        if (!found) {
            cout << "NO SUCH BOOK EXISTS!!!" << endl;
        }
    }

    // Save book information to the file
    void saveBookData() {
        ofstream file(bookDatabase);
        if (file.is_open()) {
            for (const auto& bookPair : bookInformation) {
                const Book& book = bookPair.second;
                file << book.ISBN << "\t" << book.title << "\t" << book.author << "\t" << book.price << "\t" << book.quantity << "\n";
            }
            file.close();
            cout << "Book data saved successfully!" << endl;
        } else {
            cerr << "Unable to save library data to file!" << endl;
        }
    }

    // Load book information from the file
    void loadBookData() {
        ifstream file(bookDatabase);
        if (file.is_open()) {
            int ISBN, price, quantity;
            string title, author;
            file >> std::ws;
            file.clear(); 
            file.seekg(0); 

            while (file >> ISBN >> std::ws) {
                getline(file, title, '\t');
                getline(file, author, '\t');
                file >> price >> quantity;
                addBookToLibrary(ISBN, title, author, price, quantity);
            }

            file.close();
            cout << "Book data loaded successfully!" << endl;
        } else {
            cerr << "Error opening file: " << bookDatabase << endl;
        }
    }

    //add borrower to set
    void saveBorrower(int borrowerId , const string &borrowerName , const  string & borrowerMobileNo){
        Borrower newBorrower(borrowerId, borrowerName, borrowerMobileNo);
        borrowerInformation[borrowerId] = newBorrower ;
        allBorrowers.insert(borrowerId);
    }

    //issue the book
    void issueBookToBorrower(int ISBN , int borrowerID , bool& alreadyIssued){
        for(auto borrowerPair : borrowerBooks[borrowerID]){
            if(borrowerPair.first == ISBN){
                cout<<"Book is already issued to "<< borrowerInformation[borrowerID].borrowerName<<endl;
                alreadyIssued = true;
                return ;
            }
        }
        time_t now = time(0);

        tm *ltm = localtime(&now);
        string DateofBorrowed = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);

        borrowerBooks[borrowerID].insert({ISBN , DateofBorrowed});
    }

    void saveBorrowerData() {
        ofstream file(borrowerDatabase);
        if (file.is_open()) {
            for (const auto& borrowerPair : borrowerInformation) {
                const Borrower& borrower = borrowerPair.second;
                file << borrower.borrowerId << "\t" << borrower.borrowerName << "\t" << borrower.borrowerMobileNo << "\n";
            }
            file.close();
            cout << "Borrower data saved successfully!" << endl;
        } else {
            cerr << "Unable to save borrower data to file!" << endl;
        }
    }

    // Save issued books information to the file
    void saveIssuedBooksData() {
        ofstream file(issuedBooksDatabase);
        if (file.is_open()) {
            for (const auto& borrowerPair : borrowerBooks) {
                int borrowerId = borrowerPair.first;
                const set<pair<int, string>>& books = borrowerPair.second;

                for (const auto& book : books) {
                    int ISBN = book.first;
                    const string& dateBorrowed = book.second;
                    file << borrowerId << "\t" << ISBN << "\t" << dateBorrowed << "\n";
                }
            }
            file.close();
            cout << "Issued books data saved successfully!" << endl;
        } else {
            cerr << "Unable to save issued books data to file!" << endl;
        }
    }

    // Save borrowers' fines information to the file
    void saveBorrowersFineData() {
        ofstream file(borrowersFineDatabase);
        if (file.is_open()) {
            for (const auto& fines : borrowersFine) {
                file << fines.first << "\t" << fines.second << "\n";
            }
            file.close();
            cout << "Borrowers' fines data saved successfully!" << endl;
        }
        else {
            cerr << "Unable to save borrowers' fines data to file!" << endl;
        }
    }

    // Load borrower information from the file
    void loadBorrowerData() {
        ifstream file(borrowerDatabase);
        if (file.is_open()) {
            int borrowerId;
            string borrowerName, borrowerMobileNo;
            file >> std::ws;
            file.clear();
            file.seekg(0);

            while (file >> borrowerId >> std::ws) {
                getline(file, borrowerName, '\t');
                getline(file, borrowerMobileNo, '\n');
                Borrower newBorrower(borrowerId, borrowerName, borrowerMobileNo);
                borrowerInformation[borrowerId] = newBorrower;
                allBorrowers.insert(borrowerId);
            }

            file.close();
            cout << "Borrower data loaded successfully!" << endl;
        } else {
            cerr << "Error opening file: " << borrowerDatabase << endl;
        }
    }

    // Load issued books information from the file
    void loadIssuedBooksData() {
        ifstream file(issuedBooksDatabase);
        if (file.is_open()) {
            int borrowerId, ISBN;
            string dateBorrowed;
            file >> std::ws;
            file.clear();
            file.seekg(0);

            while (file >> borrowerId >> ISBN >> std::ws) {
                getline(file, dateBorrowed, '\n');
                borrowerBooks[borrowerId].insert({ISBN, dateBorrowed});
            }

            file.close();
            cout << "Issued books data loaded successfully!" << endl;
        } else {
            cerr << "Error opening file: " << issuedBooksDatabase << endl;
        }
    }

    // Load borrowers' fines information from the file
    void loadBorrowersFineData() {
        ifstream file(borrowersFineDatabase);
        if (file.is_open()) {
            int borrowerId, fine;
            file >> std::ws;
            file.clear();
            file.seekg(0);

            while (file >> borrowerId >> fine >> std::ws) {
                borrowersFine[borrowerId] = fine;
                allFines.insert(borrowerId);
            }

            file.close();
            cout << "Borrowers' fines data loaded successfully!" << endl;
        }
        else {
            cerr << "Error opening file: " << borrowersFineDatabase << endl;
        }
    }

    //Fine
    int processTransaction(const string & borrowedDate , int borrowerId){
        Transaction transaction;
        int fine = transaction.calculateFine(borrowedDate);
        if(fine){
            allFines.insert(borrowerId);
            borrowersFine[borrowerId]+=fine;
        }
        return fine;
    }
    
};


int main() {
    Library myLibrary;
    int input = -1;
    cout << "\t \t \t \tWELCOME TO THE LIBRARY" << endl
         << endl;
    while (input != 10000) {
        cout << "Enter 1 to Add a new book to the Library" << endl;
        cout << "Enter 2 to Get details of all books in the Library" << endl;
        cout << "Enter 3 to Get details of a particular book" << endl;
        cout << "Enter 4 to Issue book" << endl;
        cout << "Enter 5 to Get details of all issued books" << endl;
        cout << "Enter 6 to Get details of a particular borrower" << endl;
        cout << "Enter 7 to return the book" << endl;
        cout << "Enter 8 to pay dues" << endl;
        cout << "Enter 9 to exit" << endl;
        cin >> input;

        switch (input) {
        case 1: {
            int ISBN, price, quantity;
            string title, author;
            cout << "Enter a unique ISBN for the new Book: ";
            cin >> ISBN;
            if (myLibrary.allBooks.find(ISBN) != myLibrary.allBooks.end()) {
                cout << "Book with the same ISBN already exists" << endl;
                break;
            }
            cout << "\nEnter Book's Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "\nEnter Book's Author: ";
            getline(cin, author);
            cout << "\nEnter the price of the book: ";
            cin >> price;
            cout << "\nEnter the quantity of the book: ";
            cin >> quantity;
            myLibrary.addBookToLibrary(ISBN, title, author, price, quantity);
            cout << "\t \t \t \tBook added to the Library Successfully!! \n"
                 << endl;
            break;
        }

        case 2:{
            myLibrary.getAllBooksDetails();
            break;
        }
            
        case 3: {
            cout << "Search Book By its :" << endl;
            cout << "a : ISBN" << endl;
            cout << "b : Title" << endl;
            cout << "c : Author" << endl;
            cout << "d : Main Menu" << endl;
            char choice;
            cin >> choice;
            cin.ignore();
            switch (choice) {
            case 'a': {
                int ISBN;
                cout << "Enter the ISBN number :" << endl;
                cin >> ISBN;
                myLibrary.searchByISBN(ISBN);
                break;
            }
            case 'b': {
                string bookTitle;
                cout << "Enter the Title of the Book : " << endl;
                getline(cin, bookTitle);
                myLibrary.searchByTitle(bookTitle);
                break;
            }
            case 'c': {
                string bookAuthor;
                cout << "Enter the Author of the Book : " << endl;
                getline(cin, bookAuthor);
                myLibrary.searchByAuthor(bookAuthor);
                break;
            }

            default:
                break;
            }
            break;
        }
        case 4: {
            int ISBN;
            cout<<"Enter Book ISBN : "<<endl;
            cin>>ISBN;
            if(myLibrary.allBooks.find(ISBN) == myLibrary.allBooks.end()){
                cout<<"No such book exists"<<endl;
                break;
            }
            int remainingQuantity = myLibrary.bookInformation[ISBN].quantity;
            if(remainingQuantity == 0){
                cout<<"Book is not Available currently!!"<<endl;
                break;
            }
            
            cout<<"\t \t \t \tEnter Borrower Details :"<<endl;
            int borrowerId;
            string borrowerName;
            string borrowerMobileNo;
            cout << "Enter Borrower's ID: ";
            cin>>borrowerId;
            //If the borrower with the given id is already present then no need to again take his/her name or mobile number
            if(myLibrary.allBorrowers.find(borrowerId)==myLibrary.allBorrowers.end()){
                cout << "\nEnter Borrower's Name: ";
                cin.ignore();
                getline(cin, borrowerName);
                cout << "\nEnter Borrower's Mobile Number: ";
                getline(cin, borrowerMobileNo);
                // Borrower newBorrower;
                myLibrary.saveBorrower(borrowerId , borrowerName , borrowerMobileNo);
            }
            else{
                cout<<"Borrower's Name is "<<myLibrary.borrowerInformation[borrowerId].borrowerName<<endl;
                borrowerName = myLibrary.borrowerInformation[borrowerId].borrowerName;
            }
            bool alreadyIssued = false;
            myLibrary.issueBookToBorrower(ISBN , borrowerId , alreadyIssued);
            if(!alreadyIssued){
                myLibrary.bookInformation[ISBN].quantity--;
                cout<<"Book with ISBN "<<ISBN<<" is issued to "<<borrowerName<<" succesfully"<<endl;
            }
            break;
        }
        case 5:{
            if(myLibrary.borrowerBooks.size() == 0){
                cout<<"Currently no book is issued!"<<endl;
            }
            for(auto borrowerPair : myLibrary.borrowerBooks){
                // if(myLibrary.borrowerBooks[borrowerPair.first].size() == 0){
                //     continue;
                // }
                cout<<"\t \t \t \tBorrower ID : "<<borrowerPair.first<<" "<<"Borrower's Name : "<<myLibrary.borrowerInformation[borrowerPair.first].borrowerName<<" has following books :" <<endl;
                for(auto books : myLibrary.borrowerBooks[borrowerPair.first]){
                    cout<<"Book ISBN : "<<books.first<< " Book's Name :" <<myLibrary.bookInformation[books.first].title<< " Borrowed Date : "<<books.second<<endl;
                }
            }
            break;
        }
        case 6:{
            cout<<"Enter Borrower's ID : "<<endl;
            int borrowerId;
            cin>>borrowerId;
            if(myLibrary.allBorrowers.find(borrowerId) == myLibrary.allBorrowers.end()){
                cout<<"No such borrower exists "<<endl;
            }
            else{
                cout<<"Borrower's Name : "<<myLibrary.borrowerInformation[borrowerId].borrowerName<<endl;
                cout<<"Borrower's Mobile Number : "<<myLibrary.borrowerInformation[borrowerId].borrowerMobileNo<<endl;
                cout<<"Issued Books :"<<endl;
                for(auto books : myLibrary.borrowerBooks[borrowerId]){
                    cout<<"Book ISBN : "<<books.first<< " Book's Name :" <<myLibrary.bookInformation[books.first].title<< " Borrowed Date : "<<books.second<<endl;
                }
                if(myLibrary.allFines.find(borrowerId) != myLibrary.allFines.end()){
                    int due = myLibrary.borrowersFine[borrowerId];
                    if(due){
                        cout<<"Due : Rs "<<due<<endl;
                    }
                }
            }
            break;
        }
        case 7:{
            cout<<"Enter Borrower's ID : "<<endl;
            int borrowerId ;
            cin>>borrowerId;
            if(myLibrary.borrowerBooks[borrowerId].size() == 0){
                cout<<"Borrower has no book issued!"<<endl;
                break;
            }
            for(auto books : myLibrary.borrowerBooks[borrowerId]){
                cout<<"Book ISBN : "<<books.first<< " Book's Name :" <<myLibrary.bookInformation[books.first].title<< " Borrowed Date : "<<books.second<<endl;
            }
            cout<<"Enter Book's ID :"<<endl;
            int ISBN;
            cin>>ISBN;
            if(myLibrary.allBooks.find(ISBN) == myLibrary.allBooks.end()){
                cout<<"No such book exists"<<endl;
                break;
            }
            bool isBorrowed = false;
            string borrowedDate ;
            for(auto books : myLibrary.borrowerBooks[borrowerId]){
                if(ISBN == books.first){
                    isBorrowed = true;
                    borrowedDate = books.second;
                    break;
                }
            }
            if(!isBorrowed){
                cout<<"Borrower does not contain this book"<<endl;
                break;
            }
            
            int charges = myLibrary.processTransaction(borrowedDate , borrowerId);
            cout<<"Fine is : "<<charges<<endl;
            myLibrary.borrowerBooks[borrowerId].erase({ISBN , borrowedDate});
            myLibrary.bookInformation[ISBN].quantity++;
            if(charges){
                myLibrary.allFines.insert(borrowerId);
                cout<<"Pay fine right now ?"<<endl;
                int option;
                cout<<"Enter 1 for YES"<<endl;
                cout<<"Enter 2 for NO"<<endl;
                cin>>option;
                if(option == 1){
                    cout<<"Enter amount to pay : "<<endl;
                    int amount ;
                    cin>>amount;
                    while(amount>charges){
                        cout<<"Your amount is greater than Fine!!"<<endl;
                        cout<<"Enter amount again"<<endl;
                        cin>>amount;
                    }
                    if(amount == charges){
                        cout<<"Payment successful "<<endl;
                        cout<<"Paid : "<<amount<<endl;
                    }
                    else{
                        cout<<"Payment successful "<<endl;
                        cout<<"Paid : "<<amount<<endl;
                        cout<<"Due : "<< charges - amount<<endl;
                        myLibrary.borrowersFine[borrowerId]-=amount;
                    }
                }
                else if(option != 2){
                    cout<<"Invalid option"<<endl;
                }
            }
            if(myLibrary.borrowersFine[borrowerId] == 0){
                myLibrary.borrowersFine.erase(borrowerId);
                myLibrary.allFines.erase(borrowerId);
                cout<<"No dues"<<endl;
            }
            cout<<"Book returned Successfully!"<<endl;
        
            break;


        }
        case 8:{
            cout<<"Enter Borrower's ID : "<<endl;
            int borrowerId;
            cin>>borrowerId;
            if(myLibrary.allBorrowers.find(borrowerId)==myLibrary.allBorrowers.end()){
                cout<<"No borrower found !"<<endl;
                break;
            }
            if(myLibrary.allFines.find(borrowerId)==myLibrary.allFines.end()){
                cout<<"No Dues"<<endl;
                break;
            }
            if(myLibrary.borrowersFine[borrowerId] == 0){
                cout<<"No Dues"<<endl;
                break;
            }
            int charges = myLibrary.borrowersFine[borrowerId];
            cout<<"Dues : Rs "<< charges <<endl;
            cout<<"Enter amount to pay : "<<endl;
            int amount ;
            cin>>amount;
            while(amount>charges){
                cout<<"Your amount is greater than Fine!!"<<endl;
                cout<<"Enter amount again"<<endl;
                cin>>amount;
            }
            if(amount == charges){
                cout<<"Payment successful "<<endl;
                cout<<"Paid : "<<amount<<endl;
                cout<<"You have no due now"<<endl;
                myLibrary.borrowersFine.erase(borrowerId);
                myLibrary.allFines.erase(borrowerId);
            }
            else{
                cout<<"Payment successful "<<endl;
                cout<<"Paid : "<<amount<<endl;
                cout<<"Due : "<< charges - amount<<endl;
                myLibrary.borrowersFine[borrowerId]-=amount;
            }
            break;
        }
        case 9: {
            cout<<"Thankyou for using Library"<<endl;
            myLibrary.saveBookData();
            myLibrary.saveBorrowerData();
            myLibrary.saveIssuedBooksData();
            myLibrary.saveBorrowersFineData();
            exit(0);
        }
            

        default:
            cout<<"Invalid choice!!!"<<endl;
            break;
        }
        cout<<"Press Enter key to continue"<<endl;
        cin.ignore();
        cin.get();
       
    }

    return 0;
}
