#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "track.h"

Track track_initialize(void)
{
    Track track;

    track = (Track) malloc(sizeof(struct TrackRecord));
    if(!track) 
        return NULL;

    track->start = NULL;
    track->end = NULL;

    return track;
}


void track_insert(Track track, coordinator_t coordinator)
{
    PtrToNode tmp = (PtrToNode)malloc(sizeof(struct Node));
    if(!tmp) return; 

    if(!track->start) {
        track->start = tmp;
        track->end = tmp;
    }else {
        track->end->Next = tmp;
        track->end = tmp;
    }
    track->end->Next = NULL;

    tmp->coordinator.x = coordinator.x;
    tmp->coordinator.y = coordinator.y;

    return;
}

bool append_new_track(Track original, Track delta_track)
{
    PtrToNode ptr;
    if((!original) ||(!delta_track)) return false;
    ptr = delta_track->start;
    while(ptr) {
        track_insert(original, ptr->coordinator);
        ptr = ptr->Next;
    }

    return true;
}

void track_read(Track track, coordinator_t *coordinators, int *num)
{
    int count =0;
    if(!track) return;

    PtrToNode ptr = track->start;
    while(ptr) {
        coordinators[count].x = ptr->coordinator.x;
        coordinators[count].y = ptr->coordinator.y;
        count ++;
        ptr = ptr->Next;

        if(count == *num) break;
    }
    *num = count;
   return; 
}

void track_destroy(Track track)
{
    PtrToNode ptr, tmp;
    if(track) {
        ptr = track->start;
        while(ptr) {
            tmp = ptr->Next;
            free(ptr);
            ptr = tmp;
        }

        free(track);
    }
    return;
}
