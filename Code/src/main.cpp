#include "std_lib_facilities.h"
#include <curl/curl.h>
#include <regex>
#include "YFData.h"

int main()
{
    YFData yf;

    Stock aapl = yf.get_stock("AAPL", 1262304000, 1514764800);

    cout << "AAPL's Adjusted close data" << endl;

    for (auto const& value:aapl.data) {
        cout << value << endl;
    }

    return 0;
}
