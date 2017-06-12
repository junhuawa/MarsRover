#ifndef __ROVER_H__
#define __ROVER_H__
#include "track.h"

typedef struct rover
{
    coordinator_t coordinator;
    int direction;
    Track track;
} rover_t;


bool rover_initialize(rover_t *rover, coordinator_t coordinator, char direction, bool fake_flag);
bool rover_execute_instructions(rover_t *rover, char *cmd, coordinator_t *coordinator);
bool rover_accept_one_instruction(rover_t *rover, char instruction, coordinator_t *coordinator, bool fake_flag);
void rover_get_track(rover_t *rover, coordinator_t *coordinators, int *num);
void rover_destroy(rover_t *rover, bool fake_flag);

#endif
