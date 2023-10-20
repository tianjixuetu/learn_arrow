import pandas as pd 
import empyrical as ep
import time 
a = time.perf_counter()
data = pd.read_csv("./fund_nav.csv")
returns = data['复权净值'].pct_change().dropna()
sharpe_ratio = ep.sharpe_ratio(returns)
# print("mean_return ",returns.mean())
# print("std_deviation ",returns.std())
# print("sqrt_trading_days_per_year ",252**0.5)
print("the result of sharpe ratio : ", sharpe_ratio)
b = time.perf_counter()
print(f"the consume time of python read data and caculate the sharpe_ratio:  {(b-a)*1000.0} ms")