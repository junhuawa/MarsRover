#include <limits.h>
#include "map.h"
#include "gtest/gtest.h"

TEST(MapTest, map_init_fail_if_size_is_not_positive_integer) {
    EXPECT_FALSE(map_initialize(0, 0));
    EXPECT_FALSE(map_initialize(-1, 0));
    EXPECT_FALSE(map_initialize(-1, -1));
}

//
TEST(MapTest, map_init_success_if_size_is_positive_integer) {
    EXPECT_TRUE(map_initialize(5, 5));
    EXPECT_TRUE(map_destroy());

}

TEST(MapTest, map_set_position_out_of_border_but_still_valid) {

    coordinator_t coordinator;
    EXPECT_EQ(true, map_initialize(5, 5));

    coordinator.x = -1;
    coordinator.y = 0;
    coordinator = get_corrected_coordinator(coordinator);
    EXPECT_EQ(coordinator.x, 5);
    EXPECT_EQ(coordinator.y, 0);

    coordinator.x = 0;
    coordinator.y = -1;
    coordinator = get_corrected_coordinator(coordinator);
    EXPECT_EQ(coordinator.x, 0);
    EXPECT_EQ(coordinator.y, 5);

    coordinator.x = 6;
    coordinator.y = 6;
    coordinator = get_corrected_coordinator(coordinator);
    EXPECT_EQ(coordinator.x, 0);
    EXPECT_EQ(coordinator.y, 0);
    EXPECT_TRUE(map_destroy());

}

TEST(MapTest, map_check_if_position_allowed) {
    coordinator_t coordinator;
    EXPECT_TRUE(map_initialize(5, 5));
   
    coordinator.x = 0;
    coordinator.y = 0;
    EXPECT_EQ(false, is_coordinator_occupied(coordinator));
    coordinator.x = 5;
    coordinator.y = 5;
    EXPECT_EQ(false, is_coordinator_occupied(coordinator));
    EXPECT_TRUE(map_destroy());

}

TEST(ManyRoversInMapTest, Position_not_allowed_if_already_occupied) {
    coordinator_t coordinator;
    EXPECT_EQ(true, map_initialize(5, 5));

    coordinator.x = 0;
    coordinator.y = 0;
    EXPECT_EQ(true, mark_rover_in_map(coordinator));
    EXPECT_EQ(true, is_coordinator_occupied(coordinator));

    coordinator.x = 5;
    coordinator.y = 5;
    EXPECT_EQ(true, mark_rover_in_map(coordinator));
    EXPECT_EQ(true, is_coordinator_occupied(coordinator));

    coordinator.x = 3;
    coordinator.y = 3;
    EXPECT_EQ(true, mark_rover_in_map(coordinator));
    EXPECT_EQ(true, is_coordinator_occupied(coordinator));
    EXPECT_TRUE(map_destroy());
}


