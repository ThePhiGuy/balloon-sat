#ifndef FSM_H
#define FSM_H

#include "system.h"


/*
    Initialize the system (called from INIT state)
*/
system_status system_init(void);

/*
    Main FSM loop - call this repeatedly
*/
system_status system_tick(system_event event);

/*
    Transition to a new mode
*/
system_status system_set_mode(system_mode new_mode, system_policy new_policy);

/*
    Process a received command
*/
system_status system_process_command(system_command cmd);

/*
    Get current system configuration
*/
system_config* system_get_config(void);

/*
    Get current system context
*/
system_context* system_get_context(void);

/*
    Trigger failsafe mode
*/
void system_failsafe(system_fault fault);

/*
    Check if mode transition is valid
*/
uint8_t system_is_valid_transition(system_mode from, system_mode to);

#endif