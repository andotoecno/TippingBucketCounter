#ifndef TIPPING_BUCKET_COUNTER_H
#define TIPPING_BUCKET_COUNTER_H

#include <Arduino.h>

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

class TippingBucketCounter
{
  private:
  uint16_t i = 0;
  const int _counter_bits = 8;
  const uint8_t _binary_counter_ports[10] = {BUCKET_COUNTER_PORT_NUM_A, BUCKET_COUNTER_PORT_NUM_B, BUCKET_COUNTER_PORT_NUM_C, BUCKET_COUNTER_PORT_NUM_D, BUCKET_COUNTER_PORT_NUM_E, BUCKET_COUNTER_PORT_NUM_F, BUCKET_COUNTER_PORT_NUM_G, BUCKET_COUNTER_PORT_NUM_H, BUCKET_COUNTER_PORT_NUM_CCK, BUCKET_COUNTER_PORT_NUM_CCLR};

  public:
    int16_t whole_counts_ = 0;
    int16_t whole_counts_last_time_ = 0;
    uint8_t overflow_counts_ = 0;
    int16_t difference_counts = 0;
    float volume_since_last_time = 0;
    float total_volume = 0;
    float bucket_volume_ml = 0; // ml of once

    void begin(int16_t , uint8_t );
    void take_count();
    void count_clear();
    void calculate_volume();
    void debug();
};

#endif
