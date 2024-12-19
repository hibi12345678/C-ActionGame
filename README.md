# Knight's Action

## 概要

 プレイヤーを動かして敵を倒すアクションゲームです。ステージを移動してボスを倒すとゲームクリアになります。
 プレイヤーは剣、弓、爆弾、盾など武器を選ぶことができ、敵を倒すとアイテムがドロップします。ドロップしたアイテムはアイテムメニューから選択することができます。
 ボスは体力が高く通常の攻撃のみではクリアが難しいので、アイテムと右上のヒントを参考に攻略しましょう。
  
## 操作方法

- WASD:移動
- Space:ジャンプ
- マウス左クリック:攻撃
- R:スキル
- Tab:アイテムメニュー
- Esc:メニュー

## システム要件

 - **オペレーティングシステム** : Windows 10以降
 - **グラフィックス** : OpenGL 4.2以上に対応したGPU
 - **依存関係** : Visual Studio 2019以降用のVisual C++再頒布可能パッケージ（x86）

## ビルド環境

 - **IDE** : Visual Studio 2019
 - **コンパイラ** : MSVC（Microsoft Visual C++）
 - **グラフィックスAPI** : OpenGL
 - **追加ライブラリ** : [GLEW](https://github.com/nigels-com/glew)
, [SOIL](https://github.com/littlstar/soil)
, [stb_image](https://github.com/nothings/stb) 
, [GLM](https://github.com/g-truc/glm)
, [SDL](https://www.libsdl.org/)
, [SDL2_image](https://github.com/libsdl-org/SDL_image)
, [SDL_mixer](https://github.com/libsdl-org/SDL_mixer)
, [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
, [imgui(ver. 1.91.3-docking)](https://github.com/ocornut/imgui)
, [FMOD Studi API Windows](https://www.fmod.com/)
,[rapidjson](https://github.com/Tencent/rapidjson)
  
## 実行方法

-**Gitを使ってクローンする場合**
 -以下のコマンドを使ってリポジトリをクローンできます：
```bash
  git clone https://github.com/hibi12345678/Knight-s-Action.git　
```
 -その後、クローンしたフォルダ内で[Release](./Release)フォルダ内のGame.exe ファイルを実行してゲームを開始できます。
 
-**ZIPファイルをダウンロードする場合**
 -ダウンロードしたZIPファイルを解凍し、解凍したフォルダ内にある [Release](./Release)フォルダ内のGame.exe ファイルを実行してゲームを開始できます。

Debugモードでビルドを行いたい場合は、Visual Studioで .sln ファイルを開き、ビルド設定を Debug に変更してからビルドを実行してください。DebugモードのみImGuiウィンドウが表示されます。
追加ライブラリは[External](./External)ファイル内に含まれています。

## License

This project is licensed under the BSD 3-Clause License.  
See the [LICENSE](./LICENSE) file for details.

## Acknowledgments

Some parts of this project are adapted from *Game Programming in C++* by Sanjay Madhav.  
Copyright (C) 2017 Sanjay Madhav. All rights reserved.

## Source
- **Model**   : Mixamo(https://www.mixamo.com/)
- **Sound**   : 効果音ラボ(https://soundeffect-lab.info/)
- **Texture** : Poly Haven(https://polyhaven.com/)
                pinterest(https://jp.pinterest.com/)
