#include "TippingBucketCounter.h"

/**
 * @brief Initialize the Tipping Bucket Counter using direct GPIO reading.
 * @param kept_whole_counts_last_time The last saved whole count value.
 * @param kept_overflow_counts The last saved overflow count value.
 */
void TippingBucketCounter::begin(int16_t kept_whole_counts_last_time, uint8_t kept_overflow_counts)
{
  count_mode = MODE_BINARY_COUNTER;
  /*pin mode*/
  for (uint16_t i = 0; i < _counter_bits; i++)
    pinMode(_binary_counter_ports[i], INPUT);
  pinMode(_binary_counter_ports[PORT_CCK_INDEX], INPUT);
  pinMode(_binary_counter_ports[PORT_CCLR_INDEX], INPUT);
  /*take over last value*/
  whole_counts_last_time_ = kept_whole_counts_last_time;
  overflow_counts_ = kept_overflow_counts;
}

/**
 * @brief Initialize the Tipping Bucket Counter using ESP32 PCNT peripheral.
 * @param kept_whole_counts_last_time The last saved whole count value.
 * @param kept_overflow_counts The last saved overflow count value.
 * @param pulse_input_gpio_num GPIO pin number for pulse input.
 * @param pcnt_unit PCNT unit to use.
 */
void TippingBucketCounter::begin(int16_t kept_whole_counts_last_time, uint8_t kept_overflow_counts, uint8_t pulse_input_gpio_num, pcnt_unit_t pcnt_unit, uint16_t pcnt_filter_value)
{
  count_mode = MODE_PULSE_COUNTER;
  /*pin mode*/
  pcnt_config_t pcnt_config;
  pcnt_config.pulse_gpio_num = pulse_input_gpio_num;
  pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED;
  pcnt_config.lctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config.hctrl_mode = PCNT_MODE_KEEP;
  pcnt_config.channel = PCNT_CHANNEL_0;
  pcnt_config.unit = pcnt_unit;
  pcnt_config.pos_mode = PCNT_COUNT_INC;
  pcnt_config.neg_mode = PCNT_COUNT_DEC;
  pcnt_config.counter_h_lim = PCNT_MAX_COUNT;
  pcnt_config.counter_l_lim = -PCNT_MAX_COUNT;
  pcnt_unit_config(&pcnt_config);                      // ユニット初期化
  pcnt_set_filter_value(pcnt_unit, pcnt_filter_value); // フィルタ値設定
  pcnt_counter_pause(PCNT_UNIT_0);                     // カウンタ一時停止
  pcnt_counter_clear(PCNT_UNIT_0);                     // カウンタ初期化
  pcnt_counter_resume(PCNT_UNIT_0);                    // カウント開始
  /*take over last value*/
  whole_counts_last_time_ = kept_whole_counts_last_time;
  overflow_counts_ = kept_overflow_counts;
}

/**
 * @brief Take the current count by reading GPIO pins directly.
 */
void TippingBucketCounter::take_count()
{
  if (count_mode != MODE_BINARY_COUNTER)
  {
    Serial.println("Error: BucketCounter Mode Mismatch between initialization (Pulse) and execution (Binary).");
    return;
  }

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

/**
 * @brief Take the current count using the ESP32 PCNT peripheral.
 * @param pcnt_unit PCNT unit to read from.
 */
void TippingBucketCounter::take_count(pcnt_unit_t pcnt_unit)
{
  if (count_mode != MODE_PULSE_COUNTER)
  {
    Serial.println("Error: BucketCounter Mode Mismatch between initialization (Binary) and execution (Pulse).");
    return;
  }

  pcnt_get_counter_value(pcnt_unit, &whole_counts_);
  difference_counts = whole_counts_ - whole_counts_last_time_;

  if (difference_counts < 0)
  {
    difference_counts += 256; /*not 255 because next count of 255 is 0(not 1)*/
    overflow_counts_++;
  }
  whole_counts_last_time_ = whole_counts_;
}

/**
 * @brief Clear the external counter IC by toggling CCLR and CCK pins.
 */
void TippingBucketCounter::count_clear()
{
  if (count_mode != MODE_BINARY_COUNTER)
  {
    Serial.println("Error: BucketCounter Mode Mismatch between initialization (Pulse) and execution (Binary).");
    return;
  }

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

  total_volume = 0;
  whole_counts_ = 0;
  difference_counts = 0;
  overflow_counts_ = 0;
  whole_counts_last_time_ = 0;
}

/**
 * @brief Clear the ESP32 PCNT counter.
 * @param pcnt_unit PCNT unit to clear.
 */
void TippingBucketCounter::count_clear(pcnt_unit_t pcnt_unit)
{
  if (count_mode != MODE_PULSE_COUNTER)
  {
    Serial.println("Error: BucketCounter Mode Mismatch between initialization (Binary) and execution (Pulse).");
    return;
  }

  pcnt_counter_pause(pcnt_unit);  // カウンタ一時停止
  pcnt_counter_clear(pcnt_unit);  // カウンタ初期化
  pcnt_counter_resume(pcnt_unit); // カウント開始

  total_volume = 0;
  whole_counts_ = 0;
  difference_counts = 0;
  overflow_counts_ = 0;
  whole_counts_last_time_ = 0;
}

void TippingBucketCounter::calculate_volume(float bucket_volume_ml)
{
  volume_since_last_time = (float)difference_counts * bucket_volume_ml;
  total_volume = ((float)whole_counts_ + (float)overflow_counts_ * 256.0) * bucket_volume_ml;
}

void TippingBucketCounter::debug()
{
  if (count_mode == MODE_BINARY_COUNTER)
  {
    Serial.println("Count mode: MODE_BINARY_COUNTER");
    Serial.print("Ports pin number: ");
    for (i = 0; i < _counter_bits; i++)
    {
      Serial.print(_binary_counter_ports[i]);
      if (i < 7)
      {
        Serial.print(", ");
      }
    }
    Serial.println();

    Serial.print("Ports state: ");
    for (i = 0; i < _counter_bits; i++)
    {
      Serial.print(digitalRead(_binary_counter_ports[i]));
      if (i < 7)
      {
        Serial.print(", ");
      }
    }
    Serial.println();
  }
  else if (count_mode == MODE_PULSE_COUNTER)
  {
    Serial.println("Count mode: MODE_PULSE_COUNTER");
  }

  String str_buf = "";
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
