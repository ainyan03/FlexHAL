## 名前空間 (Namespace) のルール

`FlexHAL` のコードはすべて `flexhal` 名前空間内に配置されます。

### トップレベル構造 (`flexhal::*`)

`flexhal` 名前空間の直下には、主に **環境に依存しない共通の機能** が配置されます。

- **共通インターフェース:** 各種ハードウェア機能 (GPIO, SPI, I2C, Timer など) の基本的な操作を定義する抽象クラスや型定義。
    - 例: `flexhal::gpio`, `flexhal::bus`, `flexhal::timer`
- **コア機能:** ライブラリの基本的な動作に必要な、環境非依存のクラスや関数。
    - 例: (もしあれば `flexhal::core` など)
- **ユーティリティ:** ログ出力、エラー処理など、汎用的に使える補助的な機能。
    - 例: `flexhal::utils`

### 環境依存実装 (`flexhal::platform::*`)

特定のCPUアーキテクチャ、チップ、フレームワーク、RTOSなどに依存するコードは、すべて `flexhal::platform` 名前空間以下に配置されます。これにより、環境依存部分が明確に分離されます。

- **`flexhal::platform::hardware`**: CPUアーキテクチャ (`arch`)、具体的なチップ (`chip`)、ボード (`board`) 固有の実装。
    - `arch`: CPUコアに依存するコード
      - (例: `arm`, `riscv`, `xtensa`)
    - `chip`: 特定のマイコンチップに依存するコード
      - (例: `espressif::esp32`, `stmicro::stm32::stm32f::stm32f4`)
    - `board`: 特定の開発ボードに依存するコード (もし必要なら)
- **`flexhal::platform::framework`**: フレームワーク固有の実装やラッパー
  - (例: Arduino, ESP-IDF, CMSIS)
- **`flexhal::platform::rtos`**: RTOS固有の実装やラッパー。
  - (例: FreeRTOS, Zephyr)
- **`flexhal::platform::software`**: 特定のソフトウェア環境やOSに依存するコード。
  - (例: (libc))

*注意:* `flexhal` 直下の共通インターフェース (例: `flexhal::gpio::IGpio`) に対する具体的な実装は、この `flexhal::platform` 以下の対応する場所 (例: `flexhal::platform::hardware::chip::esp32::gpio::GpioImpl`) に配置されます。

### (参考) 詳細な階層構造案

**共通機能 (flexhal::*)**

- `flexhal::core` (もしコア機能があれば)
- `flexhal::utils`
  - `logger`
  - `error`
- `flexhal::gpio` (インターフェース)
- `flexhal::bus` (インターフェース)
  - `spi`
  - `i2c`
  - `i2s`
  - `uart` (uartp?)
  - `parallel`
- `flexhal::timer` (インターフェース)
- `flexhal::pwm` (インターフェース)
- `flexhal::adc` (インターフェース - 追加提案)
- `flexhal::dac` (インターフェース - 追加提案)
- `flexhal::can` (インターフェース - 追加提案)

**環境依存実装 (flexhal::platform::*)**

- `flexhal::platform::hardware`
  - `arch`
    - `arm`
      - `cortex_m` (例: M0, M4, M7)
    - `riscv`
    - `xtensa`
      - `lx6` (例: ESP32)
      - `lx7` (例: ESP32-S3)
  - `chip`
    - `espressif`
      - `esp8266`
      - `esp32` (ESP32シリーズ共通)
        - `gpio` (実装)
        - `spi` (実装)
        - ... (各ペリフェラルの実装)
      - `esp32s2` (ESP32-S2固有)
      - `esp32s3` (ESP32-S3固有)
      - ... (他のESP32系チップ)
    - `raspberrypi`
      - `rp2040`
      - `rp2350`
    - `stmicro`
      - `stm32`
        - `stm32f1`
        - `stm32f4`
        - ... (他のSTM32シリーズ)
  - `board` (もしボード固有の実装が必要なら)
- `flexhal::platform::framework`
  - `arduino`
    - `gpio` (実装)
    - `spi` (実装)
    - ...
  - `esp_idf`
    - `gpio` (実装)
    - ...
  - `cmsis`
- `flexhal::platform::rtos`
  - `freertos`
  - `zephyr`
- `flexhal::platform::software`
  - `libc` (例: printf ベースのロガーなど)

