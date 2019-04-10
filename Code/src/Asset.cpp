//
// Created by J-C Borman on 4/7/2019.
//

#include "Asset.h"


// Asset -------------------------------------------------------------------------------------------------------


vector<double> Asset::calc_returns() {
    // TODO - Calculate returns

    /*
    for px prices:
        ret[1] = px[1] - px[2]

    */
}

double Asset::calc_expected_return() {
    // TODO - Calculate expected return
}

double Asset::calc_variance() {
    // TODO - Calculate Variance
}

double Asset::calc_skewness() {
    // TODO - Calculate Skewness
}

double Asset::calc_kurtosis() {
    // TODO - Calculate Kurtosis
}

void Asset::set_data(vector<double> &data) {
    this->data = data;
}


// Stock -------------------------------------------------------------------------------------------------------


// (Constructor) that initializes a Asset object
Stock::Stock(string &ticker, vector<double> &data) {
    this->ticker = ticker;
    this->data = data;
}

// Public ----

// Getter functions - return the corresponding data from the member variables
string Stock::get_ticker(){
    return this->ticker;
}
vector<double> Stock::get_returns(){
    return this->returns;
}
double Stock::get_expected_return(){
    return this->expected_return;
}
double Stock::get_variance(){
    return this->variance;
}
double Stock::get_skewness(){
    return this->skewness;
}
double Stock::get_kurtosis(){
    return this->kurtosis;
}

// Private -----

// Setter functions set the corresponding data (do not return anything)
void Stock::set_returns(){
    this->returns = this->calc_returns();
}
void Stock::set_expected_return(){
    this->expected_return = this->calc_expected_return();
}
void Stock::set_variance(){
    this->variance = this->calc_variance();
}
void Stock::set_skewness(){
    this->skewness = this->calc_skewness();
}
void Stock::set_kurtosis(){
    this->kurtosis = this->calc_kurtosis();
}


// Portfolio ---------------------------------------------------------------------------------------------------


Portfolio::Portfolio(vector<Stock> &stocks) {
    // TODO - Implement Constructor when no weights are provided
}

Portfolio::Portfolio(vector<Stock> &stocks, vector<double> &allocations) {
    // TODO - Implement Constructor when weights are provided
}

vector<Stock> Portfolio::get_stocks() {
    return this->stocks;
}

vector<double> Portfolio::get_weights() {
    return this->weights;
}

void Portfolio::set_stocks(vector<Stock> &stocks) {
    this->stocks = stocks;
}

void Portfolio::set_weights(vector<double> &allocations) {
    // TODO - Set % weights based on $ allocations
}