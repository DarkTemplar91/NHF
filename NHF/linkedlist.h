#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#define LIMIT 50

typedef struct listNode
{
    struct listNode* prev;
    struct listNode* next;
    char name[10+1];
    int score;
}listNode;

typedef struct linkedList
{
    int count;
    listNode* first;
    listNode* last;

}linkedList;

void freelist(linkedList* list);
void linkedListInit(linkedList* list);
void insertNode(linkedList* list, char* name, int score);
void printList(linkedList* list);
void linkedListInit(linkedList* list);
void saveLinkedList(char* path,linkedList* list);
void loadTopList(char* path,linkedList* list);


#endif // LINKEDLIST_H_INCLUDED
