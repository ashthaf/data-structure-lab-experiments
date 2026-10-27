#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef struct Node {
    int key, value;
    struct Node *next;
} Node;

typedef struct {
    int key, value, status;
} Entry;

// Hash table data
Node *chain[SIZE] = {NULL};
Entry table[SIZE] = {0};

int hash(int key) {
    return (key % SIZE + SIZE) % SIZE;
}

// Separate Chaining
void insertChain(int k, int v) {
    int i = hash(k);

    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    n->key = k;
    n->value = v;
    n->next = chain[i];
    chain[i] = n;

    printf("Inserted using Separate Chaining\n");
}

// Linear Probing
void insertOpen(int k, int v) {
    int i = hash(k);

    for (int j = 0; j < SIZE; j++) {
        int p = (i + j) % SIZE;

        if (table[p].status != 1) {
            table[p] = (Entry){k, v, 1};
            printf("Inserted at index %d using Open Addressing\n", p);
            return;
        }
    }

    printf("Hash table is full\n");
}

// Search
int search(int k) {
    int i = hash(k);

    // Search using Linear Probing
    for (int j = 0; j < SIZE; j++) {
        int p = (i + j) % SIZE;

        if (table[p].status == 0)
            break;

        if (table[p].status == 1 && table[p].key == k) {
            printf("Found! Value = %d\n", table[p].value);
            return 1;
        }
    }

    // Search using Separate Chaining
    for (Node *n = chain[i]; n != NULL; n = n->next) {
        if (n->key == k) {
            printf("Found! Value = %d\n", n->value);
            return 1;
        }
    }

    printf("Key not found\n");
    return 0;
}

// Delete
void deleteKey(int k) {
    int i = hash(k);

    // Delete from Linear Probing table
    for (int j = 0; j < SIZE; j++) {
        int p = (i + j) % SIZE;

        if (table[p].status == 0)
            break;

        if (table[p].status == 1 && table[p].key == k) {
            table[p].status = 2;
            printf("Key deleted\n");
            return;
        }
    }

    // Delete from Separate Chaining
    Node *n = chain[i];
    Node *prev = NULL;

    while (n != NULL) {
        if (n->key == k) {
            if (prev != NULL)
                prev->next = n->next;
            else
                chain[i] = n->next;

            free(n);
            printf("Key deleted\n");
            return;
        }

        prev = n;
        n = n->next;
    }

    printf("Key not found\n");
}

// Main function
int main(void) {
    int choice, k, v, method;

    while (1) {
        printf("\n--- HASH TABLE MENU ---\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            break;
        }

        switch (choice) {
            case 1: {
                printf("Enter key and value: ");

                if (scanf("%d%d", &k, &v) != 2) {
                    printf("Invalid input\n");
                    return 1;
                }

                int i = hash(k);

                // Insert directly if the original index is free
                if (table[i].status == 0 && chain[i] == NULL) {
                    table[i] = (Entry){k, v, 1};
                    printf("Inserted at index %d\n", i);
                } else {
                    printf("Collision!\n");
                    printf("1. Separate Chaining\n");
                    printf("2. Open Addressing (Linear Probing)\n");
                    printf("Choose method: ");

                    if (scanf("%d", &method) != 1) {
                        printf("Invalid input\n");
                        return 1;
                    }

                    if (method == 1)
                        insertChain(k, v);
                    else if (method == 2)
                        insertOpen(k, v);
                    else
                        printf("Invalid method\n");
                }
                break;
            }

            case 2:
                printf("Enter key: ");

                if (scanf("%d", &k) != 1) {
                    printf("Invalid input\n");
                    return 1;
                }

                search(k);
                break;

            case 3:
                printf("Enter key: ");

                if (scanf("%d", &k) != 1) {
                    printf("Invalid input\n");
                    return 1;
                }

                deleteKey(k);
                break;

            case 4:
                // Free dynamically allocated nodes
                for (int i = 0; i < SIZE; i++) {
                    Node *n = chain[i];

                    while (n != NULL) {
                        Node *temp = n;
                        n = n->next;
                        free(temp);
                    }
                }

                printf("Exiting program\n");
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}