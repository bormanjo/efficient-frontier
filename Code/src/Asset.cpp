//
// Created by J-C Borman on 4/7/2019.
//

#include "Asset.h"
#include <cmath>

// Asset -------------------------------------------------------------------------------------------------------


vector<double> Asset::calc_returns() {
    vector<double> rets = {0};
    for (unsigned int i = 1; i < this->data.size(); ++i) {
        rets.push_back(log(this->data[i]/this->data[i-1]));
    }
    return rets;
}

double Asset::calc_expected_return() {
    double sum = 0;

    for (auto ret:this->returns) {
        sum += ret;
    }

    return sum/this->returns.size();
}

double Asset::calc_variance() {
    double sum = 0;

    for(auto ret: this->returns){
        sum += pow(ret - this->expected_return, 2);
    }

    return sum / (this->returns.size() - 1);
}

double Asset::calc_skewness() {
    // TODO - Calculate Skewness
    return -3;
}

double Asset::calc_kurtosis() {
    // TODO - Calculate Kurtosis
    return -4;
}

double Asset::calc_covariance(Asset other){
    int a_n = this->returns.size()-1;
    int b_n = other.returns.size()-1;
    int min_data = (a_n >= b_n) ? b_n : a_n;

    auto a_end = this->returns.end(), b_end = other.returns.end();
    auto a_start = a_end - min_data, b_start = b_end - min_data;

    // create equal length vectors a,b
    vector<double> a(a_start, a_end);
    vector<double> b(b_start, b_end);

    // Calculate E[r]
    double sum_a = 0, sum_b = 0;
    for (unsigned int i = 0; i < a.size(); ++i) {
        sum_a += a[i];
        sum_b += b[i];
    }

    double e_a = sum_a / a.size();
    double e_b = sum_b / b.size();

    double sum = 0;
    for (unsigned int j = 0; j < a.size(); ++j) {
        sum += (a[j] - e_a) * (b[j] - e_b);
    }

    return sum / (a.size() - 1);
}

void Asset::set_data(vector<double> &adj_close) {
    this->data = adj_close;
}

// Setter functions set the corresponding data (do not return anything)
void Asset::set_returns() {
    this->returns = this->calc_returns();
}
void Asset::set_expected_return(){
    this->expected_return = this->calc_expected_return();
}
void Asset::set_variance(){
    this->variance = this->calc_variance();
}
void Asset::set_skewness(){
    this->skewness = this->calc_skewness();
}
void Asset::set_kurtosis(){
    this->kurtosis = this->calc_kurtosis();
}


// Stock -------------------------------------------------------------------------------------------------------


// (Constructor) that initializes a Asset object
Stock::Stock(string &ticker, vector<double> &data) {
    this->ticker = ticker;
    this->set_data(data);

    this->set_returns();
    this->set_expected_return();
    this->set_variance();
    this->set_skewness();
    this->set_kurtosis();
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


// Portfolio ---------------------------------------------------------------------------------------------------


Portfolio::Portfolio(vector<Stock> &stocks) {
    /*
     * Construct an equal weight portfolio
     */

    // Set the Stocks
    this->set_stocks(stocks);

    vector<double> w = {};
    double weight = 1.0 / stocks.size();

    for(unsigned int i = 0; i < stocks.size(); i++){
        w.push_back(weight);
    }

    // Set the weights
    this->set_weights(w);

    // Calculate covariance matrix
    this->covar_matrix = this->calc_covar_matrix();
}

Portfolio::Portfolio(vector<Stock> &stocks, vector<double> &weights) {
    // TODO - Implement Constructor when weights are provided (must sum to 1)



}

vector<Stock> Portfolio::get_stocks() {
    return this->stocks;
}

vector<double> Portfolio::get_weights() {
    return this->weights;
}

vector<vector<double>> Portfolio::get_covar_matrix() {
    return this->covar_matrix;
}

double Portfolio::get_covar_matrix(int i, int j) {
    return this->covar_matrix[i][j];
}

void Portfolio::set_stocks(vector<Stock> &Stocks) {
    this->stocks = Stocks;
}

void Portfolio::set_allocations(vector<double> &Allocations) {
    // TODO - Set % weights based on $ allocations
}

void Portfolio::set_weights(vector<double> &Weights) {
    if(Weights.size() != this->stocks.size()){
        error("Number of weights do not match number of stocks");
    }

    double sum = 0;
    for (unsigned int i = 0; i < Weights.size(); ++i) {
        if(Weights[i] < 0){
            error("Weights must be larger than 0");
        } else if(sum + Weights[i] > 1){
            error("Sum of weights is greater than 1");
        } else {
            sum += Weights[i];
        }
    }

    if(sum != 1){
        error("Sum of weights is not equal to 1");
    }

    this->weights = Weights;
}

vector<vector<double>> Portfolio::calc_covar_matrix(){
    /*
     * Calculates and returns the covariance matrix for the set of stocks
     */

    vector<vector<double>> sigma;
    double covar;

    // For each stock
    for(auto stock_a:this->stocks){
        // Generate a row of covariances
        vector<double> row = {};
        for (auto stock_b:this->stocks) {
            // If on diagonal
            if(stock_a.get_ticker() == stock_b.get_ticker()){
                // Set covar as the variance
                covar = stock_a.get_variance();
            } else {
                // Otherwise calculate covar
                covar = stock_a.calc_covariance(stock_b);
            }

            // printf("(%s, %s) = %f\n", stock_a.get_ticker().c_str(), stock_b.get_ticker().c_str(), covar);

            // Add covar to the row
            row.push_back(covar);
        }
        // Add the row to the covar matrix
        sigma.push_back(row);
    }

    // Return the matrix
    return sigma;
}