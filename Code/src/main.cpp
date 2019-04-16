#include "std_lib_facilities.h"
#include <curl/curl.h>
#include <regex>
#include "YFData.h"

int main()
{
    YFData yf;

    vector<string> meta = yf.get_metadata();

    cout << meta[0] << endl << meta[1] << endl;

    return 0;
}
