#ifndef __SHORTEST_PATH_H__
#define __SHORTEST_PATH_H__
#include <stack>
#include "rover.h"
#include "coordinator.h"
using namespace std;

bool get_commands_by_find_shortest_path(rover_t *start_rover, rover_t *destination_rover, stack<char> *commands);


#endif
