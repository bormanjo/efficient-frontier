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
    YFData (string ticker, long start_data, long end_date);

    // Returns a stock object containing the ticker's adjusted close data
    Asset get_stock();

private:
    // Member properties
    string ticker;
    long start_date, end_date;
    vector<double> data;

    // Member functions
    vector<double> get_adj_close_data();
};


#endif //CODE_YFDATA_H
