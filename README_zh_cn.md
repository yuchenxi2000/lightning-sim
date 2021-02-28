# Lightning Simulation

视频 https://www.bilibili.com/video/BV17g4y1q7Ad 的源码

## 原理

1. 求解泊松方程

   -Δφ = ρ/ε

2. 在击穿的格点附近的点以场强三次方为权重选择下一个击穿的格点，击穿格点电势和闪电电势相同

  Pj =|∇φj |^3 / ( ∑i|∇φi |^3 )

  选择三次方的原因，三次方模拟出的闪电的行为和现实最接近，一次方或者二次方闪电团成一团

3. 重复上述步骤

## 依赖

模拟闪电请编译运行lightning.cpp。你需要 GNU Scienfitic Library (gsl)，可以从这里下载 https://www.gnu.org/software/gsl/

运行 visualize.py 来可视化/制作动画。需要安装ffmpeg。

