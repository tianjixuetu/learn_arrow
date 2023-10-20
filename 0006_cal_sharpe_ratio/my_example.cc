// (文档部分: 包含)
#include <arrow/api.h>
#include <arrow/io/api.h>
#include "arrow/csv/api.h"
#include <arrow/compute/api.h>
#include <iostream>
#include <chrono>
//#include "../empyrical/empyrical.h"

arrow::Status RunMain(){
    
    // 首先，我们需要设置一个可读文件对象，它允许我们将读取器指向磁盘上的正确数据。我们将重复使用这个对象，并将其重新绑定到多个文件中。
    std::shared_ptr<arrow::io::ReadableFile> infile;
    // 绑定输入文件到 "test_in.csv"
    ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open("./fund_nav.csv"));
    // (文档部分：CSV 表格声明)
    std::shared_ptr<arrow::Table> csv_table;
    // CSV 读取器有多个对象，用于不同选项。现在，我们将使用默认值。
    ARROW_ASSIGN_OR_RAISE(
        auto csv_reader,
        arrow::csv::TableReader::Make(
            arrow::io::default_io_context(), infile, arrow::csv::ReadOptions::Defaults(),
            arrow::csv::ParseOptions::Defaults(), arrow::csv::ConvertOptions::Defaults()));
    // 读取表格。
    ARROW_ASSIGN_OR_RAISE(csv_table, csv_reader->Read());

    // 输出显示Table的元数据信息
    // std::cout << "Table Metadata:" << std::endl;
    // std::cout << "Number of columns: " << csv_table->num_columns() << std::endl;
    // std::cout << "Number of rows: " << csv_table->num_rows() << std::endl;
    // std::cout << "Schema: " << csv_table->schema()->ToString() << std::endl;

    // 输出显示Table的数据
    // for (int i = 0; i < csv_table->num_columns(); ++i) {
    //   std::shared_ptr<arrow::Array> column = csv_table->column(i);
    //   std::cout << "Column " << i << ": " << column->ToString() << std::endl;
    // }

    // 1. 显示table信息到std::cout的方法
    // std::shared_ptr<arrow::RecordBatch> record_batch;
    // arrow::Result<std::shared_ptr<arrow::RecordBatch>> result = csv_table->CombineChunksToBatch(); // 执行某个操作，返回Result
    // if (result.ok()) {
    //   record_batch = result.ValueOrDie();
    //   // 在这里使用 record_batch
    // } else {
    //   // 处理错误
    //   std::cerr << "Error: " << result.status().ToString() << std::endl;
    // }
    // //arrow::PrettyPrint(*record_batch, 2, &std::cout);
    // arrow::Status status = arrow::PrettyPrint(*record_batch, 2, &std::cout);
    // if (!status.ok()) {
    //   // 处理错误，例如打印错误信息
    //   std::cerr << "Error: " << status.ToString() << std::endl;
    // }
    // 2. 显示table信息到std::cout的方法
    // std::cout << csv_table->ToString() << std::endl;
    // 3. 显示table信息到std::cout的方法
    // arrow::Status status = arrow::PrettyPrint(*csv_table, 2, &std::cout);
    // if (!status.ok()) {
    //   // 处理错误，例如打印错误信息
    //   std::cerr << "Error: " << status.ToString() << std::endl;
    // }
    // 开始计算夏普率
    // std::cout << "一年的交易日有" << AnnualizationFactors::DAILY << "天" << std::endl;
    // std::cout << DAILY << std::endl;

    // 计算收益率
    auto start_time = std::chrono::high_resolution_clock::now();
    arrow::Datum fund_returns;
    arrow::Datum fund_diff;
    std::shared_ptr<arrow::ChunkedArray> cum_nav = csv_table->GetColumnByName("复权净值");
    std::shared_ptr<arrow::ChunkedArray> now_cum_nav = cum_nav->Slice(1,cum_nav->length()-1);
    std::shared_ptr<arrow::ChunkedArray> pre_cum_nav = cum_nav->Slice(0,cum_nav->length()-1);
    ARROW_ASSIGN_OR_RAISE(fund_diff, arrow::compute::CallFunction(
                                          "subtract", {now_cum_nav,pre_cum_nav}));
    ARROW_ASSIGN_OR_RAISE(fund_returns, arrow::compute::CallFunction(
                                          "divide", {fund_diff,pre_cum_nav}));
    // // 获取结果数组
    // std::cout << "Datum kind: " << fund_returns.ToString()
    //           << " content type: " << fund_returns.type()->ToString() << std::endl;

    // // std::cout << fund_returns.scalar_as<arrow::DoubleScalar>().value << std::endl;
    // std::cout << fund_returns.chunked_array()->ToString() << std::endl;
    // 计算夏普率
    arrow::Datum avg_return;
    arrow::Datum avg_std;
    arrow::Datum daily_sharpe_ratio;
    arrow::Datum sharpe_ratio;
    arrow::Datum sqrt_year;
    // 创建 Arrow Double 标量
    double days_of_year_double = 252.0;
    std::shared_ptr<arrow::Scalar> days_of_year = arrow::MakeScalar(days_of_year_double);
    ARROW_ASSIGN_OR_RAISE(sqrt_year, arrow::compute::CallFunction(
                                          "sqrt", {days_of_year}));
    ARROW_ASSIGN_OR_RAISE(avg_return, arrow::compute::CallFunction(
                                          "mean", {fund_returns}));
    arrow::compute::VarianceOptions variance_options;
    variance_options.ddof = 1;
    ARROW_ASSIGN_OR_RAISE(avg_std, arrow::compute::CallFunction(
                                          "stddev", {fund_returns},&variance_options));
    ARROW_ASSIGN_OR_RAISE(daily_sharpe_ratio, arrow::compute::CallFunction(
                                          "divide", {avg_return,avg_std}));
    ARROW_ASSIGN_OR_RAISE(sharpe_ratio, arrow::compute::CallFunction(
                                          "multiply", {daily_sharpe_ratio,sqrt_year}));
    
    std::cout << "计算得到的夏普率为 : " << sharpe_ratio.scalar_as<arrow::DoubleScalar>().value << std::endl;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "c++读取数据,然后计算夏普率一共耗费时间为: " << duration.count()/1000.0 << " ms" << std::endl;

    return arrow::Status::OK();
  }

// (文档部分: 主函数)
int main() {
  arrow::Status st = RunMain();
  if (!st.ok()) {
    std::cerr << st << std::endl;
    return 1;
  }
  
  return 0;
}



