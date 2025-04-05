# FlexHAL GPIO 機能 設計仕様

## 1. はじめに

FlexHALのGPIO機能は、様々なマイクロコントローラやプラットフォームにおけるGPIO（General Purpose Input/Output）操作を抽象化し、統一的で使いやすいインターフェースを提供することを目的とします。

主な目標は以下の通りです。

*   **ハードウェア抽象化:** プラットフォーム固有の実装詳細を隠蔽し、ポータブルなコード記述を可能にする。
*   **使いやすさ:** 直感的で理解しやすいAPIを提供する。
*   **柔軟性:** オブジェクト指向インターフェースによる拡張性と再利用性を提供する。
*   **パフォーマンス:** 必要に応じて低レベルアクセスも可能にし、性能要件にも応える。

本文書では、FlexHAL GPIO機能のアーキテクチャとAPI仕様について説明します。

## 2. APIの種類

FlexHAL GPIOは、用途に応じて選択可能な2種類のAPIを提供します。

### 2.1. インターフェースAPI (`IGpio`, `IPort`, `IPin`)

*   **特徴:**
    *   GPIOペリフェラル、ポート、ピンをオブジェクトとして扱う、階層化されたインターフェース。
    *   高い抽象化レベルを提供し、プラットフォーム間の差異を吸収。
    *   型安全性が高く、コンパイル時のエラーチェックが期待できる。
    *   オブジェクト指向設計により、テストやモック化が容易。
*   **推奨されるユースケース:**
    *   一般的なアプリケーション開発。
    *   再利用可能なハードウェア制御コンポーネントの作成。
    *   プラットフォーム非依存のコードを書きたい場合。

### 2.2. 関数型API (`flexhal::hal::gpio::*`)

*   **特徴:**
    *   C言語スタイルのシンプルな関数群。
    *   プラットフォーム固有のピン番号を直接使用し、低オーバーヘッドで動作。
    *   ハードウェアに近いレベルでの直接的な制御が可能。
*   **推奨されるユースケース:**
    *   FlexHALのHAL（Hardware Abstraction Layer）内部実装。
    *   パフォーマンスが最重要視される、クリティカルなコードセクション。
    *   特定のプラットフォーム機能を最大限に活用したい場合。

## 3. インターフェース階層 (`IGpio`, `IPort`, `IPin`)

インターフェースAPIは、以下の3つの階層的なインターフェースで構成されます。

```
+---------+       +---------+       +-------+
|  IGpio  | ----> |  IPort  | ----> | IPin  |
+---------+ 1   * +---------+ 1   * +-------+
    |                 |                 |
 (GPIOペリフェラル)   (ポート)          (ピン)
```

*   **`IGpio`**: 特定のGPIOペリフェラル全体（例: マイコン内蔵のGPIOA〜GPIOZ、I2C接続のGPIOエキスパンダ）を表します。`IPort` オブジェクトへのアクセスを提供します。
*   **`IPort`**: `IGpio` に属する1つのポート（例: GPIOA、GPIOB）を表します。ポート単位の一括操作（読み書き）と、そのポートに属する `IPin` オブジェクトへのアクセスを提供します。
*   **`IPin`**: `IPort` に属する1つの物理ピンを表します。ピン単位での詳細な設定や入出力操作を提供します。

## 4. `IGpio` インターフェース

GPIOペリフェラル全体を抽象化し、ポートへのアクセスを提供します。

```cpp
namespace flexhal { namespace hal { namespace gpio {

class IPort; // 前方宣言
class IPin; // 前方宣言

class IGpio {
public:
    virtual ~IGpio() = default;

    // ポート数を取得
    virtual uint32_t getNumberOfPorts() const = 0;

    // インデックスでポートを取得 (0 〜 getNumberOfPorts()-1)
    // 範囲外の場合はエラーまたは表明違反？
    virtual IPort& getPort(uint32_t port_index) = 0;
    virtual const IPort& getPort(uint32_t port_index) const = 0;

    // (オプション) ポート名でポートを取得
    // virtual IPort& getPortByName(char name) = 0;
    // virtual const IPort& getPortByName(char name) const = 0;

    // (オプション) ピン指定で直接 IPin を取得するヘルパー
    // 実装方法はプラットフォーム依存 (例: ネイティブ番号、ポート+ピン番号)
    // virtual IPin& getPin(/* TBD */) = 0;
    // virtual const IPin& getPin(/* TBD */) const = 0;

    // (オプション) ペリフェラル全体に関わる設定等
};

}}} // namespace flexhal::hal::gpio
```

*   `getPort` は、有効なポートインデックスに対して有効な `IPort` 参照を返す必要があります。
*   `getPortByName` や `getPin` ヘルパーは、利便性のために追加される可能性があります。具体的な引数や存在有無は実装に依存します。

## 5. `IPort` インターフェース

1つのGPIOポートを抽象化し、ポート単位の操作とピンへのアクセスを提供します。

```cpp
namespace flexhal { namespace hal { namespace gpio {

class IPin; // 前方宣言
class IGpio; // 前方宣言

class IPort {
public:
    virtual ~IPort() = default;

    // 所属する IGpio を取得
    virtual IGpio& getGpio() = 0;
    virtual const IGpio& getGpio() const = 0;

    // ポートインデックスを取得 (IGpio 内でのインデックス)
    virtual uint32_t getPortIndex() const = 0;

    // ポート内のピン数を取得 (通常 8, 16, 32 など)
    virtual uint32_t getNumberOfPins() const = 0;

    // ポート全体に値を書き込む (データサイズは実装依存？ or 固定？)
    // 例: 32bit固定の場合
    virtual base::error_t write(uint32_t value) = 0;

    // ポート全体の状態を読み取る (データサイズは実装依存？ or 固定？)
    // 例: 32bit固定の場合 (エラー情報は別途取得 or 戻り値で示す？)
    virtual uint32_t read() const = 0;

    // ポート内のピンをインデックスで取得 (0 〜 getNumberOfPins()-1)
    // 範囲外の場合はエラーまたは表明違反？
    virtual IPin& getPin(uint32_t pin_index_in_port) = 0;
    virtual const IPin& getPin(uint32_t pin_index_in_port) const = 0;

    // (オプション) ポート固有の設定等
};

}}} // namespace flexhal::hal::gpio
```

*   `write`/`read` のデータサイズ（`uint32_t` の部分）は、プラットフォームのレジスタ幅に合わせて可変にするか、最大の32bit等に固定するか検討が必要です。固定する場合、小さいポートでは上位ビットは無視されます。
*   `read` のエラーハンドリング（例: 読み取り失敗）の方法を明確にする必要があります。

## 6. `IPin` インターフェース

単一のGPIOピンを抽象化し、ピン単位の詳細な操作を提供します。

```cpp
namespace flexhal { namespace hal { namespace gpio {

class IPort; // 前方宣言
enum class PinMode : uint8_t; // 別途定義
struct PinConfig; // 別途定義

class IPin {
public:
    virtual ~IPin() = default;

    // 所属する IPort を取得
    virtual IPort& getPort() = 0;
    virtual const IPort& getPort() const = 0;

    // ポート内でのピンインデックスを取得
    virtual uint32_t getPinIndex() const = 0;

    // --- ピン設定 ---
    // 簡易モード設定 (Input, Output, InputPullup, etc.)
    virtual base::error_t setMode(PinMode mode) = 0;

    // 詳細設定 (速度、ドライブ強度、オープンドレインなどを含む構造体)
    virtual base::error_t setConfig(const PinConfig& config) = 0;

    // 現在のピンモード/設定を取得するメソッド (オプション)
    // virtual PinMode getMode() const = 0;
    // virtual PinConfig getConfig() const = 0;

    // --- デジタル入出力 ---
    virtual base::error_t digitalWrite(bool level) = 0;
    virtual int digitalRead() const = 0; // 戻り値: 0 or 1, エラー時は負の値

    // --- アナログ入出力 (オプション、未対応の場合はエラーを返す) ---
    // アナログ出力 (PWMなど) の値の範囲は要検討
    virtual base::error_t analogWrite(uint32_t value) = 0;
    // アナログ入力の値の範囲/分解能は要検討
    virtual int analogRead() const = 0; // 戻り値: 0 〜 Max, エラー時は負の値

    // --- 割り込み (オプション、将来構想) ---
    // virtual base::error_t attachInterrupt(InterruptCallback callback, InterruptMode mode) = 0;
    // virtual base::error_t detachInterrupt() = 0;

    // (オプション) ピン固有の状態取得等 (例: isOutput(), isPullupEnabled())
};

}}} // namespace flexhal::hal::gpio
```

*   `digitalRead`, `analogRead` はエラーを示すために負の値を返す規約とします。成功時は 0 以上の値を返します。
*   アナログ関連のメソッドや割り込み関連のメソッドは、全てのピンやプラットフォームでサポートされるとは限らないため、オプション扱いとし、未対応の場合は適切なエラーコード (`flexhal::base::status::NotSupported` など) を返す必要があります。
*   `PinMode` enum と `PinConfig` 構造体の具体的な内容は別途定義が必要です。

## 7. 関数型API

インターフェースAPIとは別に、低レベルアクセスやHAL内部実装のための関数型APIも提供されます。

```cpp
namespace flexhal { namespace hal { namespace gpio {

// (例)
base::error_t set_mode(uint8_t native_pin, PinMode mode);
base::error_t digital_write(uint8_t native_pin, bool level);
int digital_read(uint8_t native_pin); // 戻り値: 0 or 1, エラー時は負の値
// ... 他の関数 ...

}}} // namespace flexhal::hal::gpio
```

*   **役割:** 速度が最優先される場面や、特定のプラットフォーム機能に直接アクセスしたい場合に使用します。主に環境依存層 (HAL 実装) の内部で利用されることを想定します。
*   **ピン指定:** プラットフォーム固有のピン番号 (`uint8_t` など、プラットフォーム依存の数値型) を直接引数に取ります。どの数値がどのピンに対応するかは、使用するプラットフォームのドキュメントを参照する必要があります。
*   **安全性:** ピン番号の妥当性チェックやエラーハンドリングは限定的です。不正なピン番号を指定した場合の動作は未定義となる可能性があります。使用者の責任で正しく利用する必要があります。
*   **提供場所:** 通常、プラットフォーム固有の名前空間 (`flexhal::internal::platform::xxx::hal::gpio` など) の下に実装が提供されますが、共通の `flexhal::hal::gpio` 名前空間からも利用可能になる場合があります（実装による）。

## 8. エラーハンドリング

*   インターフェースAPIのメソッド（`read`系を除く）は、操作の結果を示す `flexhal::base::error_t` 型を返すことを原則とします。成功時は `flexhal::base::status::Ok` (数値としては 0)、エラー時は対応するエラーコード (負の値) を返します。
*   `digitalRead`, `analogRead`, `IPort::read` など、読み取り値を直接返す必要があるメソッドは、成功時には 0 以上の値を、エラー発生時には負の値を返す規約とします。エラーの種類を詳細に知りたい場合は、別途エラー状態を取得するメソッドが必要になる可能性があります（現時点では未定義）。
*   関数型APIも、可能な範囲で同様のエラーコード規約に従いますが、インターフェースAPIほどの厳密なエラーチェックは期待できません。

## 9. プラットフォーム実装ガイドライン

各プラットフォーム（STM32, ESP32, Arduino フレームワーク, GPIOエキスパンダなど）で `IGpio`, `IPort`, `IPin` インターフェースを実装する際のガイドラインです。

*   **`IGpio` 実装:**
    *   ターゲットとなるGPIOペリフェラル（またはそのグループ）を表現します。
    *   `getNumberOfPorts()`, `getPort()` で、そのペリフェラルのポート構成に合わせて `IPort` オブジェクトを管理・返却します。
*   **`IPort` 実装:**
    *   ハードウェアのポートレジスタに対応付けられることが多いです。
    *   `getNumberOfPins()` でポートの物理的なピン数を返します。
    *   `write`/`read` は、ポートレジスタへの一括アクセスを実装します。データサイズに注意が必要です。
    *   `getPin()` で、ポート内の物理ピンに対応する `IPin` オブジェクトを管理・返却します。
*   **`IPin` 実装:**
    *   ピンごとの設定レジスタや入出力レジスタのビット操作を実装します。
    *   `setMode`, `setConfig` で、プラットフォーム固有の設定オプションを抽象化します。
    *   `digitalWrite/Read`, `analogWrite/Read` で、ピン単位の入出力を実装します。
*   **仮想ポート:**
    *   Arduinoのように明確な「ポート」概念がないプラットフォームでは、実装内で仮想的なポートを定義します。
    *   例えば、Arduino Uno では D0-D7 をポート0 (8ピン)、D8-D13, A0-A5 をポート1 (12ピン？) のように、実装の都合の良い単位でグルーピングし、`IGpio` がそれらを `IPort` として提供します。
    *   `IPort::write/read` は、仮想ポート内のピンに対してループ処理を行うなどの実装になる可能性があります。

これにより、ユーザーはプラットフォームの違いを意識することなく、統一された `IGpio`, `IPort`, `IPin` インターフェースを通じてGPIOを操作できます。
