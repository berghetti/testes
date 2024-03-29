// list.c
// linked list

#include <stdio.h>
#include <stdlib.h>

struct node
{
  int value;
  struct node *next, *prev;
};

static struct node *head, *last;

static struct node*
create_node(int value)
{
  struct node *node = malloc (sizeof *node);

  if (node)
    {
      node->value = value;
      node->next = NULL;
      node->prev = NULL;
    }

  return node;
}

static void
insert_head(struct node *node)
{
  if (!head)
    {
      head = node;
      last = node;
      return;
    }

  head->prev = node;
  node->next = head;
  head = node;
}

static void
insert_tail(struct node *node)
{
  if (!head)
    {
      head = node;
      last = node;
      return;
    }

  last->next = node;
  node->prev = last;
  last = node;
}

// insert after element
static void
insert_after(struct node *node, struct node *after)
{
  if (!after)
    {
      insert_tail(node);
      return;
    }

  struct node *tmp = after->next;

  if (!tmp)
    {
      last = node;  // last element
    }
  else
    {
      node->next = tmp;
      tmp->prev = node;
    }

  after->next = node;
  node->prev = after;
}

static void
insert_before(struct node *node, struct node *before)
{
  if (!before)
    {
      insert_head(node);
      return;
    }

  struct node *tmp = before->prev;

  if ( !tmp )
    {
      head = node; // first element
      head->prev = NULL;
    }
  else
    {
      tmp->next = node;
      node->prev = tmp;
    }

  node->next = before;
  before->prev = node;
}

static void
delete_first(struct node *node)
{
  head = node->next;
  head->prev = NULL;
}

static void
delete_middle(struct node *node)
{
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

static void
delete_last(struct node *node)
{
  last = node->prev;
  last->next = NULL;
}

static void
delete(struct node *node)
{
  if (!node)
    return;

  if ( !node->prev )
    {
      // only one element in list
      if ( !node->next )
        {
          last = head = NULL;
        }
       // first element
      else
        delete_first(node);
    }
  // last element
  else if ( !node->next )
    {
      delete_last(node);
    }
  else
    {
      delete_middle(node);
    }

  free(node);
}

static struct node *
search(int value)
{
  struct node *p = head;

  while(p)
    {
      if (p->value == value)
        return p;

      p = p->next;
    }

  // NULL
  return p;
}

static struct node *
search_tail(int value)
{
  struct node *p = last;

  while(p)
    {
      if (p->value == value)
        return p;

      p = p->prev;
    }

  // NULL
  return p;
}

void print_list(struct node *node)
{
  while(node)
    {
      printf(" %d ", node->value);
      node = node->next;
    }
  putchar('\n');
}

int main(void)
{
  // init
  for (int i = 0; i <= 10; i++)
    {
      insert_tail( create_node(i) );
    }

  puts("List initialized");
  print_list(head);

  puts("\nInserted 20 after element of value 5");
  insert_after( create_node(20), search(5) );
  print_list( head );

  puts("\nInserted 60 after element of value 10");
  insert_after( create_node(60), search(10) );
  print_list( head );

  puts("\nDeleted element of value 6");
  delete( search(6) );
  print_list( head );

  puts("\nInserted 40 before element of value 0");
  insert_before( create_node(40), search(0) );
  print_list(head);

  puts("\nDeleted element of value 40");
  delete( search(40) );
  print_list( head );

  puts("\nDeleted element of value 20");
  delete( search(20) );
  print_list( head );

  puts("\nDeleted element of value 60");
  delete( search(60) );
  print_list( head );

  puts("\nDeleting all elements (last to first)...");
  while(last)
    {
      printf("deleting ... %d\n", last->value);
      delete(last);
    }

  puts("\nNew initialized list");

  for (int i = 0; i <= 10; i++)
    {
      insert_tail( create_node(i+10) );
    }

  print_list(head);

  puts("\nInserting 5 head (implict)");
  // search go return NULL;
  insert_before(create_node(5), search (8));
  print_list(head);

  printf( "\nsearch tail - %d\n", ( search_tail(10) )->value );


  puts("\nDeleting all elements (first to last)...");
  while(head)
    {
      printf("deleting ... %d\n", head->value);
      delete(head);
    }

}
