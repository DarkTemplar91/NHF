#include "linkedlist.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>

void printList(linkedList* list)
{
    listNode* current=list->first->next;
    while(current!=list->last)
    {
        printf("\nName: %s - Score: %d",current->name,current->score);
        current=current->next;
    }
}

static void deleteLastNode(linkedList* list)
{
    listNode* del=list->last->prev;
    listNode* last=del->prev;
    last->next=list->last;
    free(del);
}
void linkedListInit(linkedList* list)
{
    list->count=0;
    listNode* s=(listNode*)malloc(sizeof(listNode));
    listNode* e=(listNode*)malloc(sizeof(listNode));
    s->next=e;
    e->prev=s;
    list->first=s;
    list->last=e;
}
void insertNode(linkedList* list, char* name, int score)
{
    list->count++;
    listNode* current=list->first->next;
    while(current!=list->last && score>current->score)
        current=current->next;

    listNode* newElement=(listNode*)malloc(sizeof(listNode));
    newElement->prev=current->prev;
    newElement->next=current;
    current->prev->next=newElement;
    current->prev=newElement;
    newElement->score=score;
    strcpy(newElement->name,name);
    if(list->count>LIMIT)
    {
        deleteLastNode(list);
        list->count--;
    }
}
void freelist(linkedList* list)
{
    listNode* current=list->first;
    while(current!=NULL)
    {
        listNode* next=current->next;
        free(current);
        current=next;
    }

}
void saveLinkedList(char* path,linkedList* list)
{
    if(list->count==0)
        return;
    FILE* fp=fopen(path,"w");
    listNode* current=list->first->next;
    while(current!=list->last)
    {
        fprintf(fp,"%s ;%d;\n",current->name,current->score);
        current=current->next;
    }
    fclose(fp);

}
void loadTopList(char* path,linkedList* list)
{
    FILE* fp=fopen(path,"r");
    char buffer[20];
    if (!fp)
        return;

    while (fgets(buffer,20,fp))
    {
        char name[11];
        int score;
        sscanf(buffer,"%s ;%d;\n",name,&score);
        insertNode(list,name,score);
    }

    fclose(fp);

}
