//
// Created by J-C Borman on 4/18/2019.
//

#include "EfficientFrontier.h"

bool _debug = true;

// Constructors ------------------------------------------------------------------------------------------------------

EfficientFrontier::EfficientFrontier(Portfolio& port) {
    /*
     * Initializes the efficient frontier object with:
     *  - Vector of Stocks
     *  - Vector of Expected Returns
     *  - Covariance Matrix
     */
    this->base = port;

    this->stocks = port.get_stocks();
    this->num_stocks = this->stocks.size();

    Eigen::MatrixXd cov(this->num_stocks, this->num_stocks);
    Eigen::VectorXd rets(this->num_stocks);

    for (unsigned int i = 0; i < this->num_stocks; ++i) {
        rets(i) = port.get_stocks()[i].get_expected_return();

        for (unsigned int j = 0; j < this->num_stocks; ++j){
            cov(i, j) = port.get_covar_matrix(i, j);
        }
    }

    this->covariance_mat = cov;
    this->expected_returns = rets;
}


void EfficientFrontier::build_frontier(long num_portfolios) {
    /*
     * Creates a set of random portfolios then calculates expected returns and variances of all portfolios
     */

    // Set number of stocks
    int n = this->num_stocks;
    this->_frontier = Eigen::MatrixXd(num_portfolios, 2);

    if(_debug) cout << "Creating random ports" << endl;

    // Create a matrix of random portfolios
    Eigen::MatrixXd mat =           // Matrix of random double in [0, 2] =
            Eigen::MatrixXd::Random(num_portfolios, n) +    // Matrix of random double in [-1, 1]
            Eigen::MatrixXd::Ones(num_portfolios, n);       // Matrix of double 1's

    if(_debug) cout << "Calculating rowsums" << endl;

    // Calculate rowsums to normalize the values
    Eigen::VectorXd I = Eigen::VectorXd::Ones(n);
    Eigen::VectorXd sums = mat * I;

    // Temp values for each iteration
    Eigen::VectorXd w;
    double e_r, vol;

    if(_debug) cout << "Iterating" << endl;

    // Calculate (normalized) (long only) weights
    for (long i = 0; i < num_portfolios; ++i) {
        // Normalize weights
        w = mat.row(i) / sums(i);

        // Calc Expected return and variance
        e_r = w.dot(this->expected_returns);            // E[R] = <w, r>

        vol = w.transpose() * this->covariance_mat * w; // Variance = w^T * Sigma * w

        // Add to data
        this->_frontier(i, 0) = e_r;
        this->_frontier(i, 1) = vol;
    }

    if(_debug) cout << "build_frontier is done." << endl;
}

void EfficientFrontier::build_optimal_portfolios() {
    /*
     * Calculate the minimum volatility and maximum sharpe ratio portfolios
     */

    double rf = 0; // Risk free rate = 0


    Eigen::VectorXd min_var, max_sp;
    Eigen::VectorXd I = Eigen::VectorXd::Ones(this->num_stocks); // Identity vector
    Eigen::MatrixXd Sigma_inv = this->covariance_mat.inverse();


    // Minimum Variance = (S^-1 * I) / (I^T * S^-1 * I)
    min_var = (Sigma_inv * I) / (I.transpose() * Sigma_inv * I);

    /*
     * Maximum Sharpe Ratio (AKA Tangency portfolio) = (S^-1 * b) / (I^T * S^-1 * b)
     * - Where b = E[R] - rf * I;
     */

    Eigen::VectorXd b = this->expected_returns - rf * I;

    max_sp =
            (Sigma_inv * b) /
            (I.transpose() * Sigma_inv * b);


    vector<double> min_var_alloc(min_var.data(), min_var.data() + min_var.size());
    vector<double> max_sharpe_alloc(max_sp.data(), max_sp.data() + max_sp.size());


    Portfolio mv(this->stocks, min_var_alloc);
    this->min_volatility = mv;

    Portfolio ms(this->stocks, max_sharpe_alloc);
    this->max_sharpe = ms;

}

Portfolio EfficientFrontier::get_max_sharpe() {
    return this->max_sharpe;
}

Portfolio EfficientFrontier::get_min_vol() {
    return this->min_volatility;
}

Eigen::MatrixXd EfficientFrontier::get_frontier() {
    return this->_frontier;
}

void EfficientFrontier::write_frontier_portfolios() {

    ofstream frontier_file("../../output/frontier.txt");

    if(frontier_file){
        frontier_file << "Expected Return\tVariance" << endl;
        frontier_file << this->get_frontier() << endl;
        frontier_file.close();
    } else {
        error("Could not write Frontier to file");
    }

    ofstream port_file("../../output/portfolios.txt");

    if(port_file){
        port_file << this->max_sharpe.get_expected_return() << "\t" \
            << this->max_sharpe.get_variance() << "\t" << "\"Max Sharpe\"" << endl;
        port_file << this->min_volatility.get_expected_return() << "\t" \
            << this->min_volatility.get_variance()  << "\t" << "\"Min Variance\"" << endl;
        port_file << this->base.get_expected_return() << "\t" \
            << this->base.get_variance()  << "\t" << "\"Initial Portfolio\"" << endl;

        port_file.close();
    } else {
        error("Could not write Max Sharpe to file");
    }
}

void EfficientFrontier::plot_frontier_portfolios() {
    string plot_script = "../../plot-scripts/frontier.p";
    string plot = "gnuplot '" + plot_script + "'";
    system(plot.c_str());
}