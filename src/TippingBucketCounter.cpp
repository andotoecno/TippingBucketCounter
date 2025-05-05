#include "TippingBucketCounter.h"

void TippingBucketCounter::begin(int16_t kept_whole_counts_last_time, uint8_t kept_overflow_counts)
{
  /*pin mode*/
  for (uint16_t i = 0; i < _counter_bits; i++)
    pinMode(_binary_counter_ports[i], INPUT);
  pinMode(_binary_counter_ports[PORT_CCK_INDEX], INPUT);
  pinMode(_binary_counter_ports[PORT_CCLR_INDEX], INPUT);
  /*take over last value*/
  whole_counts_last_time_ = kept_whole_counts_last_time;
  overflow_counts_ = kept_overflow_counts;
}

void TippingBucketCounter::take_count()
{
  whole_counts_ = 0;
  for (i = 0; i < _counter_bits; i++)
    whole_counts_ += digitalRead(_binary_counter_ports[i]) << i;

  difference_counts = whole_counts_ - whole_counts_last_time_;

  if (difference_counts < 0)
  {
    difference_counts += 256; /*not 255 because next count of 255 is 0(not 1)*/
    overflow_counts_++;
  }
  whole_counts_last_time_ = whole_counts_;
}

void TippingBucketCounter::count_clear()
{
  pinMode(_binary_counter_ports[PORT_CCLR_INDEX], OUTPUT);
  digitalWrite(_binary_counter_ports[PORT_CCLR_INDEX], 0);
  delay(100);
  digitalWrite(_binary_counter_ports[PORT_CCLR_INDEX], 1);
  delay(100);
  pinMode(_binary_counter_ports[PORT_CCLR_INDEX], INPUT);

  pinMode(_binary_counter_ports[PORT_CCK_INDEX], OUTPUT);
  delay(100);
  digitalWrite(_binary_counter_ports[PORT_CCK_INDEX], 1);
  delay(100);
  digitalWrite(_binary_counter_ports[PORT_CCK_INDEX], 0);
  pinMode(_binary_counter_ports[PORT_CCK_INDEX], INPUT);
}

void TippingBucketCounter::calculate_volume(float bucket_volume_ml)
{
  volume_since_last_time = (float)difference_counts * bucket_volume_ml;
  total_volume = ((float)whole_counts_ + (float)overflow_counts_ * 256.0) * bucket_volume_ml;
}

void TippingBucketCounter::debug()
{
  String str_buf = "";

  Serial.print("Ports pin number: ");
  for (i = 0; i < _counter_bits; i++) {
    Serial.print(_binary_counter_ports[i]);
    if (i < 7) {
      Serial.print(", ");
    }
  }
  Serial.println();

  Serial.print("Ports state: ");
  for (i = 0; i < _counter_bits; i++) {
    Serial.print(digitalRead(_binary_counter_ports[i]));
    if (i < 7) {
      Serial.print(", ");
    }
  }
  Serial.println();


  Serial.println("--- Counts ---");
  str_buf = ("  Whole Counts: " + String(whole_counts_));
  Serial.println(str_buf);

  str_buf = ("  Difference Counts: " + String(difference_counts));
  Serial.println(str_buf);

  str_buf = ("  Overflow Counts: " + String(overflow_counts_));
  Serial.println(str_buf);

  Serial.println("--- Volume ---");
  str_buf = ("  Volume per once (ml): " + String(volume_since_last_time));
  Serial.println(str_buf);

  str_buf = ("  Total Volume (ml): " + String(total_volume));
  Serial.println(str_buf);
}
