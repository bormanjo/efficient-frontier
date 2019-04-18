//
// Created by J-C Borman on 4/7/2019.
//

#ifndef CODE_STOCK_H
#define CODE_STOCK_H

#include "std_lib_facilities.h"


// Asset -------------------------------------------------------------------------------------------------------------


class Asset {
    /*
     * A generalized notion of an Asset. Has data and may calculate returns and moments
     */
public:
    vector<double> data;                    // Represents ordered time series pricing/value data (assumed to be ordered)
    vector<double> returns;
    double expected_return, variance, skewness, kurtosis;

    vector<double> calc_returns();
    double calc_expected_return();
    double calc_variance();
    double calc_skewness();
    double calc_kurtosis();
    double calc_covariance(Asset other);

    // Setter functions set the corresponding data (do not return anything)
    void set_data(vector<double> &adj_close);     // Method for setting the class' data
    void set_returns();
    void set_expected_return();
    void set_variance();
    void set_skewness();
    void set_kurtosis();
};


// Stock -------------------------------------------------------------------------------------------------------------


class Stock : public Asset {
    /*
     * A Asset object represents a stock that contains its Adjusted Close data. The object also has methods
     * to run calculations on that data.
     *
     * Mentality of Getter/Setter functions is so that computations need not be repeated. The object's data wont change
     * so neither should its summary statistics/moments
     */
public:
    // (Constructor) that initializes a Asset object
    Stock (string &ticker, vector<double> &data);

    // Getter functions - return the corresponding data from the member variables
    string get_ticker();

    // Overrides
    vector<double> get_returns();
    double get_expected_return();
    double get_variance();
    double get_skewness();
    double get_kurtosis();

private:
    // Member variables/properties (Data storage)
    string ticker;
    vector<double> returns;
};


// Portfolio ---------------------------------------------------------------------------------------------------------


class Portfolio : public Asset {
    /*
     * A Portfolio object represents a set of Stocks and their allocations. May include other data (TBD)
     *
     * TODO - Do we need to define any other functionality of this class?
     */
public:
    // Constructors --------------------------------------------------------------
    explicit Portfolio(vector<Stock> &stocks);
    Portfolio(vector<Stock> &stocks, vector<double> &allocations);

    // Getters
    vector<Stock> get_stocks();
    vector<double> get_weights();
    vector<vector<double>> get_covar_matrix();
    double get_covar_matrix(int i, int j);

private:
    // Member Variables
    vector<Stock> stocks;
    vector<double> weights, allocations;
    vector<vector<double>> covar_matrix;

    // Setters
    void set_stocks(vector<Stock> &stocks);
    void set_allocations(vector<double> &allocations);
    void set_weights(vector<double> &weights);

    vector<vector<double>> calc_covar_matrix();
};


#endif //CODE_STOCK_H
