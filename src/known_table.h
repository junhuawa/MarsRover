#ifndef __KNOWN_TABLE_H__
#define __KNOWN_TABLE_H__

char *known_table_initialize();
bool is_coordinator_known(char * known_table, coordinator_t coordinator);
bool set_coordinator_known(char * known_table, coordinator_t coordinator);
void known_table_destroy(char *table);
#endif
