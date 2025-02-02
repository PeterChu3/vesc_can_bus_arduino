#include <mcp_can.h>
#include <SPI.h>
#include "buffer.h"
#include "vesc_can_bus_arduino.h"

MCP_CAN CAN0(10); // Set CS to pin 10

void CAN::initialize()
{
  CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ);
  CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
  // SET PINMODE DONE IN MAIN // pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
}

void CAN::spin()
{
  get_frame();
  /*
    if (rxId == 0x8000090A)
    { // Can status message 1
      dutyCycleNow = buffer_get_int16(rxBuf, 6) *= 0.001;
      avgMotorCurrent = buffer_get_int16(rxBuf, 4) *= 0.1;
      erpm = buffer_get_int32(rxBuf, 0);
    }
    if (rxId == 0x80000E0A)
    { // Can status message 2
      // TODO Amp hours
      // TODO Amp hours charged
    }
    if (rxId == 0x80000F0A)
    { // Can status message 3
      // TODO Watt hours
      // TODO Watt hours charged
    }
    if (rxId == 0x8000100A)
    { // Can status message 4
      // TODO reimpliment code
      tempFET = process_data_frame_vesc('F', rxBuf[0], rxBuf[1]);
      tempMotor = process_data_frame_vesc('T', rxBuf[2], rxBuf[3]);
      avgInputCurrent = process_data_frame_vesc('I', rxBuf[4], rxBuf[5]);
      currentIn = 0;
      PID_positon = 0;
    }
    if (rxId == 0x80001B0A)
    { // Can status message 5
      // TODO reimpliment code
      char receivedByte[4], *p;
      sprintf(receivedByte, "%02X%02X", rxBuf[4], rxBuf[5]);
      inpVoltage = hex2int(receivedByte) * 0.1;
    }
*/
  if (rxId == 0x80003A0A)
  { // Can status message 6
    // ADC1, ADC2, ADC3, PPM
    int32_t send_index = 0;
    adc1 = process_data_frame_vesc('D', rxBuf[0], rxBuf[1]);
    adc2 = buffer_get_float16(rxBuf[2], 1000, 0);
    adc3 = buffer_get_float16(rxBuf[4], 1000, 0);
    ppm = buffer_get_float16(rxBuf[6], 1000, 0);
    Serial.print("ADC1: ");
    Serial.print(adc1);
    Serial.print("\t ADC2: ");
    Serial.print(adc2);
    Serial.print("\t ADC3: ");
    Serial.print(adc3);
    Serial.print("\t PPM: ");
    Serial.println(ppm);
  }

  // print_raw_can_data() // uncomment to see raw can messages
}

void CAN::print_raw_can_data()
{
  int len = 8;
  sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  Serial.print(msgString);
  for (byte i = 0; i < len; i++)
  {
    sprintf(msgString, " 0x%.2X", rxBuf[i]);
    Serial.print(msgString);
  }
  Serial.println();
}

float CAN::process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2)
{
  char receivedByte[4], *p;
  sprintf(receivedByte, "%02X%02X", byte1, byte2);
  float output = hex2int(receivedByte);

  switch (datatype)
  {
  case 'D':
    output *= 0.001;
    break; // dutyCycleNow
  case 'C':
    output *= 0.1;
    break; // avgMotorCurrent
  case 'F':
    output *= 0.1;
    break; // tempFET
  case 'T':
    output *= 0.1;
    break; // tempMotor
  case 'I':
    output *= 0.1;
    break; // avgInputCurrent
  case 'V':
    output *= 0.1;
    break; // inpVoltage
  }
  return output;
}

int CAN::hex2int(char buf[])
{
  return (short)strtol(buf, NULL, 16);
}

void CAN::vesc_set_duty(float duty)
{
  uint32_t set_value = duty * 100000;
  uint8_t buffer[4];
  buffer[0] = (set_value >> 24) & 0xFF;
  buffer[1] = (set_value >> 16) & 0xFF;
  buffer[2] = (set_value >> 8) & 0xFF;
  buffer[3] = set_value & 0xFF;
  byte sndStat = CAN0.sendMsgBuf(0x0000000A, 1, 4, buffer);
}

void CAN::vesc_set_current(float current)
{
  uint32_t set_value = current * 1000;
  uint8_t buffer[4];
  buffer[0] = (set_value >> 24) & 0xFF;
  buffer[1] = (set_value >> 16) & 0xFF;
  buffer[2] = (set_value >> 8) & 0xFF;
  buffer[3] = set_value & 0xFF;
  byte sndStat = CAN0.sendMsgBuf(0x0000010A, 1, 4, buffer);
}

void CAN::vesc_set_erpm(float erpm)
{
  uint32_t set_value = erpm;
  uint8_t buffer[4];
  buffer[0] = (set_value >> 24) & 0xFF;
  buffer[1] = (set_value >> 16) & 0xFF;
  buffer[2] = (set_value >> 8) & 0xFF;
  buffer[3] = set_value & 0xFF;
  byte sndStat = CAN0.sendMsgBuf(0x0000030A, 1, 4, buffer);
}
void CAN::get_frame()
{
  CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read data: len = data length, buf = data byte(s)
}
