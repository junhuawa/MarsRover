#include <limits.h>
#include <stdio.h>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "gtest/gtest.h"

using namespace std;

class DirectionTest: public testing::Test{
    protected:
        virtual void SetUp() {
            map_initialize(5, 5);
        }

        virtual void TearDown() {
            map_destroy();
        }
};

TEST_F(DirectionTest, turn_direction_from_north_to_north) {
    stack<char> commands;
    push_turn_direction_commands(&commands, north, north);

    EXPECT_EQ(true, commands.empty());
}

TEST_F(DirectionTest, turn_direction_from_north_to_west) {
    stack<char> commands;
    push_turn_direction_commands(&commands, north, west);
    EXPECT_EQ('L', commands.top());
    commands.pop();
    EXPECT_EQ(true, commands.empty());
}

TEST_F(DirectionTest, turn_direction_from_north_to_east) {
    stack<char> commands;
    push_turn_direction_commands(&commands, north, east);
    EXPECT_EQ('R', commands.top());
    commands.pop();
    EXPECT_EQ(true, commands.empty());
}

TEST_F(DirectionTest, turn_direction_from_east_to_west) {
    stack<char> commands;
    push_turn_direction_commands(&commands, east, west);
    EXPECT_EQ('L', commands.top());
    commands.pop();
    EXPECT_EQ('L', commands.top());
    commands.pop();
    EXPECT_EQ(true, commands.empty());
}
