## ヘッダ内実装のルール (`.hpp` ファイル)

FlexHAL では、ライブラリのビルドを簡素化し、Arduino IDE などでの互換性を高めるため、実装コードを原則としてヘッダファイル (`.hpp`) 内に記述します。

### 実装コードの配置場所

- 各ヘッダファイル (`.hpp`) 内の実装コードは、**ファイルの最後にまとめて記述します**。
- ファイルの前半部分には、クラス定義、関数宣言、インライン関数など、**インターフェース（宣言）のみを記述**し、実装の詳細は含めません。
- ファイルの末尾に、実装コードを記述するセクションを設けます。このセクションは `#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION` マクロで囲みます。

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

} // namespace my_feature
} // namespace flexhal

// --- 実装セクション (ファイルの最後に配置) ---
#ifdef FLEXHAL_INTERNAL_IMPLEMENTATION

namespace flexhal {
namespace my_feature {

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

} // namespace my_feature
} // namespace flexhal

#endif // FLEXHAL_INTERNAL_IMPLEMENTATION
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
