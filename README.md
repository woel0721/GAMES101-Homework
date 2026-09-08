# GAMES101-Homework
GAMES101课程作业合集，包括作业pdf和代码框架。

## Assignment0

已完成二维点 `(2, 1)` 逆时针旋转 45°，再平移 `(1, 2)` 的齐次坐标变换。
结果约为 `(1.70711, 4.12132, 1)`。

需要 CMake 3.16 或更高版本、支持 C++17 的编译器，以及 Eigen。
在仓库根目录运行：

```bash
cmake -S Assignment0 -B build/Assignment0 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/Assignment0 --target Transformation
./build/Assignment0/Transformation
```

### VS Code

打开整个 `GAMES101-Homework` 仓库文件夹。配置当前指向 Assignment0。
安装 C/C++、CMake Tools、Code Runner 扩展后，可使用右上角的 **Run Code** 按钮构建并运行。
F5 调试配置使用 CodeLLDB 扩展，并在启动前执行 CMake 构建。
运行配置适用于 macOS/Linux，需要终端可以找到 `cmake`。
