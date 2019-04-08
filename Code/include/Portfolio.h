//
// Created by J-C Borman on 4/7/2019.
//

#ifndef CODE_PORTFOLIO_H
#define CODE_PORTFOLIO_H

#include "std_lib_facilities.h"
#include "Stock.h"

class Portfolio {
    /*
     * A Portfolio object represents a set of assets and their allocations. May include other data (TBD)
     *
     * TODO - Do we need to define any other data in this class?
     */
public:
    // Constructors --------------------------------------------------------------
    Portfolio(vector<Stock>);
    Portfolio(vector<Stock>, vector<double> allocations);

    // Getters
    vector<Stock> get_stocks();
    vector<Stock> get_weights();

private:
    // Member Variables
    vector<Stock> stocks;
    vector<double> weights, allocations, returns;

    // Setters
    void set_stocks();
    void set_weights();
};

#endif //CODE_PORTFOLIO_H
