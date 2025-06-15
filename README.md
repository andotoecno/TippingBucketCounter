## Support Hardware
転倒ます型の液量カウントのためのパルスカウンタライブラリです。
以下のハードウェアを使ったパルスカウントに対応しています。
1. Binary Counter (8-bit, TC74HC590AP)
   - GPIOとParallel接続し、8ビットのカウントを行います。
   - カウントクリア含めて全10pinの接続が必要です。
   - カウントの保持はTC74HC590APの内部RAMに行うので、省電力設計において有利です。
   - TC74HC590APがカウントする際はチャタリング対策にRCローパスフィルタとシュミットトリガを使用します。
2. [ESP32 Pulse Counter](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/pcnt.html)
   - ESP32のPCNT Peripheralを使用して、GPIOピンに接続されたパルスをカウントします。
   - チャタリング対策のフィルタ、カウントクリア機能を持っています。
   - センシングしたパルスを直接GPIOピン1本に入力するだけなので、気軽に接続できます。
   - 参考文献
     - https://qiita.com/wanko_in_lunch/items/a508d8da78961c855d7f
     - https://docs.espressif.com/projects/esp-idf/en/v3.2.3/api-reference/peripherals/pcnt.html

## ESP32 Pulse Counter使用時の注意点
- 磁気スイッチのチャタリング対策として、take_countのメソッドは転倒周期よりも頻繁に(例えば毎秒)呼び出して使います。
  - Pulse Count Modeのtake_countでは、複数回のカウントアップを1カウントとする処理を入れているので、チャタリングで複数回誤カウントされても、1カウントにはなりません。
  - 転倒周期よりも遅い頻度でtake_countを呼び出した場合、チャタリングによる誤カウントが発生する可能性があります。
- ちなみにPulse Counter(PCNT)でもフィルタを設定していますが最大で12.8usであり、多くの磁気スイッチではより長い周期のシグナルが発生するためチャタリング対策としては不十分です。
  - このフィルタはの磁気スイッチのチャタリング対策ではなく、ノイズで稀に誤カウントが発生するので、その対策として使用しています。
  
## ライセンス
- [MITライセンス](https://opensource.org/licenses/MIT)に基づいています。
- 詳細は[LICENSE](LICENSE)を参照してください。