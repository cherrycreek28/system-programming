#ifndef STACK_H
#define STACK_H

typedef struct StackNode {
    struct StackNode* next; // pointer to item in the stack ("below" the current node)
    void* stackValue;       // pointer to the data stored
} StackNode;

typedef struct{
    StackNode* head;    // pointer to the head of the stack, NULL if empty
    int depth; 
} StackInfo;



void push(StackInfo* stack, void* value); 
void* newNode(void** data); 
int isEmpty(StackInfo* data); 
//StackNode* newNode(int data);
//int isEmpty(StackNode* root);
//void push(StackNode** root, int data);
void* pop(StackInfo* stack);
void* peek(StackInfo* stack);
#endif /* STACK_H */
