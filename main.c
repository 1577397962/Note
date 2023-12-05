#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include "list.h"
// (*(void (*)())0)(); //0->void (*)()   == fun ,  *fun()
// struct node{
//     int a;
//     int b;
// } n;
// struct node m;
// typedef struct Node{
//     int h;
// } Node;
// Node *p;
// ///////////
// void (*single(int,void (*)(int)))(int);

// typedef void(*fun)(int);
// fun single(int,fun);

// void (*pfun1)();
// void *pfun2();


// void test(char *str)
// {
//     printf("%s",str);
// }


/*
* This is a singly linked list.
*/
typedef struct list{
    void *data;
    struct list *next;

    int nodes;
} list;
typedef struct node{
    void  *data;
    struct node *next;
} node;
struct list *create_list(void)
{
    struct list *list;
    list = (struct list *)malloc(sizeof(struct list *));
    if(!list)
        return NULL;
    return list;
}
void destory_list(struct list *list)
{
    if(list)
        free(list);
}
struct node *list_prepend(void *data)
{
    struct node *n = malloc(sizeof(node *));
    n->data = data;
    n->next = NULL;
    return n;
}
int main(int argc,char *argv[])
{
    struct list *l;
    int data[4] = {1,2,5,4};
    if(!(l = create_list()))
    {
        perror("result is ");
        return -1;
    }
    l->data = data[2];
    l->nodes++;
       
    l->next = NULL;
    printf("%d\n",l->nodes);
    free(l);
    l = NULL;
    //l=l->next;        
    return 0;
}
