#include <limits.h>
#include "map.h"
#include "direction.h"
#include "rover.h"
#include "gtest/gtest.h"

class CollisionTest: public testing::Test{
    protected:
        virtual void SetUp() {
            map_initialize(5, 5);
        }

        virtual void TearDown() {
            map_destroy();
        }
};

static void initialize_2_obstacles(void)
{
    coordinator_t coordinator= {2, 2};

    mark_rover_in_map(coordinator);
    coordinator.x = 3;
    mark_rover_in_map(coordinator);
}

TEST_F(CollisionTest, Rover_failed_if_target_position_and_next_position_are_already_cooupied) {
    rover_t rover;
    coordinator_t coordinator;
    char move[] = "MMLMMLMMLMMM";

    coordinator.x = 1;
    coordinator.y = 2;
    rover_initialize(&rover, coordinator, 'E', false);
    initialize_2_obstacles();

    EXPECT_EQ(false, rover_execute_instructions(&rover, move, NULL));

    rover_destroy(&rover, false);
}

TEST_F(CollisionTest, Rover_go_around_the_obstacle_and_arrive_the_destination) {
   rover_t rover;
   coordinator_t coordinators[20];
   int num = 20;
   coordinator_t path[20]=
   {   {1,2},
       {1,3},
       {2,3},
       {3,3},
       {3,2},
       {3,3},
       {3,4},
       {2,4},
       {1,4},
       {1,3},
       {1,2},
       {1,1},
   };
   int rc;
   char move[] = "MMLMMLMMLMMM";
   coordinator_t coordinator;

   coordinator.x = 1;
   coordinator.y = 2;
   rover_initialize(&rover, coordinator, 'E', false);
   coordinator.x = 2;
   coordinator.y = 2;
   mark_rover_in_map(coordinator);

   rc = rover_execute_instructions(&rover, move, NULL);
   rover_get_track(&rover, coordinators, &num);

   EXPECT_EQ(12, num);
   for(int i=0; i<num; i++) {
        EXPECT_EQ(coordinators[i].x, path[i].x);
        EXPECT_EQ(coordinators[i].y, path[i].y);

   }

   EXPECT_EQ(true, rc);
   rover_destroy(&rover, false);
}

TEST_F(CollisionTest, Rover_go_around_obstacle_and_go_to_next_position) {
    rover_t rover;
    int rc;
    char move[] = "MM";
    coordinator_t coordinator;

    coordinator.x = 3;
    coordinator.y = 2;
    rover_initialize(&rover, coordinator, 'W', false);
    coordinator.x = 2;
    coordinator.y = 2;
    mark_rover_in_map(coordinator);

    rc = rover_execute_instructions(&rover, move, NULL);
    EXPECT_EQ(true, rc);
    rover_destroy(&rover, false);
}
