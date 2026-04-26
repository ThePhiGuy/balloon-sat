#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "radio.h"

/*
    Defines system states for the finite state machine loop
*/
typedef enum {
    UNKNOWN,
    INIT,
    RX,
    PROCESS_PACKET,
    TX,
    FAILSAFE
} system_state;

/*
    Defines system operation modes
*/
typedef enum {
    MODE_BEACON,
    MODE_DIGIPEATER,
    MODE_CROSSBAND,
    MODE_IDLE
} system_mode;

/*
    Defines system policy for operating mode
*/
typedef enum {
    NORMAL,
    LOW_POWER,
    EMERGENCY
} system_policy;

/*
    Defines system radio for operating mode
*/
typedef enum {
    VHF_PRIMARY,
    UHF_PRIMARY,
} radio_behavior;

/*
    Defines "events" for state machine triggers
*/
typedef enum {
    EVENT_NONE,
    EVENT_TIMER,
    EVENT_RX_VHF,
    EVENT_RX_UHF,
    EVENT_TX_DONE,
} system_event;

/*
    Defines system command that can be recieved via command link
*/
typedef enum {
    CMD_NONE,
    CMD_DIGI_VHF,
    CMD_DIGI_UHF,
    CMD_CROSSBAND_V_U,
    CMD_CROSSBAND_U_V,
    CMD_LOW_POWER,
    CMD_EMERGENCY_DIGI,
    CMD_IDLE
} system_command;

/*
    Defines full system mode within current state.
*/
typedef struct {
    system_state state;
    system_mode mode;
    system_policy policy;
    radio_behavior radio;
} system_config;

/*
    Used for System Error status
*/
typedef enum {
    SYS_OK,
    SYS_ERROR,
    SYS_INVALID_STATE,
    SYS_INVALID_COMMAND,
    SYS_TIMEOUT,
    SYS_FAILSAFE_TRIGGERED
} system_status;

/*
    Struct that contains all current runtime information
*/
typedef struct {
    system_config current_config;
    system_state previous_state;

    radio_channel last_recv;
    packet rx_packet;
    packet tX_packet;
    uint8_t tx_pending;

    system_status last_status;
    
    uint32_t uptime_ms;
    uint32_t state_entry_time_ms;
    uint16_t battery_voltage_mv;
    uint8_t watchdog_active;
} system_context;

/*
    Struct for fault handling
*/
typedef struct {
    system_status code;
    uint32_t timestamp_ms;
    uint16_t extra_data;
} system_fault;

/*
    Basic struct for packet handling
*/
typedef struct {
    uint8_t data[350];
    uint16_t len;
    radio_channel source_radio;
} packet;

#endif