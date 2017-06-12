#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <stack>
#include <iostream>
#include <cstdlib>
#include "shortestpath.h"
#include "map.h"
#include "direction.h"
#include "known_table.h"

using namespace std;
typedef struct vertex * vertex_t;

struct vertex {
    coordinator_t coordinator;
    vertex_t path;
    char direction;
};

vertex_t create_new_vertex(coordinator_t coordinator, int direction)
{
    vertex_t tmp;
    tmp = (vertex_t)malloc(sizeof(struct vertex));
    if(!tmp) return NULL;

    tmp->coordinator.x = coordinator.x;
    tmp->coordinator.y = coordinator.y;
    tmp->direction = direction;
    tmp->path = NULL;
    return tmp;
}


void free_stack(stack<vertex_t>* Stack)
{
    vertex_t V;
    while(!Stack->empty()) {
        V = Stack->top();
        free(V);
        Stack->pop();
    }
}

void free_queue(queue<vertex_t>* Queue)
{
    vertex_t V;
    while(!Queue->empty()) {
        V = Queue->front();
        free(V);
        Queue->pop();
    }
}
void print_vertex(vertex_t V)
{
    printf("(%d, %d) %d\n", V->coordinator.x, V->coordinator.y, V->direction);
}

void get_all_commands(stack<char> *commands, vertex_t V, int new_direction, int final_direction)
{
    push_turn_direction_commands(commands, new_direction, final_direction);
    commands->push('M');
    push_turn_direction_commands(commands, V->direction, new_direction);
    while(V->path) {
        commands->push('M');
        push_turn_direction_commands(commands, V->path->direction, V->direction);

        V = V->path;
    }
}

void free_all(stack<vertex_t> *stack, queue<vertex_t> *queue, char *table)
{
    free_stack(stack);
    free_queue(queue);
    known_table_destroy(table);
}

queue<vertex_t> Queue;
stack<vertex_t> Stack;
bool get_commands_by_find_shortest_path(rover_t *start_rover, rover_t *destination_rover, stack<char> *commands)
{
    char *known_table;
    vertex_t S, V, W;

    known_table =known_table_initialize();
    if(!known_table) return false;

    S = create_new_vertex(start_rover->coordinator, start_rover->direction);
    if(!S) {
        known_table_destroy(known_table);
        return false;
    }

    set_coordinator_known(known_table, S->coordinator);
    Queue.push(S);

    while(!Queue.empty()) {
        V = Queue.front();
        for(int i=0; i<4; i++) {
            int direction = (V->direction+i)%4;
            coordinator_t tmp = get_neighbour_coordinator(V->coordinator, direction);

            if(is_coordinator_known(known_table, tmp))continue; 
            if(is_same_coordinator(tmp, destination_rover->coordinator)) {
                get_all_commands(commands, V, direction, destination_rover->direction);
                free_all(&Stack, &Queue, known_table);
                return true;
            }
            if(is_coordinator_occupied(tmp)) continue;  
            W = create_new_vertex(tmp, direction);
            if(!W) {
                free_all(&Stack, &Queue, known_table);
                return false;
            }
            set_coordinator_known(known_table, W->coordinator);
            W->path = V;
            Queue.push(W);
        }
        Stack.push(V);
        Queue.pop();
    }
    free_all(&Stack, &Queue, known_table);
    return false;
}
