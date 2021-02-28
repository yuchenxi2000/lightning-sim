# Lightning Simulation

Chinese version https://github.com/yuchenxi2000/lightning-sim/blob/master/README_zh_cn.md

This is the source code for video https://www.bilibili.com/video/BV17g4y1q7Ad

## Methodology

1. Solve the poisson equation

   -Δφ = ρ/ε

2. We assign a initial breakdown point in our meshgrid, and choose the next breakdown point in the neighborhood of breakdown set randomly with weight of the cube of electric field. The breakdown points have the same electric potential as the initial point.

  Pj =|∇φj |^3 / ( ∑i|∇φi |^3 )

  We use the cube of electric field as weight, because the simulation result is similar to the reality. It's just an assumption.

## Usage

Compile and run lightning.cpp. You need the GNU Scienfitic Library (gsl). You can install it from https://www.gnu.org/software/gsl/

Use visualize.py to make animation. ffmpeg is required to write mp4 video.

