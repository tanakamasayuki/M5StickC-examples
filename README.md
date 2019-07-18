# M5StickC サンプルスケッチ集

ライブラリにするまでもない、細かいスケッチを保存する場所です。

## BLE_scanEx

BLEのデバイスをスキャンして、指定したサービスを持つキャラクタリスティックを列挙します。

ESP1.0.2のBLEライブラリにはバグがあり、デバイスによってはハングアップしてしまうのと、同じCharacteristic UUIDを複数もつデバイスの場合、UUIDをキーにしたMapでしか受け取れないため黒魔術(#define private public)を使って無理やり拡張してあります。

### 参考ページ
- [M5StickC(ESP32)でダイソーのBluetoothシャッターを操作](https://lang-ship.com/blog/?p=704)

## BLE_clientEx

BLEのデバイスをスキャンして、指定したサービスを持つキャラクタリスティックに通知を追加して、受信待ちをします。

ESP1.0.2のBLEライブラリにはバグがあり、デバイスによってはハングアップしてしまうのと、同じCharacteristic UUIDを複数もつデバイスの場合、UUIDをキーにしたMapでしか受け取れないため黒魔術(#define private public)を使って無理やり拡張してあります。

### 参考ページ
- [M5StickC(ESP32)でダイソーのBluetoothシャッターを操作](https://lang-ship.com/blog/?p=704)

## BluetoothSerial

Bluetooth Serialのサンプル。

## ButtonTest

Buttonクラスのサンプル。

## RealtimeDataLog

RealtimeDataLogのサンプル。

## RTCSetNTP

RTCをNTPサーバーから時刻設定するサンプル。

## RTCSetWeb

RTCをAPモードにしたM5StickCに接続したブラウザから時刻設定するサンプル。
