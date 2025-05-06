#ifndef TIPPING_BUCKET_COUNTER_H
#define TIPPING_BUCKET_COUNTER_H

#include <Arduino.h>
#include "driver/pcnt.h"

//ver3.0
#define BUCKET_COUNTER_PORT_NUM_A 14
#define BUCKET_COUNTER_PORT_NUM_B 15
#define BUCKET_COUNTER_PORT_NUM_C 4
#define BUCKET_COUNTER_PORT_NUM_D 16
#define BUCKET_COUNTER_PORT_NUM_E 17
#define BUCKET_COUNTER_PORT_NUM_F 5
#define BUCKET_COUNTER_PORT_NUM_G 18
#define BUCKET_COUNTER_PORT_NUM_H 36
#define BUCKET_COUNTER_PORT_NUM_CCK 27
#define BUCKET_COUNTER_PORT_NUM_CCLR 19

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

enum {
  MODE_BINARY_COUNTER,
  MODE_PULSE_COUNTER,
};

class TippingBucketCounter
{
  private:
  uint16_t i = 0;
  const int _counter_bits = 8;
  const int _number_of_GPIO = 10;
  int count_mode = MODE_BINARY_COUNTER;
  uint8_t _binary_counter_ports[NUMBER_OF_GPIO] = {BUCKET_COUNTER_PORT_NUM_A, BUCKET_COUNTER_PORT_NUM_B, BUCKET_COUNTER_PORT_NUM_C, BUCKET_COUNTER_PORT_NUM_D, BUCKET_COUNTER_PORT_NUM_E, BUCKET_COUNTER_PORT_NUM_F, BUCKET_COUNTER_PORT_NUM_G, BUCKET_COUNTER_PORT_NUM_H, BUCKET_COUNTER_PORT_NUM_CCK, BUCKET_COUNTER_PORT_NUM_CCLR};

  public:
    int16_t whole_counts_ = 0;
    int16_t whole_counts_last_time_ = 0;
    uint8_t overflow_counts_ = 0;
    int16_t difference_counts = 0;
    float volume_since_last_time = 0;
    float total_volume = 0;

    void begin(int16_t , uint8_t);
    void begin(int16_t , uint8_t, uint8_t, pcnt_unit_t, uint16_t pcnt_filter_value=0);
    void take_count();
    void take_count(pcnt_unit_t);
    void count_clear();
    void count_clear(pcnt_unit_t);
    void calculate_volume(float bucket_volume_ml);
    void debug();
};

#endif
