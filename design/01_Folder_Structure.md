## フォルダ構造とファイルの配置ルール
- ArduinoIDEおよびPlatformIOからライブラリとして認識できる構成にする。
- ESP-IDFの components として利用できるよう CMakeLists.txt を配置する。
- 基本的に全てのAPIはC++用として提供するが、将来的にC言語用のラッパーを提供する可能性がある。
- ビルドの検証はPlatformIOを使用する。 /FlexHAL/platformio.ini に設定を配置。

### platformio.ini の設定ルール
- platformio.ini には、esp32, native, など環境用の定義を含む。
- platformio.ini には、examples の各サンプルフォルダをビルドするためのフォルダ設定の定義を含む。
- 上記の各設定定義を掛け合わせたものを env として用意することで、複数の環境とサンプルのビルドを可能にする。

### フォルダ構成の概要
```
FlexHAL/
├── library.properties
├── library.json
├── platformio.ini         # PlatformIOの設定ファイル (ビルドやテスト用の設定)
├── examples/              # Arduinoライブラリとしてのサンプルフォルダ
│   └── build_test/        # ビルドテスト用サンプル
│       ├── build_test.ino # ビルドテスト用サンプルのArduinoスケッチ(中身は空)
│       └── src/           # ビルドテスト用サンプルの中身
│           └── main.cpp   # ビルド対象ファイル
├── src/
│   ├── FlexHAL.h          # エイリアス役。内容は #include "FlexHAL.hpp"
│   ├── FlexHAL.hpp        # ライブラリ公開用 API (実装を含む場合がある)
│   ├── FlexHAL.cpp        # 実装を有効化し、ヘッダをインクルードする唯一のソースファイル
│   └── flexhal/           # 公開APIヘッダ群 (実装を含む場合がある)
│       ├── base/          # 基本定義と言語機能の互換性レイヤー
│       │   ├── cpp/       # C++バージョン互換レイヤー
│       │   │   ├── compat.hpp    # C++バージョン依存の機能を抽象化
│       │   │   └── __include.h   # cpp/ フォルダのインクルードファイル
│       │   ├── error.hpp  # エラーコード定義 (予定)
│       │   ├── types.hpp  # 基本型定義 (予定)
│       │   └── __include.h
│       ├── __include.h    # Top-level internal include file
│       ├── version.h      # セマンティックバージョニング用のバージョン情報マクロ定義
│       ├── internal/      # 内部向け実装 (環境依存のAPIを提供する階層。ユーザーによる直接操作は推奨されない)
│       │   ├── platform/  # プラットフォーム依存API (ESP32とかSAMD51とかNative(デスクトップOS)とか)
│       │   │   └── ...    # espressif,microchipなどベンダ階層を持つ。その配下にマイコン種別などの階層
│       │   ├── framework/ # フレームワーク依存API
│       │   │   └── ...    # arduino, esp-idf, cmsis, sdl
│       │   └── arch/      # CPUアーキテクチャ依存API
│       │       └── ...    # xtensa::lx6 , arm::armv7e-m, riscv::rv32imc
│       ├── fallback/      # フォールバック実装API階層
│       │   └── ...        # (e.g., basic printf logger), always included
│       ├── utils/         # Common utility code (Logging, etc.), always included
│       │   └── ...
│       └── ...
```
### /FlexHAL/src/ フォルダの配置ルール
- このフォルダはArduinoライブラリとして、ビルド対象やインクルードパスとなるフォルダ。
- ユーザーが直接使用するヘッダファイルとして、 `FlexHAL.hpp` と `FlexHAL.h` が配置される。
- ソースファイルは `FlexHAL.cpp` のみを唯一のビルド対象ソースファイルとする。これ以外には拡張子 `.c` `.cpp` のファイルはサブフォルダを含めてプロジェクト内に存在させない。
  - 上記の理由はビルド対象ファイル数を抑えること。ファイルが増えるとWindows版ArduinoIDEでのビルド時間が長くなる。
- 上記の`FlexHAL.cpp` の内容は、まず `#define FLEXHAL_INTERNAL_IMPLEMENTATION` を記述し、その後に `#include "FlexHAL.hpp"` を記述する。これにより、ライブラリ全体のヘッダ内実装がこのファイルでコンパイルされる。
- C++の実装は、原則として対応する `.hpp` ヘッダファイル内に記述する。
- ヘッダファイル内の実装コードは `#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` / `#endif` マクロで囲み、ヘッダをインクルードするだけでは実装がコンパイルされないようにする。
- インクルードガードは原則 `#pragma once` とするが、 `FlexHAL.h` および `FlexHAL.hpp` のみ `ifndef` + `define`マクロ方式とする。これはユーザーコード側で、`ifndef`マクロによって本ライブラリがincludeされているかどうかを判断可能にするため。

### /FlexHAL/src/flexhal/ フォルダおよびその配下の配置ルール

- FlexHAL/src/flexhal/ フォルダは 名前空間 flexhal と対応する。
- namespace flexhal::foo:: に所属する関数のコードは FlexHAL/src/flexhal/foo.hpp ファイルに記述する。
- namespace flexhal::foo:: に所属するclassやnamespaceは FlexHAL/src/flexhal/foo/ フォルダに配置する。
- フォルダ内にファイルやフォルダを追加・削除した場合は、所属フォルダの __include.h ファイルも更新すること。

### コーディング規約 (一部)

#### 内部マクロの命名規則
- FlexHAL ライブラリが内部的に使用するマクロ名は、原則として `FLEXHAL_INTERNAL_` というプレフィックスで始める。
  - 例: `FLEXHAL_INTERNAL_IMPLEMENTATION`
  - 例: `FLEXHAL_INTERNAL_CPP17`
  - 例: `FLEXHAL_INTERNAL_NODISCARD`
- これは以下の場合に適用される：
  1. 内部実装の制御に使用するマクロ
  2. ライブラリ内部でのみ使用される機能の制御マクロ
  3. 環境依存の機能を抽象化するマクロ
- これにより、ユーザーがビルドオプションなどで定義する可能性のあるマクロとの名前衝突を避ける。

#### 階層構造を実現するための仕組み
- 環境依存コードが集約される `/src/flexhal/internal/` フォルダ内
  - `/src/flexhal/internal/platform/` : 特定の実行環境プラットフォーム（例: `espressif/esp32`, `native`）の実装。PlatformIO の `platform` の概念に近い階層です。
  - `/src/flexhal/internal/framework/` : 特定のソフトウェアフレームワーク（例: `arduino`, `esp-idf`, `sdl`）の実装。
  - `/src/flexhal/internal/arch/` : 特定のCPUアーキテクチャ（例: `xtensa`, `arm`）の実装。
- 環境依存コードの検出・インクルードには、各階層に配置する `__detect.h` および `__include.h` ファイルを使用する (詳細は [03_Detection_Inclusion.md](./03_Detection_Inclusion.md) を参照)
- `/src/flexhal/utils/` フォルダ内
  - ライブラリ全体で共通して使用されるユーティリティ機能を配置する。
  - 例: Loggerインターフェース (`ILogger.hpp`)
- `/src/flexhal/fallback/` フォルダ内
  - 特定のプラットフォームに依存しない、基本的な機能を提供するフォールバック実装を配置する。
  - これらの実装は、対応するプラットフォーム固有の実装が存在しない場合や、明示的に選択された場合に使用される。
  - 例: `printf` を使用した基本的なロガー実装 (`PrintfLogger.hpp`)

#### __detect.h , __include.h のルール
- 詳細は [03_Detection_Inclusion.md](./03_Detection_Inclusion.md) を参照

### `src/flexhal/internal/`

*   **役割**: 特定の環境（プラットフォーム、フレームワーク、CPUアーキテクチャ）に依存する実装を格納します。
*   **原則**:
    *   このディレクトリ内のコードは、通常ユーザーが直接利用することは**推奨されません**。FlexHALの共通インターフェースを通じて利用されることを想定しています。
    *   `__detect.h` ファイルを各サブディレクトリ（`platform`, `framework`, `arch`）やその更に下の階層に配置し、利用可能な機能や環境を定義します。
    *   `__include.h` ファイルを配置し、検出された機能に対応するヘッダーファイルをインクルードします。
*   **サブディレクトリ**:
    *   `platform/`: 特定の実行環境プラットフォーム（例: `espressif/esp32`, `native`）の実装。PlatformIO の `platform` の概念に近い階層です。
    *   `framework/`: 特定のソフトウェアフレームワーク（例: `arduino`, `esp-idf`, `sdl`）の実装。
    *   `arch/`: 特定のCPUアーキテクチャ（例: `xtensa`, `arm`）の実装。

### `src/flexhal/utils/`

*   **役割**: 環境に依存しない共通のユーティリティコード（ロガープロキシなど）を格納します。
*   **原則**:
    *   このディレクトリ内の機能は常に利用可能です。
    *   `internal` や `fallback` から利用されることがあります。

### `src/flexhal/fallback/`

*   **役割**: 特定のプラットフォーム実装が存在しない場合や、基本的な機能を提供するためのフォールバック実装を格納します。
*   **原則**:
    *   このディレクトリ内の機能は常に利用可能です。
    *   `printf` ベースのロガーなどが含まれます。
