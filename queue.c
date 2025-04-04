#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>

uint64_t get_val(struct queue * path)
{
    if (path->data.head == NULL)
    {
        return path->data.head->value;
    }
    while (path->data.head->next != NULL)
    {
        path->data.head->next = path->data.head->next->next;
    }
    
    return path->data.head->value;
}

bool arrays_equal(struct game_state other_state, struct game_state current_state)
{


    for (int i = 0; i< 4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            if (other_state.tiles[i][j] != current_state.tiles[i][j])
            {
                return false;
            }
        }
            
        
    }
    return true;
}

void enqueue(struct queue *q, struct game_state state) 
{
    uint64_t value = serialize(state);
    insert_at_tail(&(q->data), value);
    
}

struct game_state dequeue(struct queue *q) 
{ 
    struct linked_list *list = &(q->data);
    uint64_t num = remove_from_head(list);

    return deserialize(num);
}

int number_of_moves(struct game_state start) 
{ 
// initializes an array of attempted moves
    // uint64_t *attempted_moves = malloc(sizeof(uint64_t)*1000);
    struct queue * attempted_moves = malloc(sizeof(struct queue));

// create goal state
    struct game_state * goal_state = malloc(sizeof(struct game_state));
    for (int i = 1; i< 17; i++)
    {
        int col_idx = (i-1)%4;
        int row_idx = (i-1)/4;
        if (i == 16)
        {
            goal_state->tiles[row_idx][col_idx]= 0; 
        }
        else
        {
            goal_state->tiles[row_idx][col_idx]= i;
        }

    }

    goal_state->num_steps = 0;
// stores the linkedlist path for the queues
    struct queue * path_entrance = malloc(sizeof(struct queue));
    struct linked_list my_list;
    struct linked_list my_list_attempted;
    my_list.head = NULL;
    my_list_attempted.head = NULL;
    path_entrance->data = my_list;
    attempted_moves->data = my_list_attempted;

// enqueues the starting path to initialize the of the current moves and the attempted moves
    enqueue(path_entrance, start);
    enqueue(attempted_moves, start);

// create current state and current val (from that state)
    struct game_state * current_state = malloc(sizeof(struct game_state));
    *current_state = start;

// variable counts the number of states
    int num_states = 1;
    // struct game_state * right;
    // struct game_state * left; 
    // struct game_state * up;
    // struct game_state * down;

    
// check if the queue is empty or if the state matches the goal state
// if not continue queueing and dequeueing 
// increment num_states
// add newest state to array of attempted moves
    while (1 != arrays_equal(*goal_state, *current_state) || path_entrance->data.head != NULL)
    {

        if (arrays_equal(*goal_state, *current_state))
        {
            free(attempted_moves);
            free(current_state->tiles);
            free(goal_state->tiles); 
        
            return current_state->num_steps;
        }
        // check if previous state has already occurred
        // if it has then move to the next value and increment num_moves

        *current_state = dequeue(path_entrance);
        
        struct game_state right = (*current_state);
        struct game_state left = *current_state;
        struct game_state up = *current_state;
        struct game_state down = *current_state;

       

        move_right(&right);
        move_left(&left);
        move_up(&up);
        move_down(&down);
        
        struct game_state game_arr[4] = {right, left, up, down};

        struct linked_list * traverse_list = &(attempted_moves->data);
        struct list_node * peek_list;
        for (int i = 0; i < 4; i++)
        {
            peek_list = traverse_list->head;
            int equal = 0;
            while(peek_list != NULL)
            {

                if (arrays_equal(deserialize(peek_list->value), game_arr[i])== 1)
                {
                    equal = 1;
                }
                peek_list= peek_list->next;
            } 
            if(!equal)
            {
                enqueue(path_entrance, game_arr[i]);
                insert_at_tail(&(attempted_moves->data), serialize(*current_state));
                num_states++;
            }
            
        }
            
        
        


    // goes all the way to the end of the linkedlist to find the latest serialized state and sets it equal to value
        
    }

    // check if there is no valid path    
    if (1 != arrays_equal(*goal_state, *current_state) )
    {
        return -1;
    }
// free everything

free(attempted_moves);
free(current_state->tiles);
free(goal_state->tiles); 

return goal_state->num_steps; 
}

