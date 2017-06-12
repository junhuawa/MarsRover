#include <limits.h>
#include "track.h"
#include "gtest/gtest.h"

class TrackTest: public testing::Test{
    protected:
        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
};

TEST_F(TrackTest, Initialize_success) {
    Track track;

    track = track_initialize();
    EXPECT_TRUE(track != NULL);
    track_destroy(track);
}

TEST_F(TrackTest, Track_have_no_coordinator) {
    Track track;
    int num = 5;
    coordinator_t result[5];

    track = track_initialize();
    track_read(track, result, &num);
    EXPECT_TRUE(track != NULL);
    EXPECT_EQ(0, num);
    track_destroy(track);
}

TEST_F(TrackTest, Track_insert_one_coordinator_success) {
    Track track;
    int num = 5;
    coordinator_t coordinator = {
        1, 2
    };
    coordinator_t result[5];

    track = track_initialize();
    track_insert(track, coordinator);
    track_read(track, result, &num);

    EXPECT_EQ(1, result[0].x);
    EXPECT_EQ(2, result[0].y);
    EXPECT_EQ(1, num);

    track_destroy(track);
}

TEST_F(TrackTest, Track_insert_two_coordinators_success) {
    Track track;
    int num = 5;
    coordinator_t coordinator[2] = {
        {1, 2 }, {1, 3}
    };
    coordinator_t result[5];

    track = track_initialize();
    track_insert(track, coordinator[0]);
    track_insert(track, coordinator[1]);
    track_read(track, result, &num);

    EXPECT_EQ(1, result[0].x);
    EXPECT_EQ(2, result[0].y);
    EXPECT_EQ(1, result[1].x);
    EXPECT_EQ(3, result[1].y);
    EXPECT_EQ(2, num);

    track_destroy(track);
}

TEST_F(TrackTest, Track_insert_three_coordinators_success) {
    Track track;
    int num = 5;
    coordinator_t coordinator[3] = {
        {1, 2 }, {1, 3}, {0, 3}
    };
    coordinator_t result[5];

    track = track_initialize();
    track_insert(track, coordinator[0]);
    track_insert(track, coordinator[1]);
    track_insert(track, coordinator[2]);
    track_read(track, result, &num);

    EXPECT_EQ(1, result[0].x);
    EXPECT_EQ(2, result[0].y);
    EXPECT_EQ(1, result[1].x);
    EXPECT_EQ(3, result[1].y);
    EXPECT_EQ(0, result[2].x);
    EXPECT_EQ(3, result[2].y);
    EXPECT_EQ(3, num);

    track_destroy(track);
}
