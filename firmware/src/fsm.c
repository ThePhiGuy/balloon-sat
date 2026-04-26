#include "fsm.h"
#include "system.h"
#include "radio.h"
#include <stdint.h>

/* ============================================================================
   Global System Context
   ============================================================================ */

static system_context context = {0};

/* ============================================================================
   System Initialization
   ============================================================================ */

/*
    Initialize the system (called from INIT state)
*/
system_status system_init(void) {
    // TODO: Implement system initialization
    // - Initialize hardware (radio, timers, GPIO)
    // - Set initial state to RX
    // - Initialize watchdog timer
    // - Set up interrupts
    return SYS_OK;
}

/* ============================================================================
   Main FSM Loop
   ============================================================================ */

/*
    Main FSM loop - call this repeatedly
*/
system_status system_tick(system_event event) {
    // TODO: Implement main FSM state machine
    // - Handle current state (INIT, RX, PROCESS_PACKET, TX, FAILSAFE)
    // - Process event
    // - Transition to next state
    // - Update context
    switch (context.current_config.state) {
        case (INIT):
            radio_system_init();
            // timer_init(); Timer for Beacon Time
            // gpio_init(); Sets up microcontroller pins for other controllers
            // watchdog_init(); sets up platform watchdog

            context.current_config.mode = MODE_BEACON;
            context.current_config.state = RX;
            context.current_config.policy = NORMAL;
            context.current_config.radio = VHF_PRIMARY;

            return SYS_OK;
        case (RX):
            switch (event) {
                case EVENT_TIMER:
                    // timer triggered for APRS
                    // aprs_generate_beacon();
                    context.previous_state = context.current_config.state;
                    context.current_config.state = TX;
                    break;
                case EVENT_RX_UHF:
                    // packet potentially recieved on UHF
                    if (context.current_config.mode == MODE_CROSSBAND && context.current_config.radio == CH_UHF) {
                        // valid key up of the radio
                        // pipe output to VHF input
                        context.previous_state = context.current_config.state;
                        context.current_config.state = TX;
                        break;
                    }
                    context.last_recv = CH_UHF;
                    context.previous_state = context.current_config.state;
                    context.current_config.state = PROCESS_PACKET;
                    break;
                case EVENT_RX_VHF:
                    // packet potentially recieved on VHF
                    if (context.current_config.mode == MODE_CROSSBAND && context.current_config.radio == CH_VHF) {
                        // valid key up of the radio
                        // pipe output to UHF input
                        context.previous_state = context.current_config.state;
                        context.current_config.state = TX;
                        break;
                    }
                    context.last_recv = CH_VHF;
                    context.previous_state = context.current_config.state;
                    context.current_config.state = PROCESS_PACKET;
                    break;
            }
            return SYS_OK;
        case (PROCESS_PACKET):
            switch (context.current_config.mode) {
                case MODE_DIGIPEATER:
                    if (context.last_recv == context.current_config.radio) {
                        // this is through the digipeater path.
                        // process and check packet for proper via
                        switch (context.current_config.policy) {
                            case EMERGENCY:
                                // Wide 1-1 Path
                                // check_valid_packet()
                                // if in_path("WIDE1-1" | "BALLOON") then:
                                // add onto TX stack
                                context.current_config.state = TX;
                                break;
                            case NORMAL:
                                // normal path (VIA BALLOON or w/e)
                                // check_valid_packet()
                                // if in_path("BALLOON") then Digipeat
                                // via adding the packet to the next tx
                                context.current_config.state = TX;
                                break;
                        }
                    } else {
                        // assume packet is a command packet
                        // check_valid_packet();
                        // check_for_command_and_validate();
                        // execute_command();
                        // tx_next = ack;
                        // next mode = tx;
                        break;
                    }
                    break;
                case MODE_BEACON:
                    if (context.last_recv == CH_UHF) {
                        // assume packet is a command packet
                        // check_valid_packet();
                        // check_for_command_and_validate();
                        // execute_command();
                        // tx_next = ack;
                        // next mode = tx;
                    }
                    // if not recieved on the command link side, discard.
                    break;  
            }
            break;
        case (TX):
            switch (context.current_config.mode) {
                case MODE_CROSSBAND:
                    // audio path is already set via the mode transition
                    // all that needs to be done is to key up the radio and wait to unkey
                    // the radio until the squelch is done on the RX side.
                    switch (context.current_config.radio) {
                        case VHF_PRIMARY:
                            radio_set_ptt(CH_VHF, 1);
                            break;
                        case UHF_PRIMARY:
                            radio_set_ptt(CH_UHF, 1);
                            break;
                    }

                    //  while (waiting for no more RX CTCSS Tone) -> do nothing
                    // I'll have to find an elegant solution for this, monitoring the SQ pin?

                    switch (context.current_config.radio) {
                        case VHF_PRIMARY:
                            radio_set_ptt(CH_VHF, 0);
                            break;
                        case UHF_PRIMARY:
                            radio_set_ptt(CH_UHF, 0);
                            break;
                    }
                    context.current_config.state = RX;
                case MODE_DIGIPEATER:
            }
            break;
        case (FAILSAFE):
            return SYS_FAILSAFE_TRIGGERED;
    }
    return SYS_OK;
}

/* ============================================================================
   Mode Management
   ============================================================================ */

/*
    Transition to a new mode
*/
system_status system_set_mode(system_mode new_mode, system_policy new_policy) {
    // TODO: Implement mode transition
    // - Check if transition is valid
    // - Update system_context.current_config
    // - Record transition time
    // - Apply mode-specific initialization
    return SYS_OK;
}

/*
    Check if mode transition is valid
*/
uint8_t system_is_valid_transition(system_mode from, system_mode to) {
    // TODO: Implement transition validation
    // - Check mode transition table
    // - Return 1 if valid, 0 if invalid
    // - Consider current policy constraints
    return 1;
}

/* ============================================================================
   Command Processing
   ============================================================================ */

/*
    Process a received command
*/
system_status system_process_command(system_command cmd) {
    // TODO: Implement command processing
    // - Validate command is allowed in current mode
    // - Check if command source is authenticated
    // - Map command to target mode/policy
    // - Call system_set_mode() if transition valid
    return SYS_OK;
}

/* ============================================================================
   Context and Configuration Access
   ============================================================================ */

/*
    Get current system configuration
*/
system_config* system_get_config(void) {
    // TODO: Return pointer to current configuration
    return &context.current_config;
}

/*
    Get current system context (runtime state)
*/
system_context* system_get_context(void) {
    // TODO: Return pointer to system context
    return &context;
}

/* ============================================================================
   Failsafe and Error Handling
   ============================================================================ */

/*
    Trigger failsafe mode
*/
void system_failsafe(system_fault fault) {
    // TODO: Implement failsafe mode
    // - Update last_fault in context
    // - Increment fault counter
    // - Disable TX on both radios
    // - Shutdown both radios
    // - Set state to FAILSAFE
    // - Record timestamp
}