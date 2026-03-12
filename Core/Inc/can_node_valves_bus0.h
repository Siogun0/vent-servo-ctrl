/* Do not edit */
/* This file is generated automatically by dbc_conv */

#ifndef __CAN_NODE_VALVES_BUS0_H__
#define __CAN_NODE_VALVES_BUS0_H__
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <can_platform.h>

#define ALIVE_THRESHOLD 10


//CAN_SIGNAL_VALVE_1
#define CAN_SIGNAL_VALVE_1_UNDEFINED  0
#define CAN_SIGNAL_VALVE_1_OPENED  1
#define CAN_SIGNAL_VALVE_1_CLOSED  2
#define CAN_SIGNAL_VALVE_1_PARTIALLY  3
#define CAN_SIGNAL_VALVE_1_MOVING  4
#define CAN_SIGNAL_VALVE_1_SHORTCIRCUIT  5
#define CAN_SIGNAL_VALVE_1_OPENCIRCUIT  6


//CAN_SIGNAL_VALVE_2
#define CAN_SIGNAL_VALVE_2_UNDEFINED  0
#define CAN_SIGNAL_VALVE_2_OPENED  1
#define CAN_SIGNAL_VALVE_2_CLOSED  2
#define CAN_SIGNAL_VALVE_2_PARTIALLY  3
#define CAN_SIGNAL_VALVE_2_MOVING  4
#define CAN_SIGNAL_VALVE_2_SHORTCIRCUIT  5
#define CAN_SIGNAL_VALVE_2_OPENCIRCUIT  6


//CAN_SIGNAL_VALVE_3
#define CAN_SIGNAL_VALVE_3_UNDEFINED  0
#define CAN_SIGNAL_VALVE_3_OPENED  1
#define CAN_SIGNAL_VALVE_3_CLOSED  2
#define CAN_SIGNAL_VALVE_3_PARTIALLY  3
#define CAN_SIGNAL_VALVE_3_MOVING  4
#define CAN_SIGNAL_VALVE_3_SHORTCIRCUIT  5
#define CAN_SIGNAL_VALVE_3_OPENCIRCUIT  6


//CAN_SIGNAL_VALVE_4
#define CAN_SIGNAL_VALVE_4_UNDEFINED  0
#define CAN_SIGNAL_VALVE_4_OPENED  1
#define CAN_SIGNAL_VALVE_4_CLOSED  2
#define CAN_SIGNAL_VALVE_4_PARTIALLY  3
#define CAN_SIGNAL_VALVE_4_MOVING  4
#define CAN_SIGNAL_VALVE_4_SHORTCIRCUIT  5
#define CAN_SIGNAL_VALVE_4_OPENCIRCUIT  6

typedef struct
{
    uint8_t ctrl_to_valve;
} t_can_node_valves_bus0_input_alive;

typedef struct
{
    struct
    {
        uint8_t VALVE_1_REQ;
        uint8_t VALVE_2_REQ;
        uint8_t VALVE_3_REQ;
        uint8_t VALVE_4_REQ;
    } CTRL_TO_VALVE;
    t_can_node_valves_bus0_input_alive alive;
} t_can_node_valves_bus0_input;

typedef struct
{
    uint32_t valve_status;
} t_can_node_valves_bus0_output_tx_now;

typedef struct
{
    struct
    {
        uint8_t VALVE_1;
        uint8_t VALVE_2;
        uint8_t VALVE_3;
        uint8_t VALVE_4;
        int16_t CPU_TEMP;
        float V_3V3;
        float V_12V;
        uint8_t CURRENT_SUM;
    } VALVE_STATUS;
    t_can_node_valves_bus0_output_tx_now tx_now;
} t_can_node_valves_bus0_output;

#define MBN_TX_VALVE_STATUS 0
#define MBN_RX_CTRL_TO_VALVE 1

#define MBN_FIRST_BUS_0 0
#define MBN_TOTAL_BUS_0 2
#define MBN_NEXT_FREE_BUS_0 2

#ifndef CAN_EID_FLG
#define CAN_EID_FLG 0x80000000UL
#endif

#ifndef __MAILBOX_CALLBACK_TYPE__
#define __MAILBOX_CALLBACK_TYPE__
typedef void (*t_mailbox_callback)(uint32_t id, uint64_t msg, uint32_t dlc);
#endif

#define MSG_CYCLE_VALVE_STATUS 100000
#define MSG_CYCLE_CTRL_TO_VALVE 100000

//====== API functions ======
void can_node_valves_bus0_tx(volatile t_can_node_valves_bus0_output *out);
void can_node_valves_bus0_rx(volatile t_can_node_valves_bus0_input *inp);
void can_node_valves_bus0_update_timers(uint32_t time_delta_us);
uint8_t can_node_valves_bus0_init(uint32_t mb_shift, uint32_t rx_id_shift, uint32_t tx_id_shift, volatile t_can_node_valves_bus0_output *out, volatile t_can_node_valves_bus0_input *in);

//====== Planform driver functions declaration ======
void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);
void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);
void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg);
uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn);
uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn);

//====== Callback functions ======
void can_node_valve_status_cb(uint32_t id, uint64_t msg, uint32_t dlc);

void can_node_valve_status_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc);

void can_node_ctrl_to_valve_cb(uint32_t id, uint64_t msg, uint32_t dlc);


#endif

