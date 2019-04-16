//
// Created by J-C Borman on 4/7/2019.
//

#include "YFData.h"
#include <curl/curl.h>
#include "std_lib_facilities.h"

bool debug = false;

// Helpers ------------------------------------------------------------------------------------------------------------

size_t write_html_data(void *ptr, size_t size, size_t nmemb, void *str){
    string* s = static_cast<string*>(str);
    copy((char*)ptr, (char*) ptr + (size + nmemb), back_inserter(*s));
    return size * nmemb;
}

// Constructors -------------------------------------------------------------------------------------------------------

YFData::YFData(string ticker, long start_data, long end_date) {
    // TODO - Implement the constructor
}

YFData::YFData() {}

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

    string result, extract, crumb, cookie;
    string dummy_url = "https://finance.yahoo.com/quote/AAPL";

    smatch match;
    regex e1("(CrumbStore\\\"\\:\\{\\\".*?\\\"\\})"), e2("(\\:\\\".*?\\\"){1}");

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(curl) {

        // Set URL to access
        curl_easy_setopt(curl, CURLOPT_URL, dummy_url.c_str());
        // Follow redirect, if exists
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Write html data with function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_html_data);
        // Write html data to result string
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        /* enable the cookie engine with a non-existing file */
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookiefile.txt");

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        if(debug) cout << "Got: " << endl << result.c_str() << endl;

        /* extract all known cookies */
        struct curl_slist *cookies = NULL;
        res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

        if(!res && cookies) {

            if(debug) cout << " Got cookies" << endl;

            /* a linked list of cookies in cookie file format */
            struct curl_slist *each = cookies;

            string tmp(each->data);
            cookie = tmp;

            size_t found = cookie.find("B\t");
            if(found != string::npos){
                cookie = cookie.substr(found+2, cookie.size() - 1);
                if(debug) cout << "Final cookie: " << cookie << endl;
            } else {
                error("Could not extract cookie");
            }

            /* we must free these cookies when we're done */
            curl_slist_free_all(cookies);
        } else {
            error("Could not get cookies from request");
        }

        // Process html for CrumbStore location
        if(regex_search(result, match, e1)){
            if(debug) cout << "Matched: " << match[0] << endl;
            crumb = match[0];
        } else {
            error("Failed to find CrumbStore location");
        }

        // Process CrumbStore for Crumb
        if(regex_search(crumb, match, e2)){
            if(debug) cout << "Matched: " << match[0] << endl;
            crumb = match[0];
            crumb = crumb.substr(2, crumb.size() - 3);
        } else {
            error("Failed to extract crumb from CrumbStore location");
        }

        if(debug) cout << "Final crumb: " << crumb << endl;

        /* always cleanup */
        curl_easy_cleanup(curl);
    } else {
        error("Failed to create CURL object");
    }

    // Return the crumb
    return vector<string> {crumb, cookie};

}