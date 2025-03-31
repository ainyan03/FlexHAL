# FlexHAL アーキテクチャ設計 🌈

## 目次

- [基本コンセプト](#基本コンセプト-)
- [対象プラットフォーム](#対象プラットフォーム-)
- [ディレクトリ構造](#ディレクトリ構造-)
- [名前空間構造](#名前空間構造-)
- [環境検出の仕組み](#環境検出の仕組み-)
- [マルチフレームワーク・マルチRTOS対応](#マルチフレームワーク・マルチrtos対応-)
- [プラットフォームとフレームワークの検出](#プラットフォームとフレームワークの検出-)
- [将来の拡張予定](#将来の拡張予定-)
- [実装選択の仕組み](#実装選択の仕組み-)
- [優先実装機能](#優先実装機能-)
- [例外処理方針](#例外処理方針-)
- [ビルド環境](#ビルド環境-)
- [バージョン管理](#バージョン管理-)

## 基本コンセプト ✨

FlexHALは、マルチプラットフォーム対応のハードウェア抽象化レイヤーで、以下の特徴を持ちます：

- **3層構造**: プラットフォーム・フレームワーク・RTOSの明確な分離
- **複数実装選択**: ファクトリパターンによる実装の選択機能
- **自動環境検出**: ビルド環境に応じた最適な実装の自動選択
- **マルチフレームワーク対応**: 複数フレームワークの同時サポート
- **マルチRTOS対応**: 複数RTOSの同時サポート
- **C++11ベース**: モダンC++の機能を活用した実装
- **Arduino IDE最適化**: ビルド時間を短縮する構造設計

## 2. 対象プラットフォーム 🖥️

初期対応予定のプラットフォーム：
- ESP32
- デスクトップ環境（SDL2使用）

将来的な対応予定：
- ESP8266
- RP2040/RP2350
- STM32
- SAMD21/SAMD51
- SPRESENSE
- その他の組み込みマイコン

## ディレクトリ構造 📁

```
FlexHAL/
├── src/                          # 公開API
│   ├── FlexHAL.h                 # C++チェック用ラッパー
│   ├── FlexHAL.hpp               # メインC++ヘッダー
│   ├── FlexHAL.cpp               # 実装エントリポイント
│   └── flexhal/                  # 公開APIフォルダ
│       ├── platform_detect.h     # プラットフォーム検出
│       ├── gpio/                 # GPIO関連
│       │   ├── _include.h        # GPIO全体インクルード
│       │   ├── gpio.h            # GPIO基本インターフェース
│       │   ├── digital.h         # デジタルIO API
│       │   └── factory.h         # GPIO実装選択ファクトリ
│       ├── time/                 # 時間関連
│       │   ├── _include.h
│       │   ├── delay.h
│       │   └── timer.h
│       ├── logger/               # ロガー関連
│       │   ├── _include.h
│       │   └── logger.h
│       ├── platforms/            # プラットフォーム固有拡張
│       └── frameworks/           # フレームワーク固有拡張
├── impl/                         # 内部実装
│   ├── _include.inl              # 全体インクルード
│   ├── gpio/                     # GPIO実装
│   │   ├── _include.inl
│   │   └── gpio.inl
│   ├── time/                     # 時間実装
│   │   ├── _include.inl
│   │   └── delay.inl
│   ├── logger/                   # ロガー実装
│   │   ├── _include.inl
│   │   └── logger.inl
│   ├── platforms/                # プラットフォーム別実装
│   │   ├── _include.inl
│   │   ├── esp32/
│   │   │   ├── _include.inl
│   │   │   ├── gpio.inl
│   │   │   └── time.inl
│   │   └── desktop/
│   │       ├── _include.inl
│   │       ├── gpio.inl
│   │       └── time.inl
│   ├── frameworks/               # フレームワーク別実装
│   │   ├── _include.inl
│   │   ├── arduino/
│   │   │   ├── _include.inl
│   │   │   └── gpio.inl
│   │   ├── esp_idf/
│   │   │   ├── _include.inl
│   │   │   └── gpio.inl
│   │   └── sdl/
│   │       ├── _include.inl
│   │       └── gpio.inl
│   └── rtos/                     # RTOS別実装
│       ├── _include.inl
│       ├── freertos/
│       │   ├── _include.inl
│       │   └── task.inl
│       └── desktop/
│           ├── _include.inl
│           └── task.inl
├── examples/                     # サンプルコード
└── docs/                         # ドキュメント
    └── spec/                     # 仕様書
```

## 名前空間構造 🏷️

フォルダ構造と名前空間構造を一致させます：

- `flexhal` - トップレベル名前空間
  - `flexhal::gpio` - GPIO関連
  - `flexhal::time` - 時間関連
  - `flexhal::logger` - ロガー関連
  - `flexhal::platform` - プラットフォーム固有実装
    - `flexhal::platform::esp32` - ESP32実装
    - `flexhal::platform::desktop` - デスクトップ実装
  - `flexhal::framework` - フレームワーク固有実装
    - `flexhal::framework::arduino` - Arduino実装
    - `flexhal::framework::esp_idf` - ESP-IDF実装
    - `flexhal::framework::sdl` - SDL実装
  - `flexhal::rtos` - RTOS固有実装
    - `flexhal::rtos::freertos` - FreeRTOS実装
    - `flexhal::rtos::desktop` - デスクトップOS実装
  - `flexhal::internal` - 内部実装（ユーザーは使用しない）

## 環境検出の仕組み 🔍

`platform_detect.h` で以下の環境を自動検出します：

### プラットフォーム検出
- `FLEXHAL_PLATFORM_ESP32` - ESP32
- `FLEXHAL_PLATFORM_ESP8266` - ESP8266
- `FLEXHAL_PLATFORM_RP2040` - RP2040/RP2350
- `FLEXHAL_PLATFORM_STM32` - STM32

## 6. マルチフレームワーク・マルチRTOS対応 🌐

FlexHALは、単一の環境で複数のフレームワークやRTOSが同時に存在する状況を想定して設計されています。

### 6.1 複数フレームワークの検出と利用

```cpp
// 利用可能なすべてのフレームワークを検出
bool has_arduino = flexhal::hasFramework(Framework::Arduino);
bool has_espidf = flexhal::hasFramework(Framework::EspIdf);
bool has_sdl = flexhal::hasFramework(Framework::SDL);

// 特定のフレームワーク向けの実装を取得
if (has_arduino) {
  // Arduino向けの処理
}

if (has_espidf) {
  // ESP-IDF向けの処理
}
```

### 6.2 複数RTOSの検出と利用

```cpp
// 利用可能なすべてのRTOSを検出
bool has_freertos = flexhal::hasRTOS(RTOS::FreeRTOS);
bool has_desktop = flexhal::hasRTOS(RTOS::Desktop);

// 特定のRTOS向けの処理
if (has_freertos) {
  // FreeRTOS向けの処理
}

if (has_desktop) {
  // デスクトップOS向けの処理
}
```

### 6.3 複数フレームワーク・RTOSの共存

FlexHALでは、複数のフレームワークやRTOSが同時に初期化され、それぞれの機能を利用できます。例えば：

- ESP32環境でArduinoとESP-IDFの両方のAPIを使用
- FreeRTOSとデスクトップOSの機能を同時に利用

### 6.4 初期化処理

```cpp
// ライブラリの初期化
flexhal::init();

// 内部では以下のような処理が行われます
void init() {
  // プラットフォームの初期化
  platform::init();
  
  // 利用可能なすべてのフレームワークを初期化
  if (hasFramework(Framework::Arduino)) {
    framework::initArduino();
  }
  if (hasFramework(Framework::EspIdf)) {
    framework::initEspIdf();
  }
  if (hasFramework(Framework::SDL)) {
    framework::initSDL();
  }
  
  // 利用可能なすべてのRTOSを初期化
  if (hasRTOS(RTOS::FreeRTOS)) {
    rtos::initFreeRTOS();
  }
  if (hasRTOS(RTOS::Desktop)) {
    rtos::initDesktop();
  }
}
```

このような設計により、各フレームワークやRTOSの長所を組み合わせた柔軟なアプリケーション開発が可能になります。

## プラットフォームとフレームワークの検出 🔍

### プラットフォーム検出
- `FLEXHAL_PLATFORM_ESP32` - ESP32
- `FLEXHAL_PLATFORM_ESP8266` - ESP8266
- `FLEXHAL_PLATFORM_RP2040` - RP2040/RP2350
- `FLEXHAL_PLATFORM_STM32` - STM32
- `FLEXHAL_PLATFORM_SAMD` - SAMD21/SAMD51
- `FLEXHAL_PLATFORM_DESKTOP` - デスクトップ環境
  - `FLEXHAL_PLATFORM_WINDOWS` - Windows
  - `FLEXHAL_PLATFORM_LINUX` - Linux
  - `FLEXHAL_PLATFORM_MACOS` - macOS

### フレームワーク検出
- `FLEXHAL_FRAMEWORK_ARDUINO` - Arduino
- `FLEXHAL_FRAMEWORK_ESP_IDF` - ESP-IDF
- `FLEXHAL_FRAMEWORK_SDL` - SDL

### RTOS検出
- `FLEXHAL_RTOS_FREERTOS` - FreeRTOS
- `FLEXHAL_RTOS_DESKTOP` - デスクトップOS

## 将来の拡張予定 🔮

### ハードウェア機能
- PWM出力機能の追加
- アナログ入出力の統合
- 割り込み処理のサポート
- SPI/I2C/UARTなどの周辺機能の実装

### ソフトウェア機能
- ロギングシステムの強化
- メモリ管理の最適化
- パフォーマンスベンチマークツールの実装

### ドキュメント
- チュートリアルの充実
- APIリファレンスの自動生成
- サンプルコードの追加

## 実装選択の仕組み 🛠️

ファクトリパターンを使用して、ユーザーが実装を選択できるようにします：

```cpp
// 使用例
#include <flexhal/gpio/factory.h>

void setup() {
  // デフォルト実装を使用（環境に応じて自動選択）
  auto gpio = flexhal::gpio::Factory::createDefault();
  
  // または明示的に実装を選択
  auto gpio_arduino = flexhal::gpio::Factory::create(flexhal::gpio::ImplType::Arduino);
  auto gpio_esp_idf = flexhal::gpio::Factory::create(flexhal::gpio::ImplType::EspIdf);
  auto gpio_native = flexhal::gpio::Factory::create(flexhal::gpio::ImplType::Native);
  
  // どの実装でも同じインターフェースで使える
  gpio->pinMode(5, OUTPUT);
  gpio->digitalWrite(5, HIGH);
}
```

## 優先実装機能 🔝

初期実装で優先する機能：

1. **基本機能**
   - 初期化・終了処理
   - ロガー（デバッグ出力）
   - millis/delay（時間関連）
   - GPIO（入出力制御）

2. **プラットフォーム**
   - ESP32
   - デスクトップ（SDL2）

## 例外処理方針 ⚠️

- ビルドフラグから`-fno-exceptions`を削除し、例外機能自体は有効にする
- 設計方針として`try-catch`文は一切使用しない
- `std::function`などの例外を内部で使用するC++標準ライブラリ機能は利用可能

## ビルド環境 🔨

- Arduino IDE + PlatformIO（デュアルサポート）
- `/src`フォルダをスッキリさせてArduino IDEのビルド時間短縮
- `.inl`ファイルを活用した実装分離

## バージョン管理 📊

- フォルダ構造ではなくGitのタグとリリースを使用
- セマンティックバージョニング（MAJOR.MINOR.PATCH）を採用
- 破壊的変更はMAJORバージョンアップで対応
