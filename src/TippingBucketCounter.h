#ifndef TIPPING_BUCKET_COUNTER_H
#define TIPPING_BUCKET_COUNTER_H

#include <Arduino.h>
#include "driver/pcnt.h"

extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_A;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_B;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_C;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_D;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_E;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_F;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_G;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_H;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_CCK;
extern const uint8_t BUCKET_COUNTER_BINARY_PORT_NUM_CCLR;

#define BUCKET_MAX_VOLUME_ML 50
#define PCNT_MAX_COUNT 32767 // Maximum count value for PCNT
#define NUMBER_OF_GPIO 10
enum
{
  PORT_A_INDEX,
  PORT_B_INDEX,
  PORT_C_INDEX,
  PORT_D_INDEX,
  PORT_E_INDEX,
  PORT_F_INDEX,
  PORT_G_INDEX,
  PORT_H_INDEX,
  PORT_CCK_INDEX,
  PORT_CCLR_INDEX,
};

enum
{
  MODE_BINARY_COUNTER,
  MODE_PULSE_COUNTER,
};

class TippingBucketCounter
{
private:
  uint16_t i = 0;
  const int _counter_bits = 8;
  const int _number_of_GPIO = 10;
  uint8_t _binary_counter_ports[NUMBER_OF_GPIO];
  pcnt_unit_t _pcnt_unit = PCNT_UNIT_0;
  
  public:
  int count_mode = MODE_BINARY_COUNTER;
  int16_t whole_counts_ = 0;
  int16_t whole_counts_last_time_ = 0;
  uint8_t overflow_counts_ = 0;
  int16_t difference_counts = 0;
  float volume_since_last_time = 0;
  float total_volume = 0;

  TippingBucketCounter();
  void begin(int16_t, uint8_t);
  void begin(int16_t, uint8_t, uint8_t, pcnt_unit_t, uint16_t pcnt_filter_value = 0);
  void take_count();
  void count_clear();
  void calculate_volume(float bucket_volume_ml);
  void debug();
};

#endif
