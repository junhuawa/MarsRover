#include <limits.h>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "shortestpath.h"
#include "gtest/gtest.h"

class ShortestpathTest: public testing::Test{
    protected:
        virtual void SetUp() {
            map_initialize(5, 5);
        }

        virtual void TearDown() {
            map_destroy();
        }
};

TEST_F(ShortestpathTest, Find_Correct_path_when_there_is_a_obstacle) {
    rover_t rover;
    rover_t rover_destination;
    rover_t rover_obstacle;
    stack<char> commands;
    int rc;
    char cmds[] = "LMRMMRMLL";
    coordinator_t coordinator;

    coordinator.x = 1;
    coordinator.y = 2;
    rover_initialize(&rover, coordinator, 'E', false);
    coordinator.x = 2;
    coordinator.y = 2;
    rover_initialize(&rover_obstacle, coordinator, 'N', false);
    coordinator.x = 3;
    coordinator.y = 2;
    rover_initialize(&rover_destination, coordinator, 'N', false);

    rc = get_commands_by_find_shortest_path(&rover, &rover_destination, &commands);

    EXPECT_EQ(true, rc);
    EXPECT_EQ(9, commands.size());
    //printf("size: %d\n", commands.size());
    for(int i=0; i<9; i++) {
        EXPECT_EQ(cmds[i], commands.top());
        commands.pop();
    }

    rover_destroy(&rover, false);
    rover_destroy(&rover_obstacle, false);
    rover_destroy(&rover_destination, false);
}

void initialize_2_obstacles(rover_t rovers[2])
{
    coordinator_t coordinator= {2, 2};
    rover_initialize(&rovers[0], coordinator, 'N', false);
    coordinator.y = 3;
    rover_initialize(&rovers[1], coordinator, 'N', false);
}

TEST_F(ShortestpathTest, Find_Correct_path_when_there_are_another_obstacles) {
    rover_t rover;
    rover_t obstacles[2];
    rover_t rover_destination;
    stack<char> commands;
    int rc;
    char cmds[] = "LLMMMMR";
    coordinator_t coordinator;

    coordinator.x = 1;
    coordinator.y = 2;

    rover_initialize(&rover, coordinator, 'E', false);
    initialize_2_obstacles(obstacles);

    coordinator.x = 3;
    coordinator.y = 2;
    rover_initialize(&rover_destination, coordinator, 'N', false);

    rc = get_commands_by_find_shortest_path(&rover, &rover_destination, &commands);

    EXPECT_EQ(true, rc);
    EXPECT_EQ(7, commands.size());
    for(int i=0; i<7; i++) {
        EXPECT_EQ(cmds[i], commands.top());
        commands.pop();
    }

    rover_destroy(&rover, false);
    for(int i=0; i<2; i++) {
        rover_destroy(&obstacles[i], false);
    }
    rover_destroy(&rover_destination, false);
}

TEST_F(ShortestpathTest, Find_path_across_the_border) {
    rover_t rover;
    rover_t obstacles[6];
    rover_t rover_destination;
    stack<char> commands;
    char cmds[] = "LLMMMMR";
    int rc;
    coordinator_t coordinator;

    coordinator.x = 1;
    coordinator.y = 2;

    rover_initialize(&rover, coordinator, 'E', false);
    for(int i=0; i<6; i++) {
        coordinator.x = 2;
        coordinator.y = i;
        rover_initialize(&obstacles[i], coordinator, 'N', false);
    }

    coordinator.x = 3;
    coordinator.y = 2;
    rover_initialize(&rover_destination, coordinator, 'N', true);

    rc = get_commands_by_find_shortest_path(&rover, &rover_destination, &commands);

    EXPECT_EQ(true, rc);
    EXPECT_EQ(strlen(cmds), commands.size());
    for(int i=0; i<commands.size(); i++) {
        EXPECT_EQ(cmds[i], commands.top());
        commands.pop();
    }

    rover_destroy(&rover, false);
    for(int i=0; i<6; i++) {
        rover_destroy(&obstacles[i], false);

    }
    rover_destroy(&rover_destination, true);
}

TEST_F(ShortestpathTest, Find_path_failed_when_there_are_no_routes) {
    rover_t rover;
    rover_t obstacles[6];
    rover_t another_obstacles[6];
    rover_t rover_destination;
    stack<char> commands;
    int rc;
    coordinator_t coordinator;

    coordinator.x = 1;
    coordinator.y = 2;

    rover_initialize(&rover, coordinator, 'E', false);
    for(int i=0; i<6; i++) {
    coordinator.x = 2;
    coordinator.y = i;
        rover_initialize(&obstacles[i], coordinator, 'N', false);
    }

    for(int i=0; i<6; i++) {
    coordinator.x = 4;
    coordinator.y = i;
        rover_initialize(&another_obstacles[i], coordinator, 'N', false);
    }
    coordinator.x = 3;
    coordinator.y = 2;
    rover_initialize(&rover_destination, coordinator, 'N', false);

    rc = get_commands_by_find_shortest_path(&rover, &rover_destination, &commands);

    EXPECT_EQ(false, rc);
    EXPECT_EQ(0, commands.size());

    rover_destroy(&rover, false);
    for(int i=0; i<6; i++) {
        rover_destroy(&obstacles[i], false);
    }
    for(int i=0; i<6; i++) {
        rover_destroy(&another_obstacles[i], false);
    }
}
