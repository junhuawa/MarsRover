#ifndef __DIRECT_H__
#define __DIRECT_H__

#include <stack>
#include <iostream>
#include <cstdlib>
using namespace std;
enum direction
{
    north,
    west,
    south,
    east
};


bool is_direction_allowed(char direction);
int get_direction_value(char direct);
char get_direction_name(int value);
bool push_turn_direction_commands(stack<char> *commands, char direction, char target_direction);
#endif
