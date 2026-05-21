#include <iostream>


template <typename T>
class MyStackArrayType {
    private:
        T* S = nullptr;
        int capacity = 1;
        int sp = -1;
        void resize() { // dynamically resize internal array
            int newCapacity = capacity * 2;
            T *newS = new T[newCapacity];
            // copy elements to new array
            for (int i = 0; i <= sp; i++) {
                newS[i] = S[i];
            }
            delete[] S; 
            S = newS;
            capacity = newCapacity;
        }
    public:
        MyStackArrayType(int size = 8) {  // Constructor
            capacity = size; // default size
            S = new T[capacity];
            sp = -1;
        }

        ~MyStackArrayType() {  // Destructor
            delete[] S;
        }

        // Copy Constructor
        MyStackArrayType(const MyStackArrayType& other)
            : capacity(other.capacity), sp(other.sp)
        {
            S = new T[capacity];
            for (int i = 0; i <= sp; ++i)
                S[i] = other.S[i];
        }

        // Copy Assignment
        MyStackArrayType& operator=(const MyStackArrayType& other) {
            if (this == &other) return *this;

            delete[] S;

            capacity = other.capacity;
            sp = other.sp;

            S = new T[capacity];
            for (int i = 0; i <= sp; ++i)
                S[i] = other.S[i];

            return *this;
        }

        // Move Constructor
        MyStackArrayType(MyStackArrayType&& other) noexcept
            : S(other.S), capacity(other.capacity), sp(other.sp)
        {
            other.S = nullptr;
            other.capacity = 0;
            other.sp = -1;
        }

        // Move Assignment
        MyStackArrayType& operator=(MyStackArrayType&& other) noexcept {
            if (this == &other) return *this;

            delete[] S;

            S = other.S;
            capacity = other.capacity;
            sp = other.sp;

            other.S = nullptr;
            other.capacity = 0;
            other.sp = -1;

            return *this;
        }

        void push(T item) { // — Pushes item onto the top of the stack.
            if (sp == capacity - 1) {
                resize(); // dynamic resize
            }
            sp++;
            S[sp] = item;
        }

        T pop() {   // — Removes and returns the top item.
            if (is_empty()) {
                std::cout << "stack is empty\n";
                return T();
            }
            T item = S[sp];
            sp--;
            return item;
        }
            
        T top() {   // — Returns the top item without removing it.
            if (is_empty()) {
                std::cout << "stack is empty\n";
                return T();
            }
            return S[sp];
        }

        bool is_empty() {   // — Returns true if the stack contains no elements, false otherwise.
            return sp == -1;
        }

        int size() { // — Returns the current number of elements in the stack.
            return sp + 1;
        }
};


template <typename T>
class Node {
    private:
    public:
        T data;
        Node* next = nullptr;
        // Constructor
        Node() : data(), next(nullptr) {}
        // Parameter Constructor
        Node(T d, Node* N) : data(d), next(N) {}
        // Destructor
        ~Node() {}
};


template <typename T>
class MyStackLinkedListType {
    private:
        Node<T>* head = nullptr;
        int sp = 0;
    public:
        MyStackLinkedListType() : head(nullptr), sp(0) {}  // Constructor

        ~MyStackLinkedListType() {  // Destructor
            Node<T>* temp;
            while(head != nullptr) {
                temp = head;
                head = head->next;
                delete temp;
            }
        }

        // Copy Constructor
        MyStackLinkedListType(const MyStackLinkedListType& other)
            : head(nullptr), sp(0)
        {
            if (!other.head) return;

            // Copy first node
            head = new Node<T>(other.head->data, nullptr);
            Node<T>* curNew = head;
            Node<T>* curOld = other.head->next;

            while (curOld) {
                curNew->next = new Node<T>(curOld->data, nullptr);
                curNew = curNew->next;
                curOld = curOld->next;
            }

            sp = other.sp;
        }

        // Copy Assignment
        MyStackLinkedListType& operator=(const MyStackLinkedListType& other) {
            if (this == &other) return *this;

            // Clear current list
            while (head) {
                Node<T>* temp = head;
                head = head->next;
                delete temp;
            }

            head = nullptr;
            sp = 0;

            if (!other.head) return *this;

            head = new Node<T>(other.head->data, nullptr);
            Node<T>* curNew = head;
            Node<T>* curOld = other.head->next;

            while (curOld) {
                curNew->next = new Node<T>(curOld->data, nullptr);
                curNew = curNew->next;
                curOld = curOld->next;
            }

            sp = other.sp;
            return *this;
        }

        // Move Constructor
        MyStackLinkedListType(MyStackLinkedListType&& other) noexcept
            : head(other.head), sp(other.sp)
        {
            other.head = nullptr;
            other.sp = 0;
        }

        // Move Assignment
        MyStackLinkedListType& operator=(MyStackLinkedListType&& other) noexcept {
            if (this == &other) return *this;

            // clear current
            while (head) {
                Node<T>* temp = head;
                head = head->next;
                delete temp;
            }

            head = other.head;
            sp = other.sp;

            other.head = nullptr;
            other.sp = 0;

            return *this;
        }
        
        void push(T item) { // — Pushes item onto the top of the stack.
            if (head == nullptr) {
                Node<T>* newNode = new Node<T>(item, nullptr);
                head = newNode;
                sp++;
            }
            else {
                Node<T>* newNode = new Node<T>(item, head);
                head = newNode;
                sp++;
            }
        }

        T pop() {
            if (is_empty()) {   // — Removes and returns the top item.
                std::cout << "stack is empty\n";
                return T();
            }
            T item = head->data;
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            sp--;
            return item;
        }
            
        T top() {   // — Returns the top item without removing it.
            if (is_empty()) {
                std::cout << "stack is empty\n";
                return T();
            }
            return head->data;
        }

        bool is_empty() {   // — Returns true if the stack contains no elements, false otherwise.
            return head == nullptr;
        }

        int size() { // — Returns the current number of elements in the stack.
            return sp;
        }
};


template <typename T>
class MyQueueArrayType {
    private:
        T* Q = nullptr;
        int capacity = 1;
        int head = 0;
        int tail = -1;
        int sp;
    public:
        MyQueueArrayType(int size = 64) {  // Constructor
            capacity = size;
            Q = new T[capacity];
            head = 0;
            tail = -1;
            sp = 0;
        }

        ~MyQueueArrayType() { // Destructor
            delete[] Q;
        }

        // Copy Constructor
        MyQueueArrayType(const MyQueueArrayType& other)
            : capacity(other.capacity),
            head(other.head),
            tail(other.tail),
            sp(other.sp)
        {
            Q = new T[capacity];
            for (size_t i = 0; i < capacity; ++i)
                Q[i] = other.Q[i];
        }

        // Copy Assignment
        MyQueueArrayType& operator=(const MyQueueArrayType& other) {
            if (this == &other) return *this;

            delete[] Q;

            capacity = other.capacity;
            head = other.head;
            tail = other.tail;
            sp = other.sp;

            Q = new T[capacity];
            for (size_t i = 0; i < capacity; ++i)
                Q[i] = other.Q[i];

            return *this;
        }

        // Move Constructor
        MyQueueArrayType(MyQueueArrayType&& other) noexcept
            : Q(other.Q),
            capacity(other.capacity),
            head(other.head),
            tail(other.tail),
            sp(other.sp)
        {
            other.Q = nullptr;
            other.capacity = 0;
            other.sp = 0;
        }

        // Move Assignment
        MyQueueArrayType& operator=(MyQueueArrayType&& other) noexcept {
            if (this == &other) return *this;

            delete[] Q;

            Q = other.Q;
            capacity = other.capacity;
            head = other.head;
            tail = other.tail;
            sp = other.sp;

            other.Q = nullptr;
            other.capacity = 0;
            other.sp = 0;

            return *this;
        }

        void enqueue(T item) {   // — Adds item to the rear of the queue.
            if (is_full()) {
                std::cout << "queue is full\n";
                return;
            }
            tail = (tail+1) % capacity;
            Q[tail] = item;
            sp++;
        }

        T dequeue() {  // — Removes and returns the item at the front.
            if (is_empty()) {
                std::cout << "queue is empty\n";
                return T();
            }
            T item = Q[head];
            head = (head+1) % capacity;
            sp--;
            return item;
        }
        
        T front() {    // — Returns the front item without removing it.
            if (is_empty()) {
                std::cout << "queue is empty\n";
                return T();
            }
            return Q[head];
        }
        
        bool is_empty() {  // — Returns true if the queue is empty.
            return sp == 0;
        }

        bool is_full() {
            return (sp == capacity);
        }

        int size() {    // — Returns the current number of elements.
            return sp;
        }
};


template <typename T>
class MyQueueLinkedListType {
    private:
        Node<T>* head = nullptr;
        Node<T>* tail = nullptr;
        int sp = 0;

    public:
        MyQueueLinkedListType() : head(nullptr), tail(nullptr), sp(0) {}  // Constructor

        ~MyQueueLinkedListType() { // Destructor
            Node<T>* temp;
            while(head != nullptr) {
                temp = head;
                head = head->next;
                delete temp;
            }
        }

        // Copy Constructor
        MyQueueLinkedListType(const MyQueueLinkedListType& other)
            : head(nullptr), tail(nullptr), sp(0)
        {
            Node<T>* curOld = other.head;
            while (curOld) {
                enqueue(curOld->data);
                curOld = curOld->next;
            }
        }

        // Copy Assignment
        MyQueueLinkedListType& operator=(const MyQueueLinkedListType& other) {
            if (this == &other) return *this;

            while (!is_empty())
                dequeue();

            Node<T>* curOld = other.head;
            while (curOld) {
                enqueue(curOld->data);
                curOld = curOld->next;
            }

            return *this;
        }

        // Move Constructor
        MyQueueLinkedListType(MyQueueLinkedListType&& other) noexcept
            : head(other.head), tail(other.tail), sp(other.sp)
        {
            other.head = nullptr;
            other.tail = nullptr;
            other.sp = 0;
        }

        // Move Assignment
        MyQueueLinkedListType& operator=(MyQueueLinkedListType&& other) noexcept {
            if (this == &other) return *this;

            while (!is_empty())
                dequeue();

            head = other.head;
            tail = other.tail;
            sp = other.sp;

            other.head = nullptr;
            other.tail = nullptr;
            other.sp = 0;

            return *this;
        }

        void enqueue(T item) {  // — Adds item to the rear of the queue.
            Node<T>* newNode = new Node<T>(item, nullptr);
            if (head == nullptr) 
                head = newNode;
            else 
                tail->next = newNode;
            tail = newNode;
            sp++;
        }
        
        T dequeue() {   // — Removes and returns the item at the front.
            if (is_empty()) {
                std::cout << "queue is empty\n";
                return T();
            }
            T item = head->data;
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            sp--;
            return item;
        }
        
        T front() { // — Returns the front item without removing it.
            if (is_empty()) {
                std::cout << "queue is empty\n";
                return T();
            }
            return head->data;
        }
        
        bool is_empty() {   // — Returns true if the queue is empty.
            return head == nullptr;
        }

        int size() { // — Returns the current number of elements.
            return sp;
        }
};


template <typename T>
class TreeNode {
    private:
    public:
        T data;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        // Constructor
        TreeNode() : data(), left(nullptr), right(nullptr) {}
        // Parameter Constructor
        TreeNode(T d, TreeNode* L, TreeNode* R) : data(d), left(L), right(R) {}
        // Destructor
        ~TreeNode() {}
};


template <typename T>
class MyBinaryTreeType {
    private:
        TreeNode<T>* root = nullptr;

        TreeNode<T>* copy_tree(TreeNode<T>* node) { // recursive helper function
            if (!node) return nullptr;
            return new TreeNode<T>(
                node->data,
                copy_tree(node->left),
                copy_tree(node->right)
            );
        }

        // recursive helper functions for traversal
        void inorder(TreeNode<T>* cur) { // recursive helper function
            if (cur == nullptr) // end of tree
                return;
            if (cur->left != nullptr) { // traverse left
                inorder(cur->left);
            }
            printf("%d, ", cur->data); // root
            if (cur->right != nullptr) { // traverse right
                inorder(cur->right);
            }
        }
        void preorder(TreeNode<T>* cur) { // recursive helper function
            if (cur == nullptr) // end of tree
                return;
            printf("%d, ", cur->data); // root
            if (cur->left != nullptr) { // traverse left
                preorder(cur->left);
            }
            if (cur->right != nullptr) { // traverse right
                preorder(cur->right);
            }
        }
        void postorder(TreeNode<T>* cur) { // recursive helper function
            if (cur == nullptr) // end of tree
                return;
            if (cur->left != nullptr) { // traverse left
                postorder(cur->left);
            }
            if (cur->right != nullptr) { // traverse right
                postorder(cur->right);
            }
            printf("%d, ", cur->data); // root
        }

        void postorder_destruct(TreeNode<T>* cur) { // recursive helper function
            if (cur == nullptr) // end of tree
                return;
            if (cur->left != nullptr) { // traverse left
                postorder_destruct(cur->left);
            }
            if (cur->right != nullptr) { // traverse right
                postorder_destruct(cur->right);
            }
            delete(cur); // root
        }
    public:
        MyBinaryTreeType() {}   // Constructor
        
        ~MyBinaryTreeType() {  // Destructor
            if (root != nullptr) {
                postorder_destruct(root);
            }
        }

        // Copy Constructor
        MyBinaryTreeType(const MyBinaryTreeType& other) {
            root = copy_tree(other.root);
        }

        // Copy Assignment
        MyBinaryTreeType& operator=(const MyBinaryTreeType& other) {
            if (this == &other) return *this;

            if (root)
                postorder_destruct(root);

            root = copy_tree(other.root);
            return *this;
        }

        // Move Constructor
        MyBinaryTreeType(MyBinaryTreeType&& other) noexcept
            : root(other.root)
        {
            other.root = nullptr;
        }

        // Move Assignment
        MyBinaryTreeType& operator=(MyBinaryTreeType&& other) noexcept {
            if (this == &other) return *this;

            if (root)
                postorder_destruct(root);

            root = other.root;
            other.root = nullptr;

            return *this;
        }

        void insert(T value) {    // — Inserts value into the BST at the correct position. Duplicates are ignored
            if (root == nullptr) { // BST is empty, make node the root
                TreeNode<T>* newNode = new TreeNode<T>(value, nullptr, nullptr);
                root = newNode;                
            }
            else { // traverse until correct position is found
                TreeNode<T>* cur; // traversing pointer
                TreeNode<T>* prev; // previous pointer
                cur = root;
                while (cur != nullptr) { // traverse until prev is a leaf node
                    if (value == cur->data) { // new node is a duplicate, ignore
                        printf("Value is already in the BST\n");
                        return;
                    }
                    if (value < cur->data) { // traverse left
                        prev = cur;
                        cur = cur->left;
                    }
                    else if (value > cur->data) { // traverse right
                        prev = cur;
                        cur = cur->right;
                    }
                }
                // prev holds node to make parent of new node, can't be a duplicate
                if (value < prev->data) { // new node is left child
                    TreeNode<T>* newNode = new TreeNode<T>(value, nullptr, nullptr);
                    prev->left = newNode;
                }
                else if (value > prev->data) { // new node is right child
                    TreeNode<T>* newNode = new TreeNode<T>(value, nullptr, nullptr);
                    prev->right = newNode;
                }
                else { // edge case of a duplicate
                    printf("Duplicate attempted to be inserted\n");
                    return;
                }

            }
        }

        bool find(T value) {  // — Returns true if value exists in the tree, false otherwise.
            TreeNode<T>* cur; // traversing pointer
            cur = root;
            while (cur != nullptr) { // traverse until value is found or exiting tree
                if (value == cur->data) { // Node is found
                    return true;
                }
                if (value < cur->data) { // traverse left
                    cur = cur->left;
                }
                else if (value > cur->data) { // traverse right
                    cur = cur->right;
                }
            }
            // reached end of tree, value does not exist
            return false;
        }

        // Traversals will print the values
        void in_order_traversal(TreeNode<T>* cur = nullptr) { // — Returns elements in ascending order (left → root → right).
            if (cur == nullptr) { // default node is root
                cur = root;
            }
            inorder(cur);
        }
        void pre_order_traversal(TreeNode<T>* cur = nullptr) {    // — Returns elements in root → left → right order.
            if (cur == nullptr) { // default node is root
                cur = root;
            }
            preorder(cur);
        }
        void post_order_traversal(TreeNode<T>* cur = nullptr) {   // — Returns elements in left → right → root order.
            if (cur == nullptr) { // default node is root
                cur = root;
            }
            postorder(cur);
        }
};