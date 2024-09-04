#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <fstream>  // Include for file handling

using namespace std;

// Structure for a Book
struct Book {
    int id;
    string title;
    string author;

    Book(int bookId, string bookTitle, string bookAuthor) 
        : id(bookId), title(bookTitle), author(bookAuthor) {}
};

// Node for Binary Search Tree (used for sorting books by title)
struct BSTNode {
    Book* book;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Book* b) : book(b), left(nullptr), right(nullptr) {}
};

// Class for managing the library system
class Library {
    unordered_map<int, Book*> bookTable;  // Hash table to store books by ID
    BSTNode* root;  // Root of BST to store books by title
    list<Book*> bookList;  // Linked list to store books in insertion order
    const string fileName = "library_data.txt";  // File to store book data

    // Helper function to insert book in BST
    BSTNode* insertInBST(BSTNode* node, Book* book) {
        if (!node) return new BSTNode(book);

        if (book->title < node->book->title)
            node->left = insertInBST(node->left, book);
        else
            node->right = insertInBST(node->right, book);

        return node;
    }

    // Helper function to delete node from BST
    BSTNode* deleteFromBST(BSTNode* root, string title) {
        if (!root) return nullptr;

        if (title < root->book->title) {
            root->left = deleteFromBST(root->left, title);
        } else if (title > root->book->title) {
            root->right = deleteFromBST(root->right, title);
        } else {
            if (!root->left) {
                BSTNode* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                BSTNode* temp = root->left;
                delete root;
                return temp;
            }

            BSTNode* temp = findMin(root->right);
            root->book = temp->book;
            root->right = deleteFromBST(root->right, temp->book->title);
        }
        return root;
    }

    // Helper function to find minimum value node in BST
    BSTNode* findMin(BSTNode* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    // Helper function to search in BST
    Book* searchInBST(BSTNode* node, string title) {
        if (!node) return nullptr;

        if (title == node->book->title)
            return node->book;
        else if (title < node->book->title)
            return searchInBST(node->left, title);
        else
            return searchInBST(node->right, title);
    }

    // Helper function to in-order traverse the BST
    void inOrderTraversal(BSTNode* node) {
        if (!node) return;
        inOrderTraversal(node->left);
        cout << "Book ID: " << node->book->id << ", Title: " << node->book->title << ", Author: " << node->book->author << endl;
        inOrderTraversal(node->right);
    }

    // Save all books to a file
    void saveToFile() {
        ofstream file(fileName);
        for (Book* book : bookList) {
            file << book->id << "," << book->title << "," << book->author << endl;
        }
        file.close();
    }

    // Load books from a file
    void loadFromFile() {
        ifstream file(fileName);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            int id;
            string title, author;
            size_t pos = 0;

            // Extract ID
            pos = line.find(",");
            id = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            // Extract Title
            pos = line.find(",");
            title = line.substr(0, pos);
            line.erase(0, pos + 1);

            // Extract Author
            author = line;

            addBook(id, title, author);
        }
        file.close();
    }

public:
    Library() : root(nullptr) {
        loadFromFile();
    }

    // Add a book to the library
    void addBook(int id, string title, string author) {
        Book* newBook = new Book(id, title, author);
        bookTable[id] = newBook;
        root = insertInBST(root, newBook);
        bookList.push_back(newBook);
        saveToFile();  // Save to file after adding
        cout << "Book added successfully." << endl;
    }

    // Remove a book from the library
    void removeBook(int id) {
        if (bookTable.find(id) == bookTable.end()) {
            cout << "Book with ID " << id << " not found." << endl;
            return;
        }

        Book* bookToRemove = bookTable[id];
        root = deleteFromBST(root, bookToRemove->title);
        bookList.remove(bookToRemove);
        bookTable.erase(id);
        delete bookToRemove;
        saveToFile();  // Save to file after removing
        cout << "Book removed successfully." << endl;
    }

    // Search for a book by title
    void searchBook(string title) {
        Book* foundBook = searchInBST(root, title);
        if (foundBook) {
            cout << "Book Found! ID: " << foundBook->id << ", Title: " << foundBook->title << ", Author: " << foundBook->author << endl;
        } else {
            cout << "Book not found." << endl;
        }
    }

    // Display all books in sorted order
    void displayBooks() {
        cout << "Books in Library (Sorted by Title):" << endl;
        inOrderTraversal(root);
    }
};

// Main function to demonstrate the Library Management System with user input
int main() {
    Library library;
    int choice, id;
    string title, author;

    do {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Search Book by Title" << endl;
        cout << "4. Display All Books" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Book ID: ";
                cin >> id;
                cin.ignore();  // Ignore the newline character left in the buffer
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                library.addBook(id, title, author);
                break;

            case 2:
                cout << "Enter Book ID to remove: ";
                cin >> id;
                library.removeBook(id);
                break;

            case 3:
                cin.ignore();  // Ignore the newline character left in the buffer
                cout << "Enter Book Title to search: ";
                getline(cin, title);
                library.searchBook(title);
                break;

            case 4:
                library.displayBooks();
                break;

            case 5:
                cout << "Exiting the system." << endl;
                break;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
