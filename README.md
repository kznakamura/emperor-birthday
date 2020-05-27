# Name
emperror_birthday

## Author
K. Z. Nakamura

## Overview
天皇誕生日がすべて祝日になるとしたら、天皇誕生日の期待値が365日を超えるまでに何世代必要なのかを漸化式とMCの両方で調べてみた。

## 設計思想
- 解析はC++、文字列の処理はpython、描画はrootのマクロ
- 解析プログラムは基本的には描画機能を持たせない（今回は遊びでgifファイルを生成させてみたが）

## Install
`$ git clone https://github.com/kznakamura/emperor-birthday.git`

## プログラム名と機能
- src/calc_recrrence.cc
  - 確率漸化式を計算
  - 解析ファイルをdata/recrrence.rootに保存
- src/calc_mc.root
  - MCから天皇誕生日の日数を計算
  - 解析ファイルをdata/mc.rootに保存
  - 1年が天皇誕生日で埋まってゆく様子をplot/mc_event.gifに保存
- script/form_emperor_birthday.ipynb
  - wikipediaの天皇誕生日一覧から、C++が読みやすいように日付の形式に変換
  - 生年月日が不明の場合は没年月日を生年月日として扱うことにした
  - 天皇の世代は、南北朝の扱いが面倒なのでwikipediaの上からの順番に振り直している
  - 抽出したデータをdata/formed_csv_data.rootに保存
- src/ana_real_data.cc
  - form_emperor_birthday.pyが出力したrootファイルを解析
  - 解析ファイルをdata/real_data.rootに保存
- macro/plot_expected_filled_day.C
  - recrrence.root, mc.root, real_data.rootを読み込んでplot
  - マクロは描画に専念させるつもりでいたが、residualをplotするために解析が必要になって、ごちゃごちゃしたプログラムになってしまった。

### C++プログラムのコンパイル方法
- `$ sh make.sh filename(w/o .cc)`
- bin以下に実行ファイルが生成される
