# FlexHAL GPIO設計仕様書 🔌

## 1. 基本コンセプト ✨

FlexHALのGPIO設計は、初心者から上級者まで使いやすい柔軟なAPIを提供することを目指しています。以下の特徴があります：

- **共通インターフェース**: すべてのGPIO実装は共通のインターフェースを実装
- **複数アクセス方法**: 習熟度に応じた複数のアクセス方法を提供
- **型安全**: テンプレートを活用した型安全な実装選択
- **拡張性**: 新しいプラットフォームやフレームワークの追加が容易

## 2. クラス階層 🏗️

```
flexhal::gpio::GPIO (インターフェースクラス)
  ├── flexhal::gpio::ArduinoGPIO
  ├── flexhal::gpio::EspIdfGPIO
  ├── flexhal::gpio::NativeGPIO
  └── flexhal::gpio::SDLGPIO
```

## 3. アクセス方法 🔑

FlexHALのGPIOには、ユーザーの習熟度に応じた3つのアクセス方法があります：

### 3.1 初心者向け: 簡易ファクトリー関数

```cpp
// 環境に最適なGPIO実装を自動選択
auto gpio = flexhal::gpio::Factory::createDefault();
gpio->pinMode(13, OUTPUT);
gpio->digitalWrite(13, HIGH);
```

### 3.2 中級者向け: 型指定ファクトリー関数

```cpp
// 具体的な型を指定して実装を選択
auto arduino_gpio = flexhal::gpio::Factory::create<flexhal::gpio::ArduinoGPIO>();
auto espidf_gpio = flexhal::gpio::Factory::create<flexhal::gpio::EspIdfGPIO>();

arduino_gpio->pinMode(13, OUTPUT);
arduino_gpio->digitalWrite(13, HIGH);
```

### 3.3 上級者向け: 直接インスタンス化

```cpp
// 直接インスタンス化して詳細設定も可能
auto custom_gpio = std::make_shared<flexhal::gpio::ArduinoGPIO>(custom_config);
custom_gpio->pinMode(13, OUTPUT);
custom_gpio->digitalWrite(13, HIGH);
```

## 4. 実装の選択ロジック 🧠

`Factory::createDefault()`は以下のロジックで実装を選択します：

1. コンパイル時に利用可能なフレームワークを検出
2. 優先順位に基づいて最適な実装を選択
   - Native > ESP-IDF > Arduino > SDL の順
3. 選択した実装のインスタンスを生成して返却

## 5. 拡張方法 🚀

新しいプラットフォームやフレームワーク向けのGPIO実装を追加するには：

1. `GPIO`インターフェースを継承した新しいクラスを作成
2. 必要なメソッドをすべて実装
3. ファクトリー関数に新しい型を登録

```cpp
// 例: 新しいプラットフォーム向けGPIO実装
class NewPlatformGPIO : public GPIO {
public:
  void init() override { /* 初期化処理 */ }
  void end() override { /* 終了処理 */ }
  void pinMode(uint8_t pin, PinMode mode) override { /* ピンモード設定 */ }
  // 他のメソッドも実装
};
```


