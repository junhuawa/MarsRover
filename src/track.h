#ifndef __LIST_H__
#define __LIST_H__
#include "coordinator.h"
typedef struct Node * PtrToNode;
typedef PtrToNode Position;
struct Node
{
    coordinator_t coordinator;
    Position Next;
};

typedef struct TrackRecord *Track;
struct TrackRecord
{
    PtrToNode start;
    PtrToNode end;
};

Track track_initialize(void);
void track_insert(Track track, coordinator_t coordinator);
bool append_new_track(Track original, Track delta_track);
void track_read(Track track, coordinator_t *coordinators, int *num);
void track_destroy(Track track);

#endif
