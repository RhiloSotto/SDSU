#include <iostream>
using namespace std;


#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

// Node class
class Node {
    public:
        // DATA MEMBERS
        char data;
        Node* leftChild;
        Node* rightChild;

        // CONSTRUCTORS
        Node() : leftChild(nullptr), rightChild(nullptr) {}
        Node(char input) : data(input), leftChild(nullptr), rightChild(nullptr) {}
};
        
// BST class
class BST {
    public:
        // CONSTRUCTORS
        // empty BST
        BST() : rootNode(nullptr) {}

        // ACCESSOR FUNCTIONS
        // get the root node
        inline Node* GetRootNode() const{
            return rootNode;
        }

        inline bool Empty() const{
            return rootNode == nullptr;
        }

        // MAIN FUNCTIONS
        // Insert a node into BST
        inline bool Insert(const char data) {
            Node* searchNode = rootNode;    // start at the root node
            Node* parentNode = nullptr; // root node has no parent
            bool found = false; // check if new node is already in BST

            while (!found && searchNode != nullptr) {
                parentNode = searchNode;    // advance parent node to next level
                if (data < searchNode->data)    // data is < so go left
                    searchNode = searchNode->leftChild;
                else if (data > searchNode->data)   // data is > so go right
                    searchNode = searchNode->rightChild;
                else    // data == data, already in BST
                    found = true;
            }
            if (!found) {   // item is not in BST yet
                searchNode = new Node(data);
                if (parentNode == nullptr)
                    rootNode = searchNode;  // this is the root node
                else if (data < parentNode->data)
                    parentNode->leftChild = searchNode; // make new node left child
                else 
                    parentNode->rightChild = searchNode;    // make new node right child
                cout << "Inserted." << endl;
                return true;
            }
            else
                cout << "This Node is already in the BST" << endl;
                return false;
        }

        // Traverse the BST in preorder 
        inline void Preorder(ostream& out) {
            out << "        Traversing Preorder" << endl
            << "Node Info   " << "Left Child Info   " << "Right Child Info" << endl
            << "---------   --------------- ----------------" << endl;

            PreorderHelper(out, rootNode);
        }

        // Search the BST for a node
        inline bool Search(const char key) {
            Node* searchNode = rootNode;
            bool found = false; // check whether found or not

            while (!found && searchNode != nullptr) {
                if (key < searchNode->data) // data is < so go left
                    searchNode = searchNode->leftChild;
                else if (key > searchNode->data)    // data is > so go right
                    searchNode = searchNode->rightChild;
                else                           // item found
                    found = true;
            }
            return found;
        }

        // Delete a node from the BST
        inline void Delete(const char key) {
            bool found = false; // signals if item is found
            Node* searchNode = rootNode;    // points to node to be deleted
            Node* parentNode = nullptr;   //    "    " parent of searchNode and searchNodeSucc
            search2(key, found, searchNode, parentNode);

            if (!found) {
                cout << key << " not in the BST" << endl;
                return;
            }
            //else
            if (searchNode->leftChild != nullptr && searchNode->rightChild != nullptr) {    // node has 2 children
                // Find searchNode's inorder successor and its parent
                Node* searchNodeSuccessor = searchNode->rightChild;
                parentNode = searchNode;
                while (searchNodeSuccessor->leftChild != nullptr) {   // descend left
                    parentNode = searchNodeSuccessor;
                    searchNodeSuccessor = searchNodeSuccessor->leftChild;
                }

                // Move contents of searchNodeSuccessor to searchNode and change searchNode 
                // to point to successor, which will be removed.
                searchNode->data = searchNodeSuccessor->data;
                searchNode = searchNodeSuccessor;
            } // end if node has 2 children

            // Now proceed with case where node has 0 or 2 child
            Node* subtree = searchNode->leftChild;  // pointer to a subtree of x
            if (subtree == nullptr)
                subtree = searchNode->rightChild;
            if (parentNode == nullptr)    // root being removed
                rootNode = subtree;
            else if (parentNode->leftChild == searchNode)   // left child of parent
                parentNode->leftChild = subtree; 
            else    // right child of parent
                parentNode->rightChild = subtree;
            delete searchNode;
            cout << key << " is deleted." << endl;
        }

        // Count the leaf nodes of the BST
        inline unsigned int Count(const Node* currentNode) {
            if (currentNode == nullptr)
                return 0;
            if (currentNode->leftChild == nullptr && currentNode->rightChild == nullptr) 
                return 1;
            else
                return Count(currentNode->leftChild) + Count(currentNode->rightChild);
        }        

        // Return sibling of a node if possible
        inline void Sibling(const char key) {
            bool found = false; // signals if item is found
            Node* searchNode = rootNode;   // points to node to be found
            Node* parentNode = nullptr;   // parent of searchNode
            search2(key, found, searchNode, parentNode);
            
            if (!found) {
                cout << key << " not in the BST" << endl;
                return;
            }
            if (searchNode == rootNode || parentNode == nullptr) {
                cout << "Root node has no siblings" << endl;
                return;
            }
            if (parentNode->leftChild == nullptr || parentNode->rightChild == nullptr) {
                cout << key << " has no sibling." << endl; // parentNode has one child, searchNode
            }
            else { // parentNode has two children
                cout << "The sibling of " << key << " is ";
                if (searchNode == parentNode->leftChild)
                    cout << parentNode->rightChild->data << endl;
                if (searchNode == parentNode->rightChild)
                    cout << parentNode->leftChild->data << endl;
            }
        }

    private:
        // DATA MEMBERS    
        Node* rootNode;

        // Helper function for Preorder, recursively prints nodes
        inline void PreorderHelper(ostream& out, Node* subrootNode) {
            if (subrootNode != nullptr) {
                out << "    " << subrootNode->data << "          ";
                
                if (subrootNode->leftChild == nullptr)
                    out << "NIL";
                else
                    out << subrootNode->leftChild->data; 
                out << "            ";
                
                if (subrootNode->rightChild == nullptr)
                    out << "NIL";
                else
                    out << subrootNode->rightChild->data; 
                out << endl;   // V operation

                PreorderHelper(out, subrootNode->leftChild); // L operation
                PreorderHelper(out, subrootNode->rightChild);    // R operation
            }
        }

        // Helper function for Delete and Sibling, returning the desired node and it's parent
        inline void search2(const char item, bool& found, Node*& searchNode, Node*& parentNode) {
            searchNode = rootNode;
            parentNode = nullptr;
            found = false;
            while (!found && searchNode != nullptr) {
                if (item < searchNode->data)       // descend left
                {
                    parentNode = searchNode;
                    searchNode = searchNode->leftChild;
                }
                else if (item > searchNode->data)  // descend right
                {
                    parentNode = searchNode;
                    searchNode = searchNode->rightChild;
                }
                else    // item found
                    found = true;
            }
        }
};
#endif