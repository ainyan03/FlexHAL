## API仕様ルール

## 🔗 インスタンスの取得と管理

### インスタンスの取得
```cpp
namespace flexhal::foo {
  // インターフェース定義
  class interface {
  public:
    virtual ~interface() = default;
    virtual error_t doSomething() = 0;
  };

  // インスタンスの取得
  using instance_ptr = std::shared_ptr<interface>;
  static instance_ptr getInstance();     // シングルトンインスタンスを取得
  static instance_ptr createInstance(); // 新規インスタンスを作成
}
```

### APIの使用方法の例
- 何らかの機能を使う例 (ここでは仮に`foo`とする)
```cpp
  // 機能のデフォルト実装なインスタンスを生成
  auto foo_impl = flexhal::foo::createInstance(); // 新規に生成する
  // または
//auto foo_impl = flexhal::foo::getInstance(); // 既存のものがあれば返す。なければ生成して返す

  // インスタンスのメンバを経由して何らかの機能を使う方法
  foo_impl->doSomething();

  // FlexHAL クラス経由で何らかの機能を使う例
  FlexHAL Hal;

  // 初期化関数は最初に呼ぶ
  // (環境依存の初期化や、各メンバのデフォルトインスタンスが設定される)
  Hal.init();

  // FlexHAL のFooメンバを経由して何らかの機能を使う
  Hal.Foo.doSomething();
```

## ✨ エラーハンドリング

### 基本ルール
1. 全てのメソッドは可能な限り `error_t` を返す
2. エラーコードは `base/status.hpp` で定義されたものを使用
3. エラー発生時は即座にエラーコードを返す

### エラーチェック
```cpp
// エラーチェックの例
error_t doSomething() override {
  // 事前条件のチェック
  if (!isInitialized()) {
    return to_error(status::not_initialized);
  }

  // 処理の実行
  if (/* エラー条件 */) {
    return to_error(status::error);
  }

  return to_error(status::ok);
}
```

## 📚 実装の切り替え

### プラットフォーム別の実装
```cpp
namespace flexhal::foo {
  // 各プラットフォームの実装
  class esp32_implementation : public interface { ... };
  class avr_implementation : public interface { ... };
  class generic_implementation : public interface { ... };

  // デフォルト実装の選択
  #if defined(ESP32)
    using default_implementation = esp32_implementation;
  #elif defined(ARDUINO_ARCH_AVR)
    using default_implementation = avr_implementation;
  #else
    using default_implementation = generic_implementation;
  #endif
}
```

### カスタム実装
```cpp
// ユーザー独自の実装
class my_implementation : public flexhal::foo::interface {
  error_t doSomething() override {
    // 独自の実装
    return to_error(status::ok);
  }
};

// 独自実装の使用
auto custom_impl = std::make_shared<my_implementation>();
Hal.Foo.setImpl(custom_impl);
```

### 任意の実装を選んで使う例
```cpp
  // 任意の機能インスタンスを生成する
  auto foo_impl = flexhal::internal::framework::arduino::foo::createInstance();

  // インスタンスのメンバを経由して何らかの機能を使う方法
  foo_impl->doSomething();

  // FlexHAL クラス経由で何らかの機能を使う例
  FlexHAL Hal;

  // 初期化関数は最初に呼ぶ
  Hal.init();

  // 任意の実装を明示的に指定する
  Hal.Foo.setImpl(foo_impl);

  // FlexHAL のFooメンバを経由して何らかの機能を使う
  Hal.Foo.doSomething();
```
