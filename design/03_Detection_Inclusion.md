## 各フォルダの __detect.h , __include.h の配置ルール

これらのルールは、主に環境依存コードが集約される `/src/flexhal/platform/` 以下のフォルダ階層で適用されます。

### __detect.h のルール
- __detect.h は、親フォルダに __detect.h があれば最初にそれを include する
- __detect.h には、所属するフォルダ階層がビルド環境に必要かどうかを検出するマクロを含める
- ビルド環境固有の define 値やヘッダファイルの存在有無を判断に用いる。
  - 例: `#if defined (ARDUINO)` や `#if __has_include(<freertos.h>)` など
  - マクロで検出するのは `hardware::arch`, `hardware::chip`, `framework`, `rtos` の各レイヤ
- 各レイヤは択一でない場合がある。例えば `framework` は Arduino と ESP-IDF が同時に存在しうる。
- ユーザー自身がコンパイラオプション等で指定をせずとも使えるようにする。

- 検出の結果として `FLEXHAL_DETECT_`*階層名* マクロを定義し、結果が 1 であるかを判断する
  - 例: `/FlexHAL/src/flexhal/platform/hardware/chip/espressif/esp32` の場合
    - 結果: `FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32` (※ マクロ名も `platform` を含むように修正)

#### 例1: `/FlexHAL/src/flexhal/platform/hardware/chip/espressif/__detect.h`
```cpp
// 親階層 (platform/hardware/chip) の __detect.h をインクルード (存在すれば)
#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF
 #if defined(ESP_PLATFORM) // ESP-IDF 環境かどうかの判定例
  #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF 1
 #else
  #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF 0
 #endif
#endif
```

#### 例2: `/FlexHAL/src/flexhal/platform/hardware/chip/espressif/esp32/__detect.h`
```cpp
// 親階層 (platform/hardware/chip/espressif) の __detect.h をインクルード
#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32
 // 親階層 (espressif) が検出されていることが前提
 #if FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF
  // sdkconfig.h があればインクルード (ESP-IDF環境の判定補助)
  #if __has_include(<sdkconfig.h>)
   #include <sdkconfig.h>
  #endif
  // ESP32 ターゲットであるかどうかの判定例 (Arduino環境とESP-IDF環境の両方に対応)
  #if defined(ESP32) || defined(CONFIG_IDF_TARGET_ESP32)
   #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32 1
  #endif
 #endif
#endif

// デフォルト値の設定 (検出されなかった場合)
#ifndef FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32
 #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32 0
#endif
```

#### 例3: `/FlexHAL/src/flexhal/platform/hardware/chip/espressif/esp32/esp32s3/__detect.h`
```cpp
// 親階層 (platform/hardware/chip/espressif/esp32) の __detect.h をインクルード
#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3
 // 親階層 (esp32) が検出されていることが前提
 #if FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32
  // ESP32-S3 ターゲットであるかどうかの判定例 (ESP-IDF環境を想定)
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
   #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3 1
  #endif
 #endif
#endif

// デフォルト値の設定 (検出されなかった場合)
#ifndef FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3
 #define FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32_ESP32S3 0
#endif
```

### __include.h のルール
- __include.h は、まず `#include "__detect.h"` し、当該階層の検出マクロ (`FLEXHAL_DETECT_...`) の結果が真 (`1`) でなければ何もしない。
- 検出に成功している場合は、当該階層に含まれる公開APIヘッダ (`*.hpp` など) およびサブフォルダの `__include.h` のインクルードを含める。

#### 例1: `/FlexHAL/src/flexhal/platform/hardware/chip/espressif/__include.h`
```cpp
#if __has_include("__detect.h")
 #include "__detect.h"
#endif

#if FLEXHAL_DETECT_PLATFORM_HARDWARE_CHIP_ESPRESSIF // 検出結果
 // この階層に直接属する公開APIヘッダがあればインクルード (例: espressif 共通機能)
 // #include "espressif_common.hpp"

 // サブフォルダの __include.h をインクルード
 #if __has_include("esp8266/__include.h")
  #include "esp8266/__include.h"
 #endif
 #if __has_include("esp32/__include.h")
  #include "esp32/__include.h"
 #endif
 // 他の espressif チップの __include.h も同様にインクルード

#endif
```

### ~~__include.inl のルール~~ (廃止)

- ヘッダオンリー + 単一ソースファイル (`FlexHAL.cpp`) による実装方式の採用に伴い、`.inl` ファイルによる実装のインクルードは不要になりました。
- 実装は対応する `.hpp` ヘッダファイル内に `#ifdef FLEXHAL_IMPLEMENTATION` / `#endif` で囲って記述します。
- 詳細は [04_Header_Implementation.md](./04_Header_Implementation.md) (予定) を参照してください。
