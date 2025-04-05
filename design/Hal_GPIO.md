
# GPIO機能

## 機能要件

### 基本要件
- GPIOを制御する
- 実装部は、複数提供する。例としてESP32-S3の場合は以下のような実装を利用可能にする
  - Arduino依存実装は、`flexhal::internal::framework::arduino::hal::gpio`
  - ESP-IDF依存実装は、`flexhal::internal::framework::espidf::hal::gpio`
  - プラットフォーム依存実装は、`flexhal::internal::platform::esp32::esp32s3::hal::gpio`
- 公開APIとしては、抽象化されたインターフェイスと、抽象化されない関数型APIを提供する
- IGpioインターフェイスで、複数のピンを持つペリフェラルを表現する型を抽象化する
- IPinインターフェースで、1つのピンを表現する型を抽象化する

### 関数型API
- 関数型APIとして、ピン設定、デジタル入出力、アナログ入出力などの関数を提供する。
- 関数型APIは、snake_caseで命名する。
- 関数型APIは、環境依存階層ごとに提供する。
- 異なる環境向けの実装それぞれの使用感が同じになるように、関数の命名や引数などは、同一になるようにする。
  - 例:関数 hal::gpio::digital_write を提供する場合、ESP32-S3環境では以下の配置にも同じ関数シグネチャで実装が用意される。
    - flexhal::internal::framework::arduino::hal::gpio::digital_write
    - flexhal::internal::framework::espidf::hal::gpio::digital_write
    - flexhal::internal::platform::esp32::esp32s3::hal::gpio::digital_write
    - flexhal::fallback::hal::gpio::digital_write
- 関数型APIは、環境依存のない階層 flexhal::hal::gpio としても提供する。
  - flexhal::hal::gpio は、環境依存APIの別名(using namespace など？)として提供する。
  - ビルド環境ごとに最適な実装が選択されるようにマクロを構築する。
  - つまりプラットフォーム依存のgpio APIがある場合はそれが利用され、無ければフレームワーク依存、それも無ければフォールバック実装が利用される。
  - これにより、抽象化オーバーヘッドが最小限の関数APIが用意される。
- 関数型APIは基本的に速度優先とし、エラーハンドリングや安全対策を考慮しない。範囲外のピンを操作指示した場合の挙動は未定義とする。
- 割込処理用のAPIは、関数型APIでは今のところ提供しない。 (将来的に提供する可能性あり)

### IGpioインターフェース
- GPIOペリフェラルの抽象化共通インターフェースとして、flexhal::hal::gpio::IGpioインターフェイスを提供する。
- マイコンのGPIOペリフェラルや、I2C接続のGPIOエキスパンダを表す型としての利用を想定。
- IGpioインターフェイスを継承して環境依存階層の実装を作る。
- IGpioインターフェイスは、関数型APIと同程度のメソッドを提供するが、可能な限り戻り値として flexhal::base::error_t 型 (内部的には enum flexhal::base::status) を返却する。
- 環境依存階層の実装部は、エラーハンドリングや安全対策を考慮した上で、内部的に関数型APIを呼び出すラッパーを提供する。
- メソッド名は、camelCaseで命名する。
- 派生型インスタンスの生成については考慮が必要。
  - 環境固有ネイティブ実装などの実体が一つで良いものはシングルトンで用意したい
  - 特殊なケースは、それぞれファクトリメソッドやコンストラクタを利用する
- 割込処理用のメソッドを提供する。（詳細未定）
- 単独のピンを表すIPinインターフェイスを提供する。
- getPinメソッドで任意のピンを表すIPinを取得できる。
- IPinを使わなくてもIGpioで全ての操作を可能とする。

### IPinインターフェース
- 単独のピンを表す抽象化インターフェイスとして、flexhal::hal::gpio::IPinインターフェイスクラスを提供する。
- IPinを継承して環境依存階層の実装を作る。
- IPinは、所属するIGpioへの参照(またはポインタ)を持ち、基本実装として、所属するIGpioの全てのメソッドに対応したラッパーメソッドを提供する。
  - 例: error_t IPin::digitalWrite(bool level) { return _gpio->digitalWrite(_pin, level); }


### 提供するAPI

- デジタル出力: 関数型 = digital_write / IGpio::digitalWrite / IPin::digitalWrite
- デジタル入力: 関数型 = digital_read / IGpio::digitalRead / IPin::digitalRead
- アナログ出力: 関数型 = analog_write / IGpio::analogWrite / IPin::analogWrite
- アナログ入力: 関数型 = analog_read / IGpio::analogRead / IPin::analogRead
- ... 以下未整理
