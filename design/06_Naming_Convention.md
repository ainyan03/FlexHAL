# 命名規則

FlexHALライブラリで使用する命名規則について定義します。

## 基本方針
- 一貫性のある命名規則を採用
- C++の一般的な慣習に従う
- 可読性と意図の明確さを重視
- 既存の一般的な名称（Arduino互換など）は維持

## 詳細規則

### 名前空間
- スネークケース（小文字）を使用
```cpp
namespace flexhal
namespace flexhal::utils
namespace flexhal::internal  // 内部実装用
```

### クラス/構造体
- パスカルケースを使用
- 略語も大文字で開始
```cpp
class GpioPin              // デバイス制御クラス
struct PinConfig          // 設定用構造体
class I2cMaster          // 略語を含むクラス
```

### メンバ関数
- キャメルケースを使用
```cpp
class GpioPin {
  void digitalWrite();     // 動作を表す動詞で開始
  uint8_t readByte();     // 戻り値がある場合も動詞で開始
  void setPinMode();      // setterは'set'で開始
};
```

### グローバル関数（API）
- ユーティリティ関数はスネークケース
- 一般的な名称は維持
```cpp
// ユーティリティ関数
error_t delay_ms();       // 単位を_msで明示
uint32_t millis();       // 一般的な名称を維持

// 内部実装
namespace internal {
  void init_system();    // スネークケース
}
```

### 変数
- ローカル変数はスネークケース
- メンバ変数は'm_'プレフィックス + スネークケース
```cpp
// ローカル変数
uint8_t pin_number;      // スネークケース

// メンバ変数
class GpioPin {
private:
  uint8_t m_pin_number;  // m_プレフィックス
};
```

### 定数・enum
- 定数はスネークケース（大文字）
- enumの型名はパスカルケース
- enumの値もパスカルケース
```cpp
// 定数
constexpr uint8_t MAX_PINS = 16;    // 大文字スネークケース

// enum class
enum class PinMode {               // パスカルケース
  Input,                          // パスカルケース
  Output,
  InputPullup                     // 複合語もパスカルケース
};
```

### マクロ
- スネークケース（大文字）を使用
```cpp
#define FLEXHAL_VERSION_MAJOR 1
#define FLEXHAL_ASSERT(x) ...
```

## 特別なケース

### ハードウェア関連
- データシート準拠の名称はそのまま使用
```cpp
#define TCCR1A ...       // レジスタ名など
```

### 外部API互換
- 互換性のために元の命名を維持
```cpp
void digitalWrite();     // Arduino互換API
```

## 補足
- プライベートメンバ変数のプレフィックス'm_'は、スコープ解決演算子'::'を使用しない場合の名前の衝突を防ぐため
- 略語は最大3文字まで（例：I2C, SPI, USB）。それ以上は通常の単語として扱う
- テンプレートパラメータは単一の大文字（T, U, V）または意味のある名前をパスカルケースで
