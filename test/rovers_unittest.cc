#include <limits.h>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "gtest/gtest.h"

class RoversTest: public testing::Test{
    protected:
        virtual void SetUp() {
            map_initialize(5, 5);
        }

        virtual void TearDown() {
            map_destroy();
        }
};

TEST_F(RoversTest, Initialize_failed_if_position_is_already_occupied) {

    rover_t rovers[4];
    coordinator_t coordinator;
    coordinator.x = 0;
    coordinator.y = 0;

    EXPECT_TRUE(rover_initialize(&rovers[0], coordinator, 'E', false));
    EXPECT_EQ(false, rover_initialize(&rovers[1], coordinator, 'N', false));
    EXPECT_EQ(false, rover_initialize(&rovers[2], coordinator, 'S', false));
    EXPECT_EQ(false, rover_initialize(&rovers[3], coordinator, 'W', false));

    for(int i=0; i<4; i++) {
        rover_destroy(&rovers[i], false);
    }
}

TEST_F(RoversTest, Rover_move_forward_failed_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {0,0 };
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'E', false));
    coordinator.x = 1;
    mark_rover_in_map(coordinator);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));
    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    EXPECT_EQ('E', get_direction_name(rover.direction));
        rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_move_out_of_border_failed_from_east_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {5,0 };
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'E', false));
    coordinator.x = 0;
    mark_rover_in_map(coordinator);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));
    EXPECT_EQ(5, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    EXPECT_EQ('E', get_direction_name(rover.direction));
    rover_destroy(&rover, false);
}
//   //
TEST_F(RoversTest, Rover_move_out_of_border_failed_from_west_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {0,0 };

    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'W', false));
    coordinator.x = 5;
    mark_rover_in_map(coordinator);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));
    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    EXPECT_EQ('W', get_direction_name(rover.direction));
    rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_move_out_of_border_failed_from_north_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {0,5 };
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'N', false));
    coordinator.y = 0;
    mark_rover_in_map(coordinator);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));
    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(5, rover.coordinator.y);
    EXPECT_EQ('N', get_direction_name(rover.direction));
    rover_destroy(&rover, false);
}

TEST_F(RoversTest, Rover_move_out_of_border_failed_from_south_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {0,0 };
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'S', false));
    coordinator.y = 5;
    mark_rover_in_map(coordinator);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));
    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    EXPECT_EQ('S', get_direction_name(rover.direction));
        rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_move_failed_becauseof_collision) {

    rover_t rover;
    coordinator_t coordinator = {5, 0};
    char cmds[] = "MMMLM";

    rover_initialize(&rover, coordinator, 'E', false);
    coordinator.x = 2;
    coordinator.y = 2;
    mark_rover_in_map(coordinator);

    rover_execute_instructions(&rover, cmds, NULL);

    EXPECT_EQ(false, rover_accept_one_instruction(&rover, 'M', NULL, false));

    EXPECT_EQ(2, rover.coordinator.x);
    EXPECT_EQ(1, rover.coordinator.y);
    EXPECT_EQ('N', get_direction_name(rover.direction));

    rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_report_obstacle_position_when_collide) {
    rover_t rover;
    coordinator_t obstacle_coordinator;
    bool rc;
    char move[] = "LMMLMMLMMLMMM";

    coordinator_t coordinator = {1, 3};
    mark_rover_in_map(coordinator);

    coordinator.x = 1;
    coordinator.y = 2;
    rover_initialize(&rover, coordinator, 'N', false);

    rc = rover_execute_instructions(&rover, move, &obstacle_coordinator);

    EXPECT_EQ(false, rc);
    EXPECT_TRUE(rover.coordinator.x== 1);
    EXPECT_TRUE(rover.coordinator.y== 2);

    EXPECT_TRUE(obstacle_coordinator.x== 1);
    EXPECT_TRUE(obstacle_coordinator.y== 3);
    rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_execute_cmds_although_no_obstacle) {
    rover_t rover;
    int rc;
    char move[] = "LMMLMMLMMLMMM";
    coordinator_t coordinator;

    rover_initialize(&rover, coordinator, 'N', false);
    rc = rover_execute_instructions(&rover, move, NULL);
    EXPECT_EQ(true, rc);
    rover_destroy(&rover, false);
}
//
TEST_F(RoversTest, Rover_get_correct_track) {
    rover_t rover;
    coordinator_t results[20]= {
        {1,2},
        {0,2},
        {5,2},
        {5,1},
        {5,0},
        {0,0},
        {1,0},
        {1,1},
        {1,2},
        {1,3},
    };
    coordinator_t coordinators[20];
    int num = 20;
    char move[] = "LMMLMMLMMLMMM";
    coordinator_t coordinator= {1, 2};

    rover_initialize(&rover, coordinator, 'N', false);
    rover_execute_instructions(&rover, move, NULL);
    rover_get_track(&rover, coordinators, &num);
    for(int i=0; i<10; i++) {
        EXPECT_EQ(coordinators[i].x, results[i].x);
        EXPECT_EQ(coordinators[i].y, results[i].y);
    }

    rover_destroy(&rover, false);
}
