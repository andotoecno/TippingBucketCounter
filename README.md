## Support Hardware
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

## PCNTのフィルタについて
- 本ライブラリではデフォルトでフィルタが無効であり、外部回路でチャタリング対策を行うことを想定しています。
  - 多くのリードスイッチはチャタリング時間が最大時間の12.8usより長く、PCNTのフィルタリングが機能しないためです。
- 12.5nS(80Mhzの1クロック)の整数倍の時間で遅延できるフィルタがあり、整数倍は10 bitが最大値です。
  - 恐らく10bitタイマーを使ってフィルタリングしていると推測されます。
  - 12.5nS * 1024 = 12.8uSのフィルタが最大値になりますので、これ以上の長時間のチャタリング対策はハードウェア等で行うしかありません。チャタリング時間については事前にオシロスコープ等で確認しておく必要があります。

## ライセンス
- [MITライセンス](https://opensource.org/licenses/MIT)に基づいています。
- 詳細は[LICENSE](LICENSE)を参照してください。