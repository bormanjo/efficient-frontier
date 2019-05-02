#include "std_lib_facilities.h"
#include <curl/curl.h>
#include <regex>
#include "YFData.h"
#include "Eigen/Dense"
#include "EfficientFrontier.h"

void test_yf_data(){
    YFData yf;

    Stock aapl = yf.get_stock("AAPL", 1262304000, 1514764800);

    cout << aapl.get_ticker() << "'s Adjusted Close Data" << endl;

    for (auto const& value:aapl.data) {
        cout << value << endl;
    }
}

void test_eigen_matrix(){
    Eigen::MatrixXd m(2,2);
    m << 3, 2,    2.5, 1;

    cout << m << endl;

    cout << "Determinant: " << m.determinant() << endl;
    cout << "Inverse: " << endl << m.inverse() << endl;
}

void test_stocks(){
    long start = 1262304000, end = 1514764800;

    YFData yf;
    Stock aapl = yf.get_stock("AAPL", start, end);
    Stock goog = yf.get_stock("GOOG", start, end);
    Stock msft = yf.get_stock("MSFT", start, end);

    vector<Stock> stocks({aapl, goog, msft});

    for(unsigned int j= 0; j < 5; ++j){
        for(const auto stock:stocks){
            cout << stock.data[j] << "\t";
        }
        cout << endl;
    }

    cout << "AAPL Ret: " << aapl.get_expected_return() << endl;
    cout << "GOOG Ret: " << goog.get_expected_return() << endl;
    cout << "MSFT Ret: " << msft.get_expected_return() << endl;

    for (auto stock:stocks) {
        cout << stock.get_ticker() << " Var: " << stock.get_variance() << endl;
    }

}

void test_portfolio(){
    long start = 1262304000, end = 1514764800;

    YFData yf;
    Stock aapl = yf.get_stock("AAPL", start, end);
    Stock goog = yf.get_stock("GOOG", start, end);
    Stock msft = yf.get_stock("MSFT", start, end);

    vector<Stock> stocks = {aapl, goog, msft};

    cout << "Making portfolio..." << endl;

    Portfolio port(stocks);

    Eigen::MatrixXd Sigma(stocks.size(), stocks.size());

    for (unsigned int i = 0; i < stocks.size(); ++i) {
        cout << endl;
        for (unsigned int j = 0; j < stocks.size(); ++j) {
            Sigma(i, j) = port.get_covar_matrix(i, j);
            cout << port.get_covar_matrix(i, j) << "\t";
        }
    }

    cout << endl << endl << Sigma << endl;

    cout << Sigma.inverse() << endl;
}

void test_frontier(){
    long start = 1262304000, end = 1514764800;

    YFData yf;
    Stock aapl = yf.get_stock("AAPL", start, end);
    Stock goog = yf.get_stock("GOOG", start, end);
    Stock msft = yf.get_stock("MSFT", start, end);

    vector<Stock> stocks = {aapl, goog, msft};
    vector<double> weights = {0.0, 0.0, 1.0};

    cout << "Making portfolio..." << endl;

    Portfolio port(stocks, weights);

    cout << "Initializing Frontier" << endl;

    try{
        EfficientFrontier frontier(port);

        cout << "Building Frontier" << endl;

        frontier.build_frontier(10000);

        cout << "Calculating optimal portfolios" << endl;

        frontier.build_optimal_portfolios();

        cout << "Max Sharpe Portfolio:\n" << frontier.get_max_sharpe() << endl;
        cout << "Min Variance Portfolio:\n" << frontier.get_min_vol() << endl;

        cout << "Writing Data" << endl;
        frontier.write_frontier_portfolios();

        cout << "Plotting" << endl;
        frontier.plot_frontier_portfolios();

    } catch (exception &e) {
        cerr << e.what() << endl;
    }
}

void user_interface(){
    string ticker;
    vector<string> tickers = {};

    cout << "Welcome to the Efficient Frontier!" << endl << endl;
    cout << "Enter the tickers of the stocks you would like in your portfolio. When you are finished, enter 'done'." << endl;

    cout << "> ";

    while(cin >> ticker){
        if(ticker == "done"){
            break;
        }

        tickers.push_back(ticker);

        cout << "> ";
    }

    vector<double> allocations = {};
    double allocation, total = 0;

    for (unsigned int i = 0; i < tickers.size(); ++i) {
        while(true){
            cout << "Amount allocated to " << tickers[i] << " = $";
            if(cin >> allocation) {
                allocations.push_back(allocation);
                total += allocation;
                break;
            } else {
                cout << "Error: Please enter a $ Dollar amount" << endl;
            }
        }
    }

    cout << "Compiling your portfolio..." << endl;

    YFData yf;
    vector<Stock> stocks = {};
    vector<double> weights = {};

    long start = 1262304000, end = 1514764800;

    for (unsigned int j = 0; j < tickers.size(); ++j) {
        Stock tmp = yf.get_stock(tickers[j], start, end);
        stocks.push_back(tmp);
        weights.push_back(allocations[j]/total);
    }

    Portfolio port(stocks, weights);

    cout << "Your Portfolio: " << endl << port << endl;

    long int n = 20000;

    cout << "Building Efficient Frontier..." << endl;

    EfficientFrontier ef(port);

    ef.build_frontier(n);
    ef.build_optimal_portfolios();
    ef.write_frontier_portfolios();
    ef.plot_frontier_portfolios();

    cout << "Results:" << endl <<
        "Your Max Sharpe Portfolio:" << endl << ef.get_max_sharpe() << endl <<
        "Your Min Variance Portfolio:" << endl << ef.get_min_vol() << endl <<
        "Your Portfolio:" << endl << port << endl;
}

int main()
{
    try{
        user_interface();
    } catch (exception &e){
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
