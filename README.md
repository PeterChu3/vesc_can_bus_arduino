**VESC library for CAN bus control using MCP2515 SPI Driver**

![image](https://github.com/craigg96/vesc_can_bus_arduino/blob/main/images/header.png?raw=true "Header")

This repo contains the CAN bus protocol for VESC that can be used with arduino and the MCP2515 driver library by Cory Fowler (see credits).
The MCP2515 is the most common way to integrate can bus onto the arduino platform. The VESC platform is the gold standard for brushless motor control for skateboards and ebikes. This library allows you to use the can bus instead of UART (see solidgeek vescuart library) to communicate with the VESC which is more reliable and allows the single UART port on most arduinos to be free for debugging.

## Realtime Data Messages

![image](https://github.com/peterchu3/vesc_can_bus_arduino/blob/main/images/CanMessages.jpg?raw=true "Header")

## Command Messages

| Parameter |
| :-------: |
|   erpm    |
| dutycycle |
|  current  |

To use the library out of the box, the vesc tool must be configured as shown in images/vesc_tool_app_settings.png.
CAN Baud Rate = CAN_BAUD_500K

# Credits

- Craig Gault - https://github.com/craigg96

DEPENDENCIES

- https://github.com/coryjfowler/MCP_CAN_lib
