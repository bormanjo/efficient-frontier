//
// Created by J-C Borman on 4/7/2019.
//

#include "YFData.h"
#include <curl/curl.h>
#include <stdio.h>
#include "std_lib_facilities.h"

bool debug = false;

// Helpers ------------------------------------------------------------------------------------------------------------

size_t write_html_data(void *ptr, size_t size, size_t nmemb, void *str){
    auto* s = static_cast<string*>(str);
    copy((char*)ptr, (char*) ptr + (size + nmemb), back_inserter(*s));
    return size * nmemb;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t nline = fwrite(ptr, size, nmemb, stream);
    return nline;
}

// Public -------------------------------------------------------------------------------------------------------------

Stock YFData::get_stock(string symbol, long start_date, long end_date) {
    /*
     * Retrieve the Stock's data and return a stock object
     */

    // Get metadata and store the cookie data in the file
    string cookie_fname = "cookie.txt", interval = "1d";
    vector<string> meta = this->get_metadata(symbol, cookie_fname);

    if (debug) cout << "Crumb: " << meta[0] << endl << "Cookie: " << meta[1] << endl;

    // Create the URL
    string url = this->get_url(symbol, start_date, end_date, interval, meta[0]);

    if (debug) cout << "URL: " << url << endl;

    // Pull the data and store to /data/ folder
    this->pull_data(url, cookie_fname, symbol);

    // Set the data to OHLCV
    this->set_ohlcv();

    vector<double> adj_close = this->get_adjusted_close();

    Stock stock(symbol, adj_close);

    return stock;
}

// Private ------------------------------------------------------------------------------------------------------------

void YFData::pull_data(string &url, string &cookie_fname, string &symbol) {
    /*
     * Downloads the data located at the corresponding url and stores in /data/
     *
     * Uses curl and the name of a file containing the YahooFinance cookie meta data to download a .CSV of stock data
    */

    vector<double> out = {0.10};

    const string fname = "../../data/" + symbol + ".csv";

    CURL *curl;
    CURLcode res;
    FILE * fp;

    curl = curl_easy_init();

    if(curl){
        // Set the cookie file
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookie_fname.c_str());
        // Set URL to access
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Follow redirect, if exists
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // GET just the file download, not the html body
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Set function to write data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        fp = fopen(fname.c_str(), "wb");

        if(fp){
            // Write data to file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            // Perform GET request and write to file
            res = curl_easy_perform(curl);

            // Close the file
            fclose(fp);
        } else {
            error("Could not write to data file");
        }

        // Tidy up curl
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    } else {
        error("Could not init cURL");
    }
}

vector<string> YFData::get_metadata(string &symbol, string &cookie_fname) {

    string result, extract, crumb, cookie;
    string dummy_url = "https://finance.yahoo.com/quote/" + symbol;

    smatch match;
    regex e1("(CrumbStore\\\"\\:\\{\\\".*?\\\"\\})"), e2("(\\:\\\".*?\\\"){1}");

    CURL *curl;
    CURLcode res;

    char nline[256];

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
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookie_fname.c_str());

        curl_easy_setopt(curl, CURLOPT_COOKIELIST, "ALL");


        snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s",
                 ".example.com", "TRUE", "/", "FALSE",
                 (unsigned long)time(NULL) + 31337UL,
                 "PREF", "sample cookie description");
        // Set file as cookie jar
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookie_fname.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        if(debug) cout << "Got: " << endl << result.c_str() << endl;

        /* extract all known cookies */
        struct curl_slist *cookies = NULL;
        res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

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

string YFData::get_url(string &symbol, long d1, long d2, string &intrvl, string &crumb) {
    string url = "https://query1.finance.yahoo.com/v7/finance/download/" + symbol +
            "?period1=" + to_string(d1) +
            "&period2=" + to_string(d2) +
            "&interval=" + intrvl +
            "&events=history&crumb=" + crumb;

    return  url;
}

void YFData::set_ohlcv() {
    /*
     * Reads and sets the matrix of ohlcv data and vector of dates
     */

    string line;

    string fname = "../../data/YFData.csv";

    ifstream file(fname);

    vector<double> open, high, low, close, adj_close, volume;

    bool first_row = true;

    while(getline(file, line)){
        vector<double> row;
        stringstream iss(line);
        int col_num = 0;
        string date, val;

        if(first_row){
           first_row = false;
        } else {
            while(col_num < 7 && getline(iss, val, ',')){

                switch(col_num){
                    case 0: this->dates.push_back(val); break;
                    case 1: open.push_back(stod(val)); break;
                    case 2: high.push_back(stod(val)); break;
                    case 3: low.push_back(stod(val)); break;
                    case 4: close.push_back(stod(val)); break;
                    case 5: adj_close.push_back(stod(val)); break;
                    case 6: volume.push_back(stod(val)); break;
                }
                col_num++;
            }

            this->ohlcv.push_back(row);
        }
    }

    this->ohlcv = {open, high, low, close, adj_close, volume};
}

vector<double> YFData::get_ohlcv_col(unsigned int col){
    /*
     * Retrieves the vector
     */
    if(col > 5){
        error("OHLCV col index out of bounds");
    }

    return this->ohlcv[col];
}

vector<double> YFData::get_adjusted_close(){
    /*
     * Get's the stock's adjusted close data from the downloaded file
     */

    return this->get_ohlcv_col(4);

}