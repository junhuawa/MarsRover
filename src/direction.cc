#include "direction.h"

char direction_names[4] = {'N', 'W', 'S', 'E'};

bool is_direction_allowed(char direction)
{
    for(int i=0; i<4; i++) {
        if(direction == direction_names[i]) {
            return true;
        }
    }
    return false;
}

int get_direction_value(char direct_name)
{
    int i=0;
    for(i=0; i<4; i++) {
        if(direct_name == direction_names[i]) {
            return i;
        }
    }
    return i;
}

char get_direction_name(int value)
{
    return direction_names[value];
}

bool push_turn_direction_commands(stack<char> *commands, char direction, char target_direction)
{
    int tmp;
    int count = 0;
    tmp = direction;
    while(tmp != target_direction) {
        tmp = (tmp + 1)%4;
        count ++;
    }

    if(count == 1) {
        commands->push('L');
    }
    if(count == 2) {
        commands->push('L');
        commands->push('L');
    }
    if(count == 3) {
        commands->push('R');
    }
    return true;
}

