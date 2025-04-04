## ヘッダ内実装のルール (`.hpp` ファイル)

実装部を構成するファイル・フォルダはすべて /FlexHAL/src/flexhal/ フォルダ配下に配置されます。
拡張子は `.hpp` で統一し、 `.h` ファイルは原則として使用しない。例外は `__detect.h` です。

### 名前空間とファイル・フォルダ階層の対応ルール
以下のような foo, bar 階層がある場合を例に説明します。

```
flexhal/           # 1. flexhal名前空間のルート
├─ foo.hpp         # 2. flexhal::foo 階層用ヘッダ
├─ foo/            # 3. flexhal::foo 階層用フォルダ
│  ├─ bar.hpp      # 4. flexhal::foo::bar 階層用ヘッダ
│  └─ bar/         # 5. flexhal::foo::bar 階層用フォルダ
│     ├─ IObj.hpp  # 6. flexhal::foo::Iobj インターフェイス定義ファイル
│     └─ Obj.hpp   # 7. flexhal::foo::Obj クラス定義ファイル
```

- ヘッダおよびフォルダは、同名の namespace と対応します。
- 例1の flexhal フォルダは namespace flexhal を表します。
- flexhal/foo/フォルダおよびflexhal/foo.hppヘッダは namespace flexhal::foo を表します。
- フォルダ内にサブフォルダを配置することで、namespace階層のネストを表します。
- フォルダとヘッダは原則ペアで配置します。
- ただし、ヘッダのみで足りる場合は、フォルダを配置しなくても良いものとします。
- ヘッダに記述できるものは以下の通り。
  - 子階層のヘッダのインクルード文 (子階層がある場合は必須)
  - 比較的小規模な構造体
  - 関数API
  - 変数やオブジェクトのインスタンス
- フォルダに配置できるものは以下の通り。
  - 子階層のフォルダ
  - 子階層のヘッダファイル
  - 比較的大きな構造体のヘッダファイル
  - クラス・インターフェイスのヘッダファイル
- 例では、 foo 階層は子階層 bar を持つため、 foo.hpp に #include "foo/bar.hpp" を記述します。
- 例では、 bar 階層は IObj.hpp および Obj.hpp を持つため、 bar.hpp に #include "bar/IObj.hpp" および #include "bar/Obj.hpp" を記述します。


### 実装コードの配置場所
- 実装部は、一般的には `.cpp` ですが、FlexHALにおいては、ヘッダファイル (`.hpp`) 内の後半部分に、`#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` マクロで囲み、マクロガードをつけてまとめて配置します。
- ファイルの前半部分には、クラス定義、関数宣言、インライン関数など、**インターフェース（宣言）のみを記述**し、実装の詳細は含めません。

```cpp
// --- 例: my_feature.hpp ---

#pragma once

#include <stdint.h>
// 他に必要なヘッダをインクルード

namespace flexhal {
namespace my_feature {

// --- クラス定義や関数宣言 (インターフェース) ---

class MyClass {
public:
    MyClass();
    void doSomething(int value);

private:
    int _internalState;
};

int utilityFunction(uint8_t input);

// インライン関数はここに書いてもOK (ただし短いものに限る)
inline int simpleInline(int x) {
    return x * 2;
}

// --- 実装セクション (ファイルの最後に配置) ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

MyClass::MyClass() : _internalState(0) {
    // コンストラクタの実装
}

void MyClass::doSomething(int value) {
    // メソッドの実装
    _internalState += value;
    // ...
}

int utilityFunction(uint8_t input) {
    // 関数の実装
    return static_cast<int>(input) + 10;
}

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION

} // namespace my_feature
} // namespace flexhal

```

### 実装コードの有効化

- 上記の `#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` / `#endif` で囲まれた実装コードは、通常はコンパイルされません。
- ライブラリ内で唯一のソースファイルである `/src/FlexHAL.cpp` の**冒頭でのみ** `#define FLEXHAL_INTERNAL_IMPLEMENTATION` を定義します。
- その後、`FlexHAL.cpp` で `#include "FlexHAL.hpp"` をインクルードすることで、ライブラリ全体のヘッダ内実装がこの `FlexHAL.cpp` ファイル内で一度だけコンパイルされる仕組みです。
- これにより、ライブラリ利用者がヘッダをインクルードした際に実装が重複してコンパイルされることを防ぎます。

### ルールの目的

- **インターフェースの明確化:** ヘッダファイルの前半を見るだけで、提供される機能の概要を把握しやすくします。
- **ビルドの簡略化:** 多くの環境で、ヘッダファイルをインクルードするだけでライブラリを利用可能にします。
- **コンパイル時間の最適化 (単一翻訳単位):** 実装を `FlexHAL.cpp` に集約することで、コンパイルの依存関係を単純化し、変更時の再コンパイル範囲を限定する効果が期待できます。
