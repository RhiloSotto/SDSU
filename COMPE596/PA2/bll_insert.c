// #include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Node{
    int value;
    struct Node* next;
    struct Node* prev;
    // omp_lock_t lock;
} typedef Node;


int main(int argc, char* argv[]) {

    // double start_time = omp_get_wtime();

    // pointers
    Node* head = NULL;
    Node* tail = NULL;
    Node* p, *prev, *newNode = NULL;
    int value = 0;

    int nThreads = atoi(argv[1]);
    int a = atoi(argv[2]);

    // create initial head/tail node
    head = (Node*)calloc(1, sizeof(Node));
    head->value = 0;
    head->next = NULL;
    head->prev = NULL;
    omp_init_lock(&head->lock);

    tail = head;

    double N = pow(2, a);
    unsigned long long k = 0;

    #pragma omp parallel for private(k, value, newNode, p, prev)
    for (k = 0; k < (unsigned long long)N; k++) {
        prev = head;
        value = rand() % 1000 + 1; // random value
        p = head->next;

        // iterate through linked list to find where to insert node
        while (p != NULL) {
            if (p->value >= value) break;
            prev = p;
            p = p->next;
        }
        // p holds the place where the node should be inserted before

        // create new node
        newNode = (Node*)calloc(1, sizeof(Node));
        newNode->value = value;
        newNode->next = p;
        newNode->prev = prev;
        if (p != NULL) {
            p->prev = newNode;
        }
        newNode->prev->next = newNode;
        // omp_init_lock(&newNode->lock);
    }
 
    printf("%d, %llu, %lf\n", nThreads, (unsigned long long)N, run_time);
    
    return 0;
}

