#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) 
{ 
  struct list_node * node = malloc(sizeof(struct list_node));
  node -> value = value;
  node -> next = NULL;
  return node; 
}


void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node *nd;
  nd = new_node(value);
  nd -> next = list -> head;
  list -> head = nd;

}



void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node * last = list->head;

  if (last == NULL)
  {
    list->head = new_node(value);
  }
  else 
  {
    while(last -> next != NULL)
    {
      last = last -> next;
    }
  
    last -> next = new_node(value);
  }

}

size_t remove_from_head(struct linked_list *list) 
{
  if (list->head == NULL)
  {
    return 0;
  }
  struct list_node * q = list -> head;
  size_t val = q->value;
  list -> head = q-> next;
  free(q);


  return val;
}

size_t remove_from_tail(struct linked_list *list) 
{ 

  if (list->head == NULL)
  {
    return 0;
  }
  size_t val= list->head->value;
  if (list->head->next == NULL)
  {
    free(list->head);
    return val;
  }

  struct list_node * second_last = list->head;
  while(second_last ->next ->next != NULL)
  {
    second_last = second_last->next;
  }

  struct list_node * last = second_last->next;
  size_t value = last->value;
  free(last);
  second_last->next = NULL;
  return value;
}

void free_list(struct linked_list list) 
{
  struct linked_list temp;
  while(list.head != NULL)
  {
    temp.head = list.head;
    list.head = list.head->next;
    free(temp.head);
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}

