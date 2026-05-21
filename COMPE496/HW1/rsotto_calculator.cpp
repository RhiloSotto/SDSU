#include "rsotto_DSA.cpp"
#include <cstring>
#include <cctype>
#include <cmath>
#include <cstdio>


int apply_operator(int a, int b, char op) { // helper function
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                printf("Error: Division by zero\n");
                return 0;
            }
            return a / b;
        case '^': {
            int result = 1;
            for (int i = 0; i < b; ++i)
                result *= a;
            return result;
        }
    }
    return 0;
}

int precedence(char op) { // helper function
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool is_right_associative(char op) { // helper function
    return op == '^';
}

int evaluate_rpn(char* expression) { // reverse polish notation

    MyStackArrayType<int> values;

    char* token = strtok(expression, " ");

    while (token != nullptr) {

        // Operator
        if (strlen(token) == 1 && strchr("+-*/^", token[0])) {

            int b = values.pop();
            int a = values.pop();
            int result = apply_operator(a, b, token[0]);
            values.push(result);
        }
        // Number
        else {
            values.push(atoi(token));
        }

        token = strtok(nullptr, " ");
    }

    return values.pop();
}


int evaluate_infix(char* expression) { // infix notation

    MyStackArrayType<int> values;
    MyStackArrayType<char> ops;

    for (int i = 0; expression[i] != '\0'; ++i) {

        if (isspace(expression[i]))
            continue;

        // Parse number
        if (isdigit(expression[i])) {
            int val = 0;
            while (isdigit(expression[i])) {
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            values.push(val);
            i--;
        }

        else if (expression[i] == '(') {
            ops.push('(');
        }

        else if (expression[i] == ')') {

            while (ops.size() > 0 && ops.top() != '(') {
                int b = values.pop();
                int a = values.pop();
                char op = ops.pop();
                values.push(apply_operator(a, b, op));
            }

            ops.pop(); // remove '('
        }

        else if (strchr("+-*/^", expression[i])) {

            char current = expression[i];

            while (ops.size() > 0 &&
                  (
                   precedence(ops.top()) > precedence(current) ||
                   (precedence(ops.top()) == precedence(current) &&
                    !is_right_associative(current))
                  )) {

                if (ops.top() == '(')
                    break;

                int b = values.pop();
                int a = values.pop();
                char op = ops.pop();
                values.push(apply_operator(a, b, op));
            }

            ops.push(current);
        }
    }

    // Apply remaining operators
    while (ops.size() > 0) {
        int b = values.pop();
        int a = values.pop();
        char op = ops.pop();
        values.push(apply_operator(a, b, op));
    }

    return values.pop();
}


template <typename S>
void StackTests(S stack) {
    // Push a series of at least five integers and print the stack size after each push.
    for (int i = 1; i <= 5; ++i) {
        stack.push(i);
        printf("size: %d\n", stack.size());
    }
    // Call top() and verify the correct value is returned without modifying the stack.
    printf("top: %d\n", stack.top());
    // Pop all items one by one, printing each value and the remaining size.
    for (int i = 1; i <= 5; ++i) {
        printf("popped: %d\n", stack.pop());
        printf("size: %d\n", stack.size());
    }
    // Edge cases: pop()/top() on an empty stack, push after a full pop cycle.
    stack.pop();
    stack.top();
    stack.push(1);
}

template <typename Q>
void QueueTests(Q queue) {
    // Enqueue a series of at least five integers, printing size after each operation.
    for (int i = 1; i <= 5; ++i) {
        queue.enqueue(i);
        printf("size: %d\n", queue.size());
    }
    // Call front() and verify correctness without dequeuing.
    printf("front: %d\n", queue.front());
    // Dequeue all items, printing each value and remaining size.
    for (int i = 1; i <= 5; ++i) {
        printf("dequeued: %d\n", queue.dequeue());
        printf("size: %d\n", queue.size());
    }
    // Edge cases: dequeue()/front() on an empty queue, interleaved enqueue/dequeue.
    queue.dequeue();
    queue.front();
    for (int i = 1; i <= 5; ++i) {
        queue.enqueue(i);
        queue.dequeue();
    }
}

template <typename B>
void BinaryTreeTests(B bst) {
    // Edge cases: find on an empty tree, insert duplicate values.
    std::cout << "find on an empty tree returns false: " << bst.find(1) << "\n";

    // Insert at least seven integers (including duplicates, negative numbers, and values that test the full tree shape).
    for (int j = 0; j <= 5; ++j) {
        for (int i = -5 + j; i <= 5 + j; i += 3) {
            printf("inserting: %d\n", i);
            bst.insert(i);
        }
    }
    // Search for integers that are present and integers that are absent, printing the result.
    for (int i = 5; i <= 15; ++i) {
        std::cout << "finding: " << i << ", found? " << bst.find(i) << "\n";
    }
    // Print the results of all three traversals and verify they match expected output.
    std::cout << "in_order_traversal: ";
    bst.in_order_traversal();
    std::cout << "\n";
    std::cout << "pre_order_traversal: ";
    bst.pre_order_traversal();
    std::cout << "\n";
    std::cout << "post_order_traversal: ";
    bst.post_order_traversal();
    std::cout << "\n";    
}

void CalculatorTests() {
    // Simple RPN expression
    char rpn1[] = "3 4 +";
    std::cout << evaluate_rpn(rpn1) << "\n";

    // Multi-operator RPN
    char rpn2[] = "3 4 2 * 1 5 - 2 ^ 3 ^ / +";
    std::cout << evaluate_rpn(rpn2) << "\n";
    
    // Simple infix
    char infix1[] = "3 + 4";
    std::cout << evaluate_infix(infix1) << "\n";
    
    // Complex infix
    char infix2[] = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";
    std::cout << evaluate_infix(infix2) << "\n";
    
    // Division by zero
    char infix3[] = "1/0";
    std::cout << evaluate_infix(infix3) << "\n";
}

void MyTest() {
    MyStackArrayType<int> MyStackA;
    MyStackLinkedListType<int> MyStackLL;
    MyQueueArrayType<int> MyQueueA;
    MyQueueLinkedListType<int> MyQueueLL;
    MyBinaryTreeType<int> MyBinaryTree;

    std::cout << "Stack Array\n";
    StackTests(MyStackA);
    std::cout << "Stack Linked List\n";
    StackTests(MyStackLL);
    std::cout << "Queue Array\n";
    QueueTests(MyQueueA);
    std::cout << "Queue Linked List\n";
    QueueTests(MyQueueLL);
    std::cout << "Binary Tree\n";
    BinaryTreeTests(MyBinaryTree);
    std::cout << "Calculator\n";
    CalculatorTests();
}

int main(int argc, char* argv[]) {
    MyTest();

    return 0;
}