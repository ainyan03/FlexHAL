# テスト構造とガイドライン

## 1. テストディレクトリ構造

テストは以下の構造で整理されています：

```
test/
└── unit/              # カテゴリフォルダ
    └── test_utils/    # テストスイート（test_プリフィックス必須）
        └── time/      # 機能別テスト
            └── time.cpp  # テストファイル
```

### 1.1 命名規則

- カテゴリフォルダ: 機能の大分類（例：`unit`, `integration`, `acceptance`）
- テストスイート: `test_`プリフィックス + 機能名（例：`test_utils`, `test_gpio`）
- テストファイル: 機能名.cpp（例：`time.cpp`, `gpio.cpp`）

## 2. テストフレームワーク

- GoogleTestを使用
- テストファイルには以下を含める：
  - テスト実装（inline関数）
  - テストケース（TESTマクロ）
  - main関数（GoogleTest初期化）

### 2.1 テストファイル構成

```cpp
#include <gtest/gtest.h>
#include "対象機能のヘッダ"

namespace flexhal::test::<機能名> {
  // テスト実装（inline関数）
  inline bool test_something() {
    // テストロジック
  }
} // namespace

// テストケース
TEST(TestSuite, TestCase) {
  EXPECT_TRUE(flexhal::test::<機能名>::test_something());
}

// main関数
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
```

## 3. PlatformIO設定

### 3.1 テスト環境設定

```ini
[test_base]
build_type = test
test_framework = googletest
lib_deps = googletest
test_build_src = yes
build_flags =
  -I.
```

### 3.2 テストフィルター

```ini
[env:native_unit_test]
extends = target_native, test_base
test_filter = unit/*  # カテゴリフォルダを指定
```

## 4. テスト実装ガイドライン

### 4.1 タイミング関連テスト

- 時間に関するテストでは、環境による誤差を考慮する
- 許容範囲は以下を参考に設定：
  - ミリ秒単位: 10%程度（例：`delay_ms`）
  - マイクロ秒単位: 50%程度（例：`delay_us`）
  - 時間の一貫性: 20%程度（例：`millis`と`micros`の比較）

### 4.2 テストケースの命名

- TestSuite: 機能の大分類（例：`TimeTest`, `GPIOTest`）
- TestCase: テストする具体的な機能（例：`DelayMs`, `DigitalWrite`）

### 4.3 エラー処理

- エラーコードの確認: `to_error(base::status::ok)`などと比較
- 境界値のテスト: 最小値、最大値、無効値など
- タイムアウト: 長時間のテストには適切なタイムアウトを設定

## 5. CI/CD統合

- 各プラットフォーム（Native, ESP32など）でテストを実行
- テストの並列実行に対応
- テスト結果のレポート生成と保存
