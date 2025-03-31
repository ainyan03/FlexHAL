# FlexHAL

> **重要なお知らせ**: このリポジトリは**Windsurf AIを使用したライブラリ開発の実験的プロジェクト**です。現時点では一般利用を推奨しておらず、APIや機能は予告なく変更される可能性があります。また、現在はIssueやプルリクエストを受け付けておりません。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)](https://www.arduino.cc/reference/en/libraries/)
[![Experimental](https://img.shields.io/badge/Status-Experimental-red)]()

FlexHALは柔軟なハードウェア抽象化レイヤー（HAL）ライブラリです。ESP32やRP2040などのマイコン向けの開発を効率化し、さらにSDLを使用してデスクトップ環境でのエミュレーションも可能にします。

## 特徴 ✨

- マルチプラットフォーム対応（ESP32, RP2040, SDL）
- 3層構造アーキテクチャ（プラットフォーム・フレームワーク・RTOS）
- シンプルで一貫性のあるAPI
- 初心者から上級者まで使いやすい複数のアクセス方法
- デスクトップ環境でのエミュレーション機能

## 必要な環境 🛠

- C++11以上に対応したコンパイラ
- 各プラットフォームに応じた開発環境:
  - ESP32: ESP-IDF または Arduino ESP32
  - RP2040: Pico SDK または Arduino RP2040
  - SDL: SDL2ライブラリ（デスクトップシミュレーション用）

## 使用例 💡

基本的な使い方については[こちらの例](/examples)を参照してください。

詳細な仕様やAPIドキュメントは[docs/spec](/docs/spec)フォルダにあります。

## アーキテクチャ 🏗️

FlexHALは独自の3層構造を採用しています：

1. **プラットフォーム層** - ハードウェア固有の実装（ESP32, AVR, RP2040, デスクトップ）
2. **フレームワーク層** - フレームワーク固有の実装（Arduino, ESP-IDF, SDL）
3. **RTOS層** - RTOS固有の実装（FreeRTOS, NoOS, Desktop）

詳細なアーキテクチャ情報は[docs/spec/architecture.md](/docs/spec/architecture.md)を参照してください。

## 開発への貢献 👩‍💻

FlexHALプロジェクトの開発に興味がある方は、[DEVELOPMENT.md](DEVELOPMENT.md)を参照してください。ここには開発フローやワークフローの詳細が記載されています。

プロジェクトは以下の6ステップの開発フローを採用しています：

1. 要件定義書の作成
2. 要件定義書の手直し
3. タスクの分解
4. GitHub Issuesへの登録
5. PRベースの開発
6. レビュー

## ライセンス 📄

このプロジェクトはMITライセンスの下で公開されています。詳細は[LICENSE](LICENSE)ファイルを参照してください。
