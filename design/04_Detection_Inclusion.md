## 各フォルダの __detect.h の配置ルール

これらのルールは、主に環境依存コードが集約される `/src/flexhal/internal/` 以下のフォルダ階層で適用されます。 `/src/flexhal/utils/` や `/src/flexhal/fallback/` のような共通コンポーネントのフォルダには、これらのファイルは通常配置 **しません**。

### __detect.h のルール
- `__detect.h` は、親フォルダに `__detect.h` があれば最初にそれを `#include` する (必須ではないが推奨)。
- `__detect.h` には、所属するフォルダ階層が現在のビルド環境で **必要かどうか** を検出するためのマクロを含める。
- ビルド環境固有の `define` 値 (例: `ESP32`, `ARDUINO`) や、特定のヘッダファイルの存在有無 (例: `#if __has_include(<freertos.h>)`) を判断材料として用いる。
- 検出対象とする主なレイヤは `internal/platform/*` (プラットフォーム), `internal/framework/*` (フレームワーク), `internal/arch/*` (アーキテクチャ) など。
- 各レイヤは択一でない場合がある。例えば `framework` は Arduino と ESP-IDF が同時に有効になりうる。
- ユーザーがコンパイラオプション等で **明示的に指定しなくても** 動作するように、自動検出を基本とする。
- 検出の結果として `FLEXHAL_DETECT_` + (フォルダパスを `_` で繋いだもの) という形式のマクロを定義し、その値が `1` であるかどうかで有効性を判断する。
  - 例: `/src/flexhal/internal/platform/hardware/chip/espressif/esp32/` の場合 → `FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32`

#### 例1: `/src/flexhal/internal/platform/hardware/chip/espressif/__detect.h`
```cpp
// 親階層 (platform/hardware/chip) の __detect.h をインクルード (存在すれば)
#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF
 #if defined(ESP_PLATFORM) // ESP-IDF 環境かどうかの判定例
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF 1
 #else
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF 0
 #endif
#endif
```

#### 例2: `/src/flexhal/internal/platform/hardware/chip/espressif/esp32/__detect.h`
```cpp
// 親階層 (platform/hardware/chip/espressif) の __detect.h をインクルード
#if __has_include("../__detect.h")
 #include "../__detect.h"
#endif

#ifndef FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32
 // espressif が検出されていて、かつ ESP32 ボードがターゲットの場合
 #if FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF && defined(CONFIG_IDF_TARGET_ESP32)
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32 1
 #else
  #define FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32 0
 #endif
#endif
```

### __detect.hが存在する階層のルール
- 子階層に `__detect.h` が存在する場合、それを最初に `#include` する。
- 子階層の `__detect.h` で定義された検出マクロ (例: `FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE`) の値が `1` かどうかを `#if` でチェックする。
- マクロが `1` の場合のみ、子階層の内部にあるヘッダのインクルードを行う:

#### 例: `/src/flexhal/internal/platform/native.hpp`
```cpp
#pragma once

#include "native/__detect.h"

#if FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE

// nativeフォルダ内のヘッダをすべて include
#include "native/aaa.hpp"
#include "native/bbb.hpp"
#include "native/ccc.hpp"

#endif // FLEXHAL_DETECT_INTERNAL_PLATFORM_NATIVE
```

### トップレベルでのインクルード構造

ユーザーが直接インクルードする `FlexHAL.hpp` もまた、子階層のヘッダをすべてインクルードします。

```cpp
#ifndef FLEXHAL_HPP_
#define FLEXHAL_HPP_

#include "flexhal/base.hpp"
#include "flexhal/internal.hpp"
#include "flexhal/fallback.hpp"
#include "flexhal/utils.hpp"

#endif // FLEXHAL_HPP_
```

