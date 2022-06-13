[toc]

# NetLibDemo
`NetLibDemo`是一款基于Linux环境、主要使用C++写的网络库。  
源自冯岳松大佬的 [30天自制C++服务器](https://github.com/yuesong-feng/30dayMakeCppServer) ，学习并手写了一遍后受益匪浅。本库主要源自其 [day14](https://github.com/yuesong-feng/30dayMakeCppServer/tree/main/code/day14) 及之前的代码，并按自己的习惯风格进行了仿写调整。原仓库里有关于服务器一步步搭建起来的思路与解释，搭配着源码更易理解。本仓库仅为了个人学习及总结，对部分知识点的理解并不一定准确，建议大家直接看原仓库。  

该网络库涉及到的知识点：
- epoll

# 如何编译
进入`NetLibDemo`所在目录后，输入以下命令：  
```shell
/NetLibDemo$ mkdir build && cd build
/NetLibDemo/build$ cmake ..
```
进行下面**格式化、代码检查、代码分析**前确保`build_support`中相应文件有执行权限（可提前`chmod`一下）；
进行代码格式化（这一步会利用根目录的`.clang-format`中的规则进行代码格式化）：
```shell
/NetLibDemo/build$ make format
```
用`cpplint`检查代码：
```shell
NetLibDemo/build$ make cpplint
```
用`clang-tidy`分析代码：
```shell
NetLibDemo/build$ make clang-tidy
```
> 注： 由于`cpplint`和`clang-tidy`报告的异常太多，而且其部分规则自己也没习惯就暂时还没有按提示做相应更改。刚开始做的话，不妨每次做完就检查并修改下。

用`make`生成网络库（会在`NetLibDemo/build/lib`目录下生成`libpine_shared.so`）：
```shell
NetLibDemo/build$ make
```
用`make`继续生成测试用的可执行文件（会在`NetLibDemo/build/bin`目录下生成相应可执行文件）：
```shell
NetLibDemo/build$ make server
NetLibDemo/build$ make multiple_client
NetLibDemo/build$ make single_client
```
运行测试服务器：
```shell
NetLibDemo/build$ ./bin/server
NetLibDemo/build$ ./bin/single_client
```
# 环境说明
为了更好地支持**工程化**，原库中涉及到了以下工具：
- `CMake`
- `Clang`编译器
- `clang-format`
- `clang-tidy`
- `cpplint`  

上述工具需要提前安装配置好。


# 目录说明
## 源码目录
```shell
/NetLibDemo$ tree
.
├── .clang-format
├── .clang-tidy
├── build_support
│   ├── clang_format_exclusions.txt
│   ├── cpplint.py
│   ├── run_clang_format.py
│   ├── run_clang_tidy_extra.py
│   └── run_clang_tidy.py
├── CMakeLists.txt
├── src
│   ├── acceptor.cpp
│   ├── buffer.cpp
│   ├── channel.cpp
│   ├── CMakeLists.txt
│   ├── connection.cpp
│   ├── epoll.cpp
│   ├── event_loop.cpp
│   ├── include
│   │   ├── acceptor.h
│   │   ├── buffer.h
│   │   ├── channel.h
│   │   ├── connection.h
│   │   ├── epoll.h
│   │   ├── event_loop.h
│   │   ├── macros.h
│   │   ├── server.h
│   │   ├── socket.h
│   │   ├── thread_pool.h
│   │   └── util.h
│   ├── server.cpp
│   ├── socket.cpp
│   ├── thread_pool.cpp
│   └── util.cpp
└── test
    ├── CMakeLists.txt
    ├── multiple_client.cpp
    ├── server.cpp
    ├── single_client.cpp
    └── thread_pool_test.cpp
```
其中：
- `src`: 源码目录，其中`include`是头文件目录；源码编译成功后会生成一个库文件（非可执行文件）；
- `test`: 测试目录，用于验证/使用该网络库；测试目录内文件编译后会生成相应的可执行文件；
- `build_support`: 构建辅助目录，用于支持编译构建过程的顺利执行；
- `.clang-format`: 自定义的`clang-format`检查规则；
- `.clang-tidy`: 自定义的`clang-tidy`检查规则；

# 手敲建议
当然还是建议优先按原仓库那样一步步地去实现，该仓库仅是归档总结，就简单粗暴了很多。
- Step 1: 环境准备；将上文提到的相关工具（如`clang-format`等）安装配置好；
- Step 2: 根据目录说明，先把源码敲一遍；
- Step 3: 把工程化的相关文件敲一遍；
- Step 4: 编译-改错-编译-改错-...-运行；
- Step 5: 相关知识点归纳总结。

