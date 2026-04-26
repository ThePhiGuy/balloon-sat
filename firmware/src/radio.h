#ifndef RADIO_H
#define RADIO_H

#include "system.h"

/*
    Defines Radio Channel
*/
typedef enum {
    CH_VHF,
    CH_UHF
} radio_channel;

/*
    Defines routing to output.
    SAME_BAND is used for digipeating
    Other modes are for Crossband or beaconing
*/
typedef enum {
    ROUTE_NONE,
    ROUTE_VHF_TO_UHF,
    ROUTE_UHF_TO_VHF,
    ROUTE_SAME_BAND
} audio_routing_mode;

/* PTT Commands */
system_status radio_set_ptt(radio_channel channel, uint8_t new_status);
uint8_t radio_get_ptt(radio_channel channel);

/* Frequency Commands */
system_status radio_set_frequency(radio_channel channel, uint32_t new_freq);
uint32_t radio_get_frequency(radio_channel channel);

/* Squelch Commands */
system_status radio_set_squelch(radio_channel channel, uint8_t new_squelch);
uint8_t radio_get_squelch(radio_channel channel);
system_status radio_set_tx_CTCSS(radio_channel channel, uint8_t new_CTCSS);
system_status radio_set_rx_CTCSS(radio_channel channel, uint8_t new_CTCSS);
uint8_t radio_get_tx_CTCSS(radio_channel channel);
uint8_t radio_get_rx_CTCSS(radio_channel channel);


/* commands for audio control */
system_status radio_set_audio_route(audio_routing_mode);
system_status radio_set_tx_audio_level(radio_channel channel, uint8_t audio_level); // sets DAC output level
system_status radio_set_rx_volume(radio_channel channel, uint8_t volume);
uint8_t radio_get_rx_volume(radio_channel channel);
uint8_t radio_get_tx_volume(radio_channel channel);

/* Radio System-based commands */
system_status radio_sleep(radio_channel channel); // used to sleep a module
system_status radio_init(radio_channel channel); // used to wake radio from sleep
system_status radio_system_init(); // used for "init" mode defaults on both radios   
system_status radio_verify_module(radio_channel channel); // used to check if module is functional


system_status check_radio_response(char* response); // used to check if a change was made




#endif