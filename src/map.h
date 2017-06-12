#ifndef __MAP_H__
#define __MAP_H__
#include "coordinator.h"

#define EMPTY 0
#define OCCUPIED 1

bool map_initialize(int height, int width);
int get_map_height(void);
int get_map_width(void);
bool mark_rover_in_map(coordinator_t coordinator);
bool move_in_map(coordinator_t from_coordinator, coordinator_t to_coordinator);
bool is_coordinator_occupied(coordinator_t coordinator);
bool is_same_coordinator(coordinator_t coordinator1, coordinator_t coordinator2);
coordinator_t get_corrected_coordinator(coordinator_t coordinator);
bool map_destroy(void);
void print_map(void);
void print_coordinator(coordinator_t coordinator);
coordinator_t get_neighbour_coordinator(coordinator_t coordinator, int direction);
void clear_in_map(coordinator_t coordinator);


#endif
