#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
    int data;
    node *next;
};

typedef struct stack
{
    node *top;  // �@���v����
    // �ݭn���ܥi�H�[�J�ܼƬ���stack�j�p
}stack;


// �^��stack�O�_����
int is_empty(stack *s)
{
    return (s->top == NULL ? 1 : 0);
}

// �˵�top�ثe�����
int peek(stack *s)
{
    if(is_empty(s))
    {
        printf("stack is empty\n");
        return -1;
    }
    else
        return s->top->data;
}

// ���X��ƨçR����node
int pop(stack *s)
{
    if(is_empty(s))
    {
        printf("stack is empty\n");
        return -1;
    }
    else
    {
        node *tmp = s->top;
        int data = tmp->data;

        s->top = tmp->next;
        free(tmp);
        return data;
    }
}

// ��J�snode
void push(stack *s, int data)
{
    node *new = malloc(sizeof(node));
    new->data = data;
    new->next = NULL;

    if(is_empty(s))
        s->top = new;
    else
    {
        new->next = s->top;
        s->top = new;
    }
}

void print(stack *s)
{
    node *tmp = s->top;

    while(tmp != NULL)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

// �R��stack
void delete(stack *s)
{
    node *cur = s->top;
    while(cur != NULL)
    {
        node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int main()
{
    // ��l��stack
    stack *s = malloc(sizeof(stack));
    s->top = NULL;

    push(s, 1);
    push(s, 4);
    push(s, 7);
    push(s, 10);
    print(s);

    printf("\npop:  %d\n", pop(s));
    print(s);

    printf("\npeek: %d\n", peek(s));
    print(s);
}
