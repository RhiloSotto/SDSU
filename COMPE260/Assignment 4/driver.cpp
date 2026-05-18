#include <iostream>
using namespace std;

#include "BST.h"


int main() {
    int choice = 0;
    unsigned int numNodes = 0;
    char data = 0;
    unsigned int i = 0;
    
    BST Tree; // empty BST

    while (choice != 7) { // loop
        cout << "-------------------- MENU ----------------------" << endl
        << "    1. Insert node(s)" << endl
        << "    2. Traverse Preorder" << endl
        << "    3. Search BST" << endl
        << "    4. Delete node" << endl
        << "    5. Leaf Count" << endl
        << "    6. Sibling of a node" << endl
        << "    7. Quit" << endl
        << "    Enter your choice: ";


        cin >> choice;  // poll user
        switch (choice) {
            case 1: // insert
                cout << "Enter number of nodes to insert: ";
                cin >> numNodes;
                if (numNodes < 1) {
                    cout << "Must be an integer greater than zero" << endl;
                    break;
                }
                i = 0;
                while (i < numNodes) {
                    cout << "Enter node: ";
                    cin >> data;
                    if(Tree.Insert(data)) // increment if a valid node was inserted
                        ++i;
                }
                break;
            case 2: // traverse
                if (Tree.Empty()) {
                    cout << "BST is empty!" << endl;
                    break;
                }
                Tree.Preorder(cout);
                break;

            case 3: // search
                if (Tree.Empty()) {
                    cout << "BST is empty!" << endl;
                    break;
                }
                cout << "Enter item you want to search for: ";
                cin >> data;
                if (Tree.Search(data))
                    cout << data << " is found in the BST" << endl;
                else
                    cout << data << " is not found in the BST" << endl;
                break;

            case 4: // delete
                if (Tree.Empty()) {
                    cout << "BST is empty!" << endl;
                    break;
                }
                cout << "Enter item you want to delete: ";
                cin >> data;
                Tree.Delete(data);

                break;

            case 5: // count
                cout << "There are " << Tree.Count(Tree.GetRootNode()) << " number of leaves in the BST." << endl;
                break;

            case 6: // sibling
                if (Tree.Empty()) {
                    cout << "BST is empty!" << endl;
                    break;
                }
                cout << "Enter item you want to find the sibling of: ";
                cin >> data;
                Tree.Sibling(data);
                break;

            case 7: // quit
                cout << "Quitting Program" << endl;
                break;
            default:
                cout << "Invalid Choice" << endl;
                break;
        }
    }
    return 0;
}