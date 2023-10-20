#include <string>

constexpr int APPROX_BDAYS_PER_MONTH = 21;
constexpr int APPROX_BDAYS_PER_YEAR = 252;

constexpr int MONTHS_PER_YEAR = 12;
constexpr int WEEKS_PER_YEAR = 52;
constexpr int QTRS_PER_YEAR = 4;

std::string DAILY = "daily";
std::string WEEKLY = "weekly";
std::string MONTHLY = "monthly";
std::string QUARTERLY = "quarterly";
std::string YEARLY = "yearly";


namespace AnnualizationFactors{
    int DAILY = APPROX_BDAYS_PER_YEAR;
    int WEEKLY = WEEKS_PER_YEAR;
    int MONTHLY = MONTHS_PER_YEAR;
    int QUARTERLY= QTRS_PER_YEAR;
    int YEARLY = 1;
}








