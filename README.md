# mbed-microbit-template

mbed-microbit-templateは、GitHubテンプレートであり、C/C++言語を使ってランチェスター大学によって作成されたmicro:bitランタイムへの参照をあらかじめ含んでいます。  
The mbed-microbit-template is a GitHub template that comes pre-populated with references to it, the micro:bit runtime provides an easy to use environment for programming the BBC micro:bit in the C/C++ language, written by Lancaster University.　　

## 概要 Overview

micro:bitランタイムには、micro:bitが備えているLEDマトリクスディスプレイ機能といった各種ハードウェア機能だけでなく、ピアツーピアの無線通信やセキュアなBluetooth Low Energyサービスなど、micro:bitのプログラミングをより簡単かつ柔軟にするためのデバイスドライバやメカニズムが含まれています。micro:bitランタイムは、ARM MbedとNordic nrf51プラットフォーム上に構築されています。  
The micro:bit runtime contains device drivers for all the hardware capabilities of the micro:bit, and also a suite of runtime mechanisms to make programming the micro:bit easier and more flexible. These range from control of the LED matrix display to peer-to-peer radio communication and secure Bluetooth Low Energy services. The micro:bit runtime is proudly built on the ARM Mbed and Nordic nrf51 platforms, BBC micro:bit v1.5.  

ところが、Windows10とPython3環境で、ARM Mbedを使って、プログラミングを行う場合、いくつかの問題点があります。ここでは、Windows10でも容易にプログラミング環境を整える手順を説明します。  
However, there are a few problems when programming with ARM Mbed in Windows10 and Python3 environment. This section explains the steps to easily set up a programming environment on Windows 10.  

> 補足：BBC micro:bit v1.5をターゲットとしています。  
> Note: BBC micro:bit v1.5 is the target.  

# はじめに Get Started

## インストール Install 

1. [Git](https://git-scm.com/)
2. [Mercurial](https://www.mercurial-scm.org/)
3. [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) - [**6-2017-q2-update**](https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-win32-sha2.exe)  
※ GNU Arm Embedded Toolchain  
  → [gcc-arm-none-eabi-6-2017-q2-update-win32-sha2.exe](https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-win32-sha2.exe)
4. [Python3](https://www.python.org/downloads/)

## 環境構築 Environment

コマンドプロンプトを開き、次のコマンドを実行します。  
Open the command prompt and run below commands.

```CommandPrompt.cmd
py -3 -m pip install --upgrade virtualenv
mkdir c:\workubit
cd c:\workubit
py -3 -m virtualenv venv
C:\workubit\venv\Scripts\activate.bat
pip install mbed-cli git+https://github.com/jp-rad/mbed-microbit-win10-setup.git
mbed config --global GCC_ARM_PATH "C:\Program Files (x86)\GNU Tools Arm Embedded\6 2017-q2-update\bin"

```

# 使い方 How to use

## Using this template

1. GitHubのリポジトリを開きます。  
   Open the repository on github.  
   https://github.com/jp-rad/mbed-microbit-template

1. `Use this template`をクリックします。  
   Click `Use this template`.  
   [Here - https://github.com/jp-rad/mbed-microbit-template/generate]   (https://github.com/jp-rad/mbed-microbit-template/generate)


## git clone

コマンドプロンプトを開き、次のコマンドを実行します。  
Open the command prompt and run below commands.

```CommandPrompt.cmd
C:\workubit\venv\Scripts\activate.bat
cd c:\workubit
git clone <your GitHub Code URL>
```

## コンパイル compile

パソコンにBBC micro:bitを接続し、例えば、Dドライブとして認識させます。   
引き続き、コマンドプロンプトで次のコマンドを実行します。   
Connect the BBC micro:bit to the PC. For example, it will be recognized as drive D.  
Continue to run below commands at the command prompt.

```CommandPrompt.cmd
cd <your directory>
mbedubitwin10
mbed compile
copy .\BUILD\NRF51_MICROBIT\GCC_ARM\microbit-mbed.hex d:\
```

## Hello World!

```cpp
#include "MicroBit.h"
MicroBit uBit;
int main()
{
    uBit.init();
    uBit.display.scroll("HELLO WORLD! :)");
    release_fiber();
}
```

# リンク Links

## micro:bit runtime

[micro:bit runtime docs](https://lancaster-university.github.io/microbit-docs/)

## BBC Community Guidelines

[Microbit Community Guidelines](http://microbit.org/community/)
