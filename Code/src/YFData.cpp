//
// Created by J-C Borman on 4/7/2019.
//

#include "YFData.h"


// Constructors -------------------------------------------------------------------------------------------------------

YFData::YFData(string ticker, long start_data, long end_date) {
    // TODO - Implement the constructor
}

// Public -------------------------------------------------------------------------------------------------------------

Asset YFData::get_stock() {
    // TODO - Get stock data, construct and return the Asset object
}

// Private ------------------------------------------------------------------------------------------------------------

vector<double> YFData::get_adj_close_data() {
    // TODO - Read/Parse adjusted close data from YahooFinance

    /* Sample query:
     * https://query1.finance.yahoo.com/v7/finance/download/AAPL?period1=1552005374&period2=1554680174&interval=1d&events=history&crumb=C5H7cEgTzTb
     *
     * Dates are provided as integers
     *
     * TODO - Figure out how to get the CRUMB (last URL parameter)
    */
}


vector<string> YFData::get_metadata() {
    string dummy_link = "https://finance.yahoo.com/quote/AAPL";


}