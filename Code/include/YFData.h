//
// Created by J-C Borman on 4/7/2019.
//

#ifndef CODE_YFDATA_H
#define CODE_YFDATA_H

#include "std_lib_facilities.h"
#include "Asset.h"

class YFData {
    /*
     * Object for pulling YahooFinance data and collecting it into Asset objects
     */
public:
    // Constructor that initializes the YahooFinance Data object
    YFData () = default;

    // Returns a stock object containing the ticker's adjusted close data
    Stock get_stock(string ticker, long start_data, long end_date);

private:

    // Member properties
    vector<vector<double>> ohlcv;
    vector<string> dates;

    // Member functions
    vector<string> get_metadata(string &symbol, string &cookie_fname);

    string get_url(string &symbol, long d1, long d2, string &intrvl, string &crumb);

    void pull_data(string &url, string &cookie_fname, string &ticker);

    void set_ohlcv();

    vector<double> get_ohlcv_col(unsigned int col);

    vector<double> get_adjusted_close();
};


#endif //CODE_YFDATA_H
