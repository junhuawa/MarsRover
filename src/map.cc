#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "coordinator.h"
#include "map.h"
int move_steps[2][4] = 
{
    {0, -1, 0, +1},
    {1, 0, -1, 0}
};

typedef struct map
{
    int height;
    int width;
    char *ptr;
} map_t;

map_t map;
bool map_initialize(int height, int width)
{
    if((height <= 0) or(width <= 0)) return false;
    map.height = height;
    map.width = width;
    map.ptr = (char *)malloc((map.height+1) * (map.width+1));
    if(!map.ptr) 
        return false;
    memset(map.ptr, EMPTY, (map.height+1)*(map.width+1));
    return true;
}

int get_map_height(void)
{
    return map.height;
}


int get_map_width(void)
{
    return map.width;
}

coordinator_t get_corrected_coordinator(coordinator_t coordinator)
{
    int x, y;
    x = coordinator.x;
    y = coordinator.y;
    while(x < 0) {
        x = x + map.width + 1;
    }

    while(x > map.width) {
        x = x - (map.width+1);
    }

    while(y < 0) {
        y = y + map.height + 1;
    }

    while(y > map.height) {
        y = y - (map.height +1);
    }

    coordinator.x = x;
    coordinator.y = y;

    return coordinator;
}

bool is_coordinator_occupied(coordinator_t coordinator)
{
    if(map.ptr[coordinator.y * (map.width+1) + coordinator.x] == OCCUPIED) {
        return true;
    }
    return false;
}

bool clear_rover_in_map(coordinator_t coordinator)
{
    if(!map.ptr) return false;
    map.ptr[coordinator.y * (map.width+1) + coordinator.x] = EMPTY;

    return true;
}

bool move_in_map(coordinator_t from, coordinator_t to)
{
    if(!map.ptr) return false;

    clear_rover_in_map(from);
    mark_rover_in_map(to);

    //printf("(%d, %d) -> (%d, %d)\n", from_coordinator.x, from_coordinator.y, to_coordinator.x, to_coordinator.y);
    return true;
}


bool mark_rover_in_map(coordinator_t coordinator)
{
    if(!map.ptr) return false;
    map.ptr[coordinator.y * (map.width+1) + coordinator.x] = OCCUPIED;

    return true;
}

bool is_same_coordinator(coordinator_t coordinator1, coordinator_t coordinator2)
{
    return ((coordinator1.x == coordinator2.x)&&(coordinator1.y == coordinator2.y));
}

void print_map(void)
{
    int i, j;
    if(!map.ptr) return;
    printf("map: \n");
    for(i=map.height; i>=0;  i--) {
        for(j=0; j<=map.width; j++) {
            printf("%d, ", map.ptr[i*(map.width+1) +j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_coordinator(coordinator_t coordinator)
{
    printf("(%d, %d)\n", coordinator.x, coordinator.y);
}

void clear_in_map(coordinator_t coordinator)
{
    if(map.ptr) {
        map.ptr[coordinator.y * (map.width+1) + coordinator.x] = EMPTY;
    }
}

bool map_destroy(void)
{
    if(map.ptr) {
        free(map.ptr);
        map.ptr = NULL;
    }
    map.height = map.width = 0;
    return true;
}

coordinator_t get_neighbour_coordinator(coordinator_t coordinator, int direction)
{
    coordinator_t new_coordinator;

    new_coordinator.x = coordinator.x + move_steps[0][direction];
    new_coordinator.y = coordinator.y + move_steps[1][direction];

    new_coordinator = get_corrected_coordinator(new_coordinator);

    return new_coordinator;
}
