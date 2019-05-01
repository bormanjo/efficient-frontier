//
// Created by J-C Borman on 4/7/2019.
//

#ifndef CODE_EFFICIENTFRONTIER_H
#define CODE_EFFICIENTFRONTIER_H

#include "std_lib_facilities.h"
#include "Asset.h"
#include "Opt.h"
#include "Eigen/Dense"
#include <random>

class EfficientFrontier {
    /*
     * Constructs an efficient frontier from a portfolio object. Also provides maximized Sharpe and minimized variance
     * portfolios
     */
public:
    // TODO - Calculate Frontier portfolio given Return or Variance

    // Create the object
    explicit EfficientFrontier(Portfolio& port);

    // Build the frontier from random portfolios
    void build_frontier(long num_portfolios);

    // Calculate optimal portfolios
    void build_optimal_portfolios();

    Portfolio get_max_sharpe();
    Portfolio get_min_vol();

    // Return a matrix of [ E[r] | Vol ], each row corresponds to a portfolio
    Eigen::MatrixXd get_frontier();

    // Write the Efficient Frontier and Portfolios to files
    void write_frontier_portfolios();

    void plot_frontier_portfolios();

private:

    unsigned int num_stocks;

    vector<Stock> stocks;
    Eigen::VectorXd expected_returns;
    Eigen::MatrixXd covariance_mat;
    Eigen::MatrixXd _frontier;

    Portfolio max_sharpe, min_volatility, base;
};

#endif //CODE_EFFICIENTFRONTIER_H
