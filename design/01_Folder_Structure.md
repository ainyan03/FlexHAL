## フォルダ構造とファイルの配置ルール
- ArduinoIDEおよびPlatformIOからライブラリとして認識できる構成にする。
- 基本的に全てのAPIはC++用として提供するが、将来的にC言語用のラッパーを提供する可能性がある。
- ビルドの検証は /FlexHAL/platformio.ini に設定を配置して PlatformIO で行う。
  - platformio.ini には、esp32, native の環境用の定義を含む。
  - platformio.ini には、examples の各サンプルフォルダをビルドするためのフォルダ設定定義を含む。
  - 上記の各設定定義を掛け合わせたものを env として用意することで、複数の環境とサンプルのビルドを可能にする。

### フォルダ構成の概要
```
FlexHAL/
├── library.properties
├── library.json
├── platformio.ini         // PlatformIOの設定ファイル (ビルドやテスト用の設定)
├── examples/              // Arduinoライブラリとしてのサンプルフォルダ
│   └── build_test/        // ビルドテスト用サンプル
│       ├── build_test.ino // ビルドテスト用サンプルのArduinoスケッチ(中身は空)
│       └── src/           // ビルドテスト用サンプルの中身
│           └── main.cpp   // ビルド対象ファイル
├── src/
│   ├── FlexHAL.h     // エイリアス役。内容は #include "FlexHAL.hpp"
│   ├── FlexHAL.hpp   // ライブラリ公開用 API (実装を含む場合がある)
│   ├── FlexHAL.cpp   // 実装を有効化し、ヘッダをインクルードする唯一のソースファイル
│   └── flexhal/      // 公開APIヘッダ群 (実装を含む場合がある)
│       ├── platform/ // 環境依存コード群
│       └── utils/    // 共通ユーティリティ
```
### srcフォルダの配置ルール
- ユーザーが使用するヘッダファイルは、 `FlexHAL.hpp` とするが、 `FlexHAL.h` も用意する。
- WindowsでArduinoIDE使用時のビルド時間を短縮するため、 `/FlexHAL/src/` フォルダ内に配置できるソースファイルは `FlexHAL.cpp` のみとする。これ以外の拡張子 `.c` `.cpp` のファイルは配置しない。
- C++の実装は、原則として対応する `.hpp` ヘッダファイル内に記述する。
- ヘッダファイル内の実装コードは `#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` / `#endif` マクロで囲み、ヘッダをインクルードするだけでは実装がコンパイルされないようにする。
- ビルド環境においては、`/FlexHAL/src/FlexHAL.cpp` のみをビルド対象とする。
- `FlexHAL.cpp` の内容は、まず `#define FLEXHAL_INTERNAL_IMPLEMENTATION` を記述し、その後に `#include "FlexHAL.hpp"` を記述する。これにより、ライブラリ全体のヘッダ内実装がこのファイルでコンパイルされる。
- インクルードガードは原則 `#pragma once` とするが、 `/FlexHAL/src/FlexHAL.h` および `/FlexHAL/src/FlexHAL.hpp` のみ `ifndef` + `define`マクロ方式とする。これはユーザーコード側で、`ifndef`マクロによって対象ファイルがincludeされているかどうかを判断可能にするため。

### コーディング規約 (一部)

#### 内部マクロの命名規則
- FlexHAL ライブラリが内部的に使用するマクロ名は、原則として `FLEXHAL_INTERNAL_` というプレフィックスで始める。
  - 例: `FLEXHAL_INTERNAL_IMPLEMENTATION`
- これにより、ユーザーがビルドオプションなどで定義する可能性のあるマクロとの名前衝突を避ける。

#### 階層構造を実現するための仕組み
- 環境依存コードが集約される `/src/flexhal/platform/` フォルダ内の各フォルダ階層には、ビルド環境の自動検出と必要なヘッダのインクルード制御のため、原則として `__detect.h` と `__include.h` を配置する。
- `__detect.h` 内には、所属するフォルダ階層がビルド環境に必要かどうかを検出するマクロを含める。
- `__include.h` 内には、検出結果に基づき、同階層に含まれる公開APIヘッダおよびサブフォルダの `__include.h` のインクルードを含める。
- 環境に依存しない共通機能のフォルダ (`/src/flexhal/utils/` など) には、これらのファイルは基本的に配置しない。

#### __detect.h , __include.h のルール
- 詳細は [03_Detection_Inclusion.md](./03_Detection_Inclusion.md) を参照
