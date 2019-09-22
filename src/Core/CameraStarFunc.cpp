//
// Created by chudonghao on 19-9-21.
//

#include "CameraStarFunc.h"

using namespace ceres;
using namespace std;

vec2 solver_(double a1, double b1, double a2, double b2, double a3, double b3, double r1, double r2, double r3) {
  double init_a = a1, init_b = b1;
  Problem problem;
  problem.AddResidualBlock(new AutoDiffCostFunction<CameraStarFunc, 1, 1, 1>(new CameraStarFunc(1., a1, b1,r1)),
                           nullptr,
                           &init_a,
                           &init_b);
  problem.AddResidualBlock(new AutoDiffCostFunction<CameraStarFunc, 1, 1, 1>(new CameraStarFunc(1., a2, b2,r2)),
                           nullptr,
                           &init_a,
                           &init_b);
  problem.AddResidualBlock(new AutoDiffCostFunction<CameraStarFunc, 1, 1, 1>(new CameraStarFunc(1., a3, b3,r3)),
                           nullptr,
                           &init_a,
                           &init_b);
  Solver::Options options;
  options.linear_solver_type = DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  cout << summary.BriefReport() << endl;
  cout << init_a << ":" << init_b << endl;
  return vec2();
}
