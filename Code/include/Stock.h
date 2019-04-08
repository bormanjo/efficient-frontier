//
// Created by J-C Borman on 4/7/2019.
//

#ifndef CODE_STOCK_H
#define CODE_STOCK_H

#include "std_lib_facilities.h"

class Stock {
    /*
     * A Stock object represents a stock that contains its Adjusted Close data. The object also has methods
     * to run calculations on that data.
     *
     * Mentality of Getter/Setter functions is so that computations need not be repeated. The object's data wont change
     * so neither should its summary statistics/moments
     */
public:
    // (Constructor) that initializes a Stock object
    Stock (string ticker, vector<double> data);

    // Getter functions - return the corresponding data from the member variables
    string get_ticker();
    vector<double> get_returns();
    double get_expected_return();
    double get_variance();
    double get_skewness();
    double get_kurtosis();

private:
    // Member variables/properties (Data storage)
    string ticker;
    vector<double> px;
    vector<double> returns;
    double expected_return, variance, skewness, kurtosis;

    // Setter functions set the corresponding data (do not return anything)
    void set_returns();
    void set_expected_return();
    void set_variance();
    void set_skewness();
    void set_kurtosis();
};

#endif //CODE_STOCK_H
