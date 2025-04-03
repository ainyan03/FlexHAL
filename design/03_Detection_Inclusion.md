## 各フォルダの __detect.h , __include.h の配置ルール

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

### __include.h のルール
- `__include.h` は、対応する `__detect.h` が存在すれば、最初にそれを `#include` する。
- `__detect.h` で定義された検出マクロ (例: `FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32`) の値が `1` かどうかを `#if` でチェックする。
- マクロが `1` の場合のみ、以下のインクルードを行う:
  - 同じフォルダ階層にある **公開ヘッダファイル** (例: `gpio.hpp` など、`_internal.hpp` のような内部用は除く)。
  - サブフォルダに存在する `__include.h` ファイル。
- これにより、不要な環境のコードは `#include` されず、コンパイル対象から外れる。

#### 例: `/src/flexhal/internal/platform/hardware/chip/espressif/esp32/__include.h`
```cpp
#if __has_include("./__detect.h")
 #include "./__detect.h"
#endif

// この階層 (esp32) が有効な場合のみインクルード
#if FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32

 // 同階層の公開ヘッダをインクルード
 #include "gpio.hpp" // 例
 #include "spi.hpp"  // 例

 // サブフォルダの __include.h をインクルード (もし存在すれば)
 // #if __has_include("subfolder/__include.h")
 //  #include "subfolder/__include.h"
 // #endif

#endif // FLEXHAL_DETECT_INTERNAL_PLATFORM_HARDWARE_CHIP_ESPRESSIF_ESP32
```

### トップレベルでのインクルード構造

最終的に、ユーザーが直接インクルードする `FlexHAL.hpp` (または内部用の `src/flexhal/__include.h`) は、各主要コンポーネントディレクトリの `__include.h` をインクルードします。

```cpp
// src/flexhal/__include.h (または FlexHAL.hpp 内) の例

// 常に必要となる共通ユーティリティをインクルード
#include "utils/__include.h" // (utils に __include.h があれば)
// または個別に #include "utils/logger/ILogger.hpp" など

// 常に利用可能なフォールバック実装をインクルード
#include "fallback/__include.h"

// プラットフォーム依存コードのインクルードを開始
// ここから先のインクルードは、__detect.h / __include.h の連鎖によって
// 必要なものだけが有効になる
#include "internal/__include.h"

```

### 実装 (.cpp) の有効化 (ヘッダオンリーでない場合)

ヘッダファイル (`.hpp`) だけでなく、ソースファイル (`.cpp`) に実装を記述する場合、その実装コードがリンクされるのは `src/FlexHAL.cpp` から間接的にインクルードされたときのみに限定したい場合があります。

この場合、以下のようなパターンを使用します。

1.  **ヘッダファイル (.hpp):** 実装部分を `#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` で囲みます。

    ```cpp
    // my_component.hpp
    #pragma once

    namespace flexhal {
    namespace my_ns {

    class MyClass {
    public:
        void doSomething(); // 宣言
    };

    } // namespace my_ns
    } // namespace flexhal

    // --- Implementation ---
    #ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
    namespace flexhal {
    namespace my_ns {

    void MyClass::doSomething() {
        // ... 実装 ...
    }

    } // namespace my_ns
    } // namespace flexhal
    #endif // FLEXHAL_INTERNAL_IMPLEMENTATION
    ```

2.  **ソースファイル (`src/FlexHAL.cpp`):** ヘッダをインクルードする **前** に `FLEXHAL_INTERNAL_IMPLEMENTATION` を定義します。

    ```cpp
    // src/FlexHAL.cpp

    // Define this macro to enable implementations in headers
    #define FLEXHAL_INTERNAL_IMPLEMENTATION

    // Include the main header which includes everything else
    #include "FlexHAL.hpp"

    // Optionally, add specific .cpp includes if needed, though the above pattern is often sufficient.
    ```

これにより、`MyClass::doSomething()` の実装は `FlexHAL.cpp` のコンパイル時にのみ有効となり、ユーザーコード側でヘッダをインクルードしただけでは実装が含まれず、多重定義エラーなどを防ぐことができます。
これは現在の `fallback/logger/PrintfLogger.hpp` で採用されている方法です。

```cpp
// src/flexhal/fallback/logger/PrintfLogger.hpp
#pragma once

namespace flexhal {
namespace fallback {
namespace logger {

class PrintfLogger {
public:
    void log(const char* message); // 宣言
};

} // namespace logger
} // namespace fallback
} // namespace flexhal

// --- Implementation ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION
namespace flexhal {
namespace fallback {
namespace logger {

void PrintfLogger::log(const char* message) {
    // ... 実装 ...
    printf("%s", message);
}

} // namespace logger
} // namespace fallback
} // namespace flexhal
#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
```

```cpp
// src/FlexHAL.cpp

#define FLEXHAL_INTERNAL_IMPLEMENTATION
#include "FlexHAL.hpp"
```
