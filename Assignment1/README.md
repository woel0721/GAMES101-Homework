# Assignment 1：旋转与投影

已完成基础部分和提高项。

- `get_model_matrix(angle)`：绕 Z 轴旋转，角度单位为度。
- `get_projection_matrix(...)`：按“透视挤压，再正交投影”的顺序构造矩阵。
- `get_rotation(axis, angle)`：使用罗德里格斯公式实现绕任意过原点轴旋转；自动归一化非零轴，零向量会抛出异常。

投影参数 `zNear`、`zFar` 表示正距离。构造矩阵时转换为相机空间坐标 `n = -zNear`、`f = -zFar`；透视除法后，近平面映射到 `+1`，远平面映射到 `-1`，与课件约定一致。

## 编译与运行

依赖 CMake、C++17 编译器、Eigen 和 OpenCV。macOS 可通过 `brew install cmake eigen opencv` 安装依赖。

在仓库根目录执行：

```bash
cmake -S "Assignment1/代码框架" -B build/Assignment1 -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Assignment1 --parallel
cd build/Assignment1
./Rasterizer
```

点击图像窗口后，按 `a` / `d` 改变旋转角度，按 `Esc` 退出。

命令行导出图片：

```bash
./Rasterizer -r 20
./Rasterizer -r 30 image.png
```

省略文件名时输出为当前目录中的 `output.png`。

提高项函数已实现，主程序默认仍绕 Z 轴旋转。例如，将 `set_model` 的参数改为 `get_rotation({1, 1, 0}, angle)`，即可体验绕斜轴旋转。

## 验证

- 编译以及命令行图片输出。
- Z 轴旋转方向、任意轴旋转、非单位轴、旋转保持长度和轴方向。
- 近远平面深度映射、近平面边界以及不同视角和宽高比下的投影。

本机 VS Code 的操作方式见 [环境配置.md](环境配置.md)。
