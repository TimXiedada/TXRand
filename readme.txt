TXRAND – C语言随机数解决方案
---------------------------------
    C语言中有一个蛋疼的问题——生成随机数。C语言目前只有rand函数生成随机数，且功能有限，使用麻烦，所以TXRand横空出世。
    TXRand受python的random库启发，为用户提供了使用的函数，这些函数可以生成C语言中绝大多数内置类型的随机数，还可以打乱数组或从数组中随机取样（一项或多项）。
    使用本库，请将系统随机数生成器包装为call_os_rng函数，声明详见include/txrand.h。目前已提供Windows平台下的包装函数（使用CryptGenRandom函数） 。未来将包装/dev/random (*nix)，及stdlib.h下的rand函数。
    本软件为静态链接库，暂时只支持Visual C++，后续将支持其他平台。下一步将支持MinGW编译器及GNU/Linux平台。
    项目系个人开发，欢迎提Issues，也欢迎提交合并请求。
    许可协议详见源代码。
