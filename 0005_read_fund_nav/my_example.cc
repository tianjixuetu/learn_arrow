// (文档部分: 包含)
#include <arrow/api.h>
#include <arrow/io/api.h>
#include "arrow/csv/api.h"
#include <iostream>

arrow::Status RunMain(){
    // 首先，我们需要设置一个可读文件对象，它允许我们将读取器指向磁盘上的正确数据。我们将重复使用这个对象，并将其重新绑定到多个文件中。
    std::shared_ptr<arrow::io::ReadableFile> infile;
    // 绑定输入文件到 "test_in.csv"
    ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open("/home/yun/Documents/fund_nav.csv"));
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
    std::cout << "Table Metadata:" << std::endl;
    std::cout << "Number of columns: " << csv_table->num_columns() << std::endl;
    std::cout << "Number of rows: " << csv_table->num_rows() << std::endl;
    std::cout << "Schema: " << csv_table->schema()->ToString() << std::endl;

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
    arrow::Status status = arrow::PrettyPrint(*csv_table, 2, &std::cout);
    if (!status.ok()) {
      // 处理错误，例如打印错误信息
      std::cerr << "Error: " << status.ToString() << std::endl;
    }
    
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
// (文档部分: 主函数)


