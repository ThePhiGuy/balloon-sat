#ifndef APRS_H
#define APRS_H

#include <stdint.h>
/*
    Represents an APRS Packet
*/
typedef struct {
    char src_callsign[7];      
    char dst_callsign[7];      
    char digipeater_path[64];  
    uint8_t control;           
    uint8_t pid;               
    uint8_t data_type_id;      
    uint8_t payload[256];      
    uint16_t payload_len;
    uint16_t crc;
} aprs_packet;

#endif