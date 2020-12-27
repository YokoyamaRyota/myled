# ロボットシステム学課題
## 概要
raspberry pi3 model b+でのカソードコモンRGBLEDのデバイスドライバです。
redやblueなどの文字でledを光ります。
## 使用部品
1. raspberry pi3 model b+
2. 100Ω抵抗
3. 220Ω抵抗
4. RGBフルカラーLED 5mm OSTA5131A カソードコモン
## 動画
動作中の動画です。
　　　
   
[![Audi R8](http://img.youtube.com/vi/5zqgm7Bb5bU/0.jpg)](https://www.youtube.com/watch?v=5zqgm7Bb5bU　)
## インストール方法
次のコマンドを実行します。
```
git clone https://github.com/YokoyamaRyota/robosys2020_kadai1.git
```
## 実行方法
raspberry piのGNDとLEDのカソードを繋げ、P25と赤に100Ω抵抗、P24と緑、P23と青を220Ω抵抗を挟んで繋げます。
その後次のコマンドを実行します。
```
make
insmod myled.ko
chmod 666 /dev/myled0
```
光らせたい色を入力することでledが光ります。
```
echo red > /dev/myled0
```
この色以外にもblue,green,purple,yellow,whilteの色に対応しています。clearでledの光を消せます。
## ライセンス
GNU General Public License v3.0
