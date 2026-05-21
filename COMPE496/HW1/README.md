COMPE 496 Data Structures & Algorithms
1. Files Included
File Description
rsotto_DSA.cpp Stack, Queue, and Binary Search Tree
implementations
rsotto_calculator.cpp Calculator implementation (RPN + Infix)
and MyTest()
README.md Build instructions, assumptions, and design notes

2. Compilation & Execution Instructions
This program was developed and tested using:
OnlineGDB C++ compiler
https://www.onlinegdb.com/online_c++_compiler
It is also compatible with g++ locally.
Compile (Local Environment)
g++ rsotto_DSA.cpp rsotto_calculator.cpp -o assignment1
Run
./assignment1

No STL containers (such as std::stack, std::queue, std::list,
etc.) were used.

3. Implementation Overview
3.1 Stack Implementations
Two independent implementations were created:
- MyStackArrayType
- MyStackLinkedListType
Both expose identical public interfaces:
- push(int item)
- pop()
- top()
- is_empty()
- size()
Design Decisions
- The array-based stack dynamically resizes when capacity is
reached.
- The linked-list stack uses a singly linked list with the head
representing the top.
- Both implementations maintain an internal size counter.
Empty Stack Policy
- Calling pop() or top() on an empty stack:
- Prints an error message.
- Returns 0 as a sentinel value.
- Program execution continues (no crash).
- This behaviour is documented and consistent across both
implementations.
3.2 Queue Implementations
Two independent implementations were created:
- MyQueueArrayType
- MyQueueLinkedListType
Both expose identical public interfaces:
- enqueue(int item)
- dequeue()
- front()
- is_empty()
- size()
Design Decisions
- The array-based queue uses a circular buffer implementation.
- The linked-list queue maintains both head (front) and tail
(rear) pointers for O(1) enqueue and dequeue.
- Both implementations maintain a size counter.
Empty Queue Policy
- Calling dequeue() or front() on an empty queue:
- Prints an error message.
- Returns 0 as a sentinel value.
- Program execution continues.
3.3 Binary Search Tree (MyBinaryTreeType)
Implemented as a standard BST where:
- Left subtree values are strictly less than the node value.
- Right subtree values are strictly greater than the node value.
- Duplicate Handling
- Duplicate values are ignored and not inserted into the tree.
Implemented Methods
- insert(int value)
- find(int value)
- in_order_traversal()
- pre_order_traversal()
- post_order_traversal()
Traversal Strategy
- Traversal methods print values directly to standard output in
the required order.
Edge Cases Handled
- find() on an empty tree returns false.
- Inserting into an empty tree correctly sets the root.
- Attempting to insert duplicates does not modify the tree.

4. Calculator Implementation
4.1 Supported Operators
The calculator supports:
+
-
*
/
^ (exponentiation)
4.2 Division Behaviour
- Integer division is used.
- Division truncates toward zero (standard C++ integer division
behaviour).
4.3 Division by Zero Handling
If division by zero occurs:
- An error message is printed.
- The function returns 0.
- Execution continues without crashing.
4.4 Reverse Polish Notation (RPN)
Method:
- evaluate_rpn(string expression)
- Uses MyStackArrayType internally.
- Assumes space-separated tokens.
- Supports multi-operator expressions.
4.5 Infix Expression Evaluation
Method:
- evaluate_infix(string expression)
Operator Rules
- ^ is right-associative
- * and / have higher precedence than + and -
- Parentheses are fully supported
- Input expressions are assumed to be well-formed but basic
safeguards prevent crashes on malformed input.

5. MyTest Function
A comprehensive MyTest() function was implemented to validate:
5.1 Stack Tests
- Push 5+ integers
- Print size after each push
- Validate top() without removal
- Pop all items
- Test pop/top on empty stack
- Push again after emptying
- Both array-based and linked-list stacks are tested.
5.2 Queue Tests
- Enqueue 5+ integers
- Print size after each enqueue
- Validate front() without dequeue
- Dequeue all items
- Test dequeue/front on empty queue
- Interleaved enqueue/dequeue operations
- Both queue types are tested.
5.3 Binary Tree Tests
- Insert 7+ integers including:
- Negative values
- Duplicate values
- Values forming different tree shapes
- Search for existing and non-existing values
Print:
- In-order traversal
- Pre-order traversal
- Post-order traversal
- Test find() on empty tree
5.4 Calculator Tests
RPN:
- Simple expressions ("3 4 +")
- Multi-operator expressions
- All supported operators
Infix:
- Simple expressions ("3 + 4")
- Complex expression:
- 3 + 4 * 2 / (1 - 5) ^ 2 ^ 3
- Right-associative exponentiation verified
- Division by zero tested
- Single-operand expressions tested

6. Memory Management
- Linked structures properly delete nodes where required.
- No memory leaks occur during normal execution.
- No STL data structure containers were used.

7. Assumptions
- Input expressions are space-separated.
- Expressions are generally well-formed.
- Sentinel return value for invalid operations is 0.
- Duplicate BST insertions are ignored.

9. Conclusion
This assignment demonstrates:
- Implementation of abstract data types from scratch
- Understanding of array vs linked-list tradeoffs
- Correct BST behaviour and traversal logic
- Stack-based parsing and evaluation of arithmetic expressions
- Robust handling of edge cases
- All required naming conventions and structural constraints were
followed exactly as specified in the assignment document.