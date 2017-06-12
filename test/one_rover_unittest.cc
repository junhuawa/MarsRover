#include <limits.h>
#include <stdio.h>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "gtest/gtest.h"

class roverTest: public testing::Test{
    protected:
        rover_t rover;
        virtual void SetUp() {
            map_initialize(5, 5);
        }

        virtual void TearDown() {
            map_destroy();
        }
};

TEST_F(roverTest, Rover_initialize_success_even_position_is_out_of_border) {
    coordinator_t coordinator= {-1, 0};

    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'N', false));
    EXPECT_EQ(5, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    rover_destroy(&rover, false);

    coordinator.x = 7;
    coordinator.y = 0;
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'N', false));
    rover_destroy(&rover, false);

    coordinator.x = 5;
    coordinator.y = 5;
    EXPECT_EQ(true, rover_initialize(&rover, coordinator, 'N', false));
    rover_destroy(&rover, false);

    coordinator.x = 6;
    coordinator.y = 6;
    EXPECT_TRUE(rover_initialize(&rover, coordinator, 'N', false));
    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_initialize_failed_if_direction_is_not_allowed) {
    coordinator_t coordinator= {0, 0};

    EXPECT_FALSE(rover_initialize(&rover, coordinator, 'X', false));

    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_init_success) {
    rover_t rovers[4];
    coordinator_t coordinators[4] = {
        {0, 0},
        {0, 1},
        {0, 2},
        {0, 3},
    };

    EXPECT_TRUE(rover_initialize(&rovers[0], coordinators[0], 'E', false));
    EXPECT_TRUE(rover_initialize(&rovers[1], coordinators[1], 'N', false));
    EXPECT_TRUE(rover_initialize(&rovers[2], coordinators[2], 'S', false));
    EXPECT_TRUE(rover_initialize(&rovers[3], coordinators[3], 'W', false));

    for(int i=0; i<4; i++) {
        rover_destroy(&rovers[i], false);
    }
}

TEST_F(roverTest, Rover_move_forward_north_one_step) {
    coordinator_t coordinators[1];
    coordinator_t coordinator = {0, 0};
    int num =5;

    rover_initialize(&rover, coordinator, 'N', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);
    track_read(rover.track, coordinators, &num);

    EXPECT_TRUE(rover.coordinator.x== 0);
    EXPECT_TRUE(rover.coordinator.y== 1);

    EXPECT_EQ(0, coordinators[0].x);
    EXPECT_EQ(0, coordinators[0].y);
    EXPECT_EQ(1, num);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'N');
    rover_destroy(&rover, false);
}
//
TEST_F(roverTest, Rover_move_forward_east_one_step) {
    coordinator_t coordinator = {0, 0};

    rover_initialize(&rover, coordinator, 'E', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);

    EXPECT_TRUE(rover.coordinator.x== 1);
    EXPECT_TRUE(rover.coordinator.y== 0);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'E');

    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_move_forward_west_one_step) {
    coordinator_t coordinator = {5, 0};

    rover_initialize(&rover, coordinator, 'W', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);

    EXPECT_TRUE(rover.coordinator.x== 4);
    EXPECT_TRUE(rover.coordinator.y== 0);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'W');

    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_move_forward_south_one_step) {
    coordinator_t coordinator = {5, 5};

     rover_initialize(&rover, coordinator, 'S', false);
     rover_accept_one_instruction(&rover, 'M', NULL, false);

     EXPECT_TRUE(rover.coordinator.x== 5);
     EXPECT_TRUE(rover.coordinator.y== 4);
     EXPECT_TRUE(get_direction_name((&rover)->direction)== 'S');

    rover_destroy(&rover, false);
}

// 
TEST_F(roverTest, Rover_move_out_of_border_from_east) {
    coordinator_t coordinator = {5, 0};

    rover_initialize(&rover, coordinator, 'E', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);

    EXPECT_EQ(0, rover.coordinator.x);
    EXPECT_EQ(0, rover.coordinator.y);
    EXPECT_EQ('E', get_direction_name(rover.direction));

    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_move_out_of_border_from_north) {
    coordinator_t coordinator = {5, 5};

    rover_initialize(&rover, coordinator, 'N', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);

    EXPECT_TRUE(rover.coordinator.x== 5);
    EXPECT_TRUE(rover.coordinator.y== 0);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'N');

    rover_destroy(&rover, false);
}

TEST_F(roverTest, Rover_move_out_of_boarder_from_south) {
    coordinator_t coordinator = {4, 0};

    rover_initialize(&rover, coordinator, 'S', false);
    rover_accept_one_instruction(&rover, 'M', NULL, false);

    EXPECT_TRUE(rover.coordinator.x== 4);
    EXPECT_TRUE(rover.coordinator.y== 5);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'S');

    rover_destroy(&rover, false);
}
//
TEST_F(roverTest, Rover_move_out_of_boarder_from_west) {
    coordinator_t coordinator = {0, 4};

    rover_initialize(&rover, coordinator, 'W', false);
    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'M', NULL, false));

    EXPECT_TRUE(rover.coordinator.x== 5);
    EXPECT_TRUE(rover.coordinator.y== 4);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'W');

    rover_destroy(&rover, false);
}

// 
// 
TEST_F(roverTest, Rover_accept_turn_left_instruction_from_4_different_directions) {
    coordinator_t coordinator = {0, 0};

    rover_initialize(&rover, coordinator, 'N', false);
    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'L', NULL, false));

    EXPECT_TRUE(rover.coordinator.x== 0);
    EXPECT_TRUE(rover.coordinator.y== 0);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'W');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'L', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'S');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'L', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'E');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'L', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'N');

    rover_destroy(&rover, false);
}
//
TEST_F(roverTest, Rover_accept_turn_right_instruction_from_4_different_directions) {
    coordinator_t coordinator = {0, 0};

    rover_initialize(&rover, coordinator, 'N', false);
    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'R', NULL, false));

    EXPECT_TRUE(rover.coordinator.x== 0);
    EXPECT_TRUE(rover.coordinator.y== 0);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'E');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'R', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'S');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'R', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'W');

    EXPECT_TRUE(rover_accept_one_instruction(&rover, 'R', NULL, false));
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'N');

    rover_destroy(&rover, false);
}

//
TEST_F(roverTest, Rover_accept_4_instructions_to_turn_around) {
    char move[] = "RRRR";
    coordinator_t coordinator = {3, 3};

    rover_initialize(&rover, coordinator, 'E', false);
    for(int i=0; i<strlen(move); i++) {
        rover_accept_one_instruction(&rover, move[i], NULL, false);
    }

    EXPECT_TRUE(rover.coordinator.x== 3);
    EXPECT_TRUE(rover.coordinator.y== 3);
    EXPECT_TRUE(get_direction_name((&rover)->direction)== 'E');

    rover_destroy(&rover, false);
}
