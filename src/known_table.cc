#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "known_table.h"

char *known_table_initialize()
{
    char *known_table;

    known_table =(char *) malloc((get_map_height()+1) * (get_map_width()+1));
    if(!known_table) 
        return NULL;
    memset(known_table, false, (get_map_height()+1) * (get_map_width()+1));

    return known_table;
}

bool set_coordinator_known(char * known_table, coordinator_t coordinator)
{
    known_table[coordinator.y * (get_map_width()+1) + coordinator.x] = true;
    return true;
}

bool is_coordinator_known(char * known_table, coordinator_t coordinator)
{
    return(known_table[coordinator.y * (get_map_width()+1) + coordinator.x]);
}

void known_table_destroy(char *table)
{
    if(table) {
        free(table);
    }
}
