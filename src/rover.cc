#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <stack>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "coordinator.h"
#include "shortestpath.h"

using namespace std;


void set_new_coordinator(rover_t *rover, coordinator_t new_coordinator);

bool rover_initialize(rover_t *rover, coordinator_t coordinator, char direction, bool fake_flag)
{
   if(!rover) return false;
   if(!is_direction_allowed(direction)) return false;

   coordinator = get_corrected_coordinator(coordinator);
   if(!fake_flag) {
       if(is_coordinator_occupied(coordinator)) {
           return false;   
       }
        mark_rover_in_map(coordinator);
   }

   set_new_coordinator(rover, coordinator);
   rover->direction = get_direction_value(direction);
   rover->track = track_initialize();
    return true;
}

bool rover_move(rover_t *rover, coordinator_t *obstacle, bool fake_flag)
{
    coordinator_t new_coordinator;

    new_coordinator = get_neighbour_coordinator(rover->coordinator, rover->direction);

    if(!fake_flag) {
        if(is_coordinator_occupied(new_coordinator)) {
            if(obstacle) {
                obstacle->x = new_coordinator.x;
                obstacle->y = new_coordinator.y;
            }
            return false;   
        }

        move_in_map(rover->coordinator, new_coordinator);
    }

    track_insert(rover->track, rover->coordinator);
    set_new_coordinator(rover, new_coordinator);

    return true;
}

bool rover_turn_left(rover_t *rover)
{
    rover->direction = (rover->direction + 1) % 4;
    return true;
}

bool rover_turn_right(rover_t *rover)
{
    rover->direction = (4+rover->direction - 1) % 4;
    return true;
}

bool rover_accept_one_instruction(rover_t *rover, char instruction, coordinator_t *obstacle, bool fake_flag)
{
    switch (instruction) {
        case 'M':
            return rover_move(rover, obstacle, fake_flag);
            break;
        case 'L':
            return rover_turn_left(rover);
            break;
        case 'R':
            return rover_turn_right(rover);
            break;
        default:
            break;

    }
    return false;
}

bool is_move_command(char cmd)
{
    return (cmd == 'M');
}

void set_new_coordinator(rover_t *rover, coordinator_t new_coordinator)
{
    rover->coordinator.x = new_coordinator.x;
    rover->coordinator.y = new_coordinator.y;
}

void copy_rover_next_state(rover_t *rover_in_next_coordinator, rover_t *fake)
{
    set_new_coordinator(rover_in_next_coordinator, fake->coordinator);
    rover_in_next_coordinator->direction = fake->direction;
}

bool get_next_coordinator(rover_t *rover, char *cmd, int pos, int *delta, rover_t *rover_in_next_coordinator)
{
    int i;
    int count = 0;
    rover_t fake_rover;

    rover_initialize(&fake_rover, rover->coordinator, get_direction_name(rover->direction), true);

    for(i=pos; i<strlen(cmd); i++) {
        rover_accept_one_instruction(&fake_rover, cmd[i], NULL, true);
        if(is_move_command(cmd[i])) {
            count ++;
            if(count == 2) break;
        }
    }
    if(count == 2) {
        if(is_coordinator_occupied(fake_rover.coordinator)) {
            rover_destroy(&fake_rover, true);
            return false;
        }
        *delta = i - pos;
        copy_rover_next_state(rover_in_next_coordinator, &fake_rover);

        rover_destroy(&fake_rover, true);
        return true;
    }
    rover_destroy(&fake_rover, true);
    return false;
}


bool run_go_around_commands(rover_t *rover, stack<char> *commands, coordinator_t *obstacle) 
{
    int rc;
    char cmd;

    while(!commands->empty()) {
        cmd = commands->top();
        commands->pop();

        rc = rover_accept_one_instruction(rover, cmd, obstacle, false);
        if(!rc) break;
    }
    return rc;
}

bool go_around_to_next_coordinator(rover_t *rover, rover_t *rover_in_next_coordinator, coordinator_t *obstacle);

bool go_around_the_obstacle(rover_t *rover,  char *cmd, int pos, int *delta, coordinator_t *obstacle)
{
    rover_t rover_in_next_coordinator;

    if(!get_next_coordinator(rover, cmd, pos, delta, &rover_in_next_coordinator))
        return false;

    return go_around_to_next_coordinator(rover, &rover_in_next_coordinator, obstacle);
}

bool go_around_to_next_coordinator(rover_t *rover, rover_t *rover_in_next_coordinator, coordinator_t *obstacle)
{
    stack<char> commands;

    if(!get_commands_by_find_shortest_path(rover, rover_in_next_coordinator, &commands))
        return false;   

    return run_go_around_commands(rover, &commands, obstacle); 
}

bool still_have_move_instructions(char *cmd, int pos)
{
    for(int i=pos; i<strlen(cmd); i++) {
        if(is_move_command(cmd[i])) {
            return true;
        }
    }
    return false;
}

bool rover_execute_instructions(rover_t *rover, char *cmd, coordinator_t *obstacle)
{
    int delta = 0;

    if((!rover) || (!cmd)) {
        return false;
    }

    for(int i=0; i<strlen(cmd);) {
        if(!rover_accept_one_instruction(rover, cmd[i], obstacle, false)) {
            if(!still_have_move_instructions(cmd, i+1)) 
                return false;

            if(!go_around_the_obstacle(rover, cmd, i, &delta, obstacle))
                return false;

            i += delta+1;
            continue;
        }
        i++;
    }
    return true;
}

void rover_get_track(rover_t *rover, coordinator_t *coordinators, int *num)
{
    int tmp;
    if((!rover) || (!coordinators) || (!num)) {
        return;
    }

    tmp = *num - 1;
    track_read(rover->track, coordinators, &tmp);

    coordinators[tmp].x = rover->coordinator.x;
    coordinators[tmp].y = rover->coordinator.y;
    *num = tmp+1;

    return;
}

void rover_destroy(rover_t *rover, bool fake_flag)
{
    if(rover) {
        if(!fake_flag) {
            clear_in_map(rover->coordinator);
        }
        track_destroy(rover->track);
    }
}
