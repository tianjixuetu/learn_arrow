// (文档部分：包含)
#include <arrow/api.h>
#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>

#include <iostream>
// (文档部分：包含)

// (文档部分：生成初始文件)
arrow::Status GenInitialFile() {
  // 创建一些8位整数数组和一个16位整数数组，就像基本的 Arrow 示例一样。
  arrow::Int8Builder int8builder;
  int8_t days_raw[5] = {1, 12, 17, 23, 28};
  ARROW_RETURN_NOT_OK(int8builder.AppendValues(days_raw, 5));
  std::shared_ptr<arrow::Array> days;
  ARROW_ASSIGN_OR_RAISE(days, int8builder.Finish());

  int8_t months_raw[5] = {1, 3, 5, 7, 1};
  ARROW_RETURN_NOT_OK(int8builder.AppendValues(months_raw, 5));
  std::shared_ptr<arrow::Array> months;
  ARROW_ASSIGN_OR_RAISE(months, int8builder.Finish());

  arrow::Int16Builder int16builder;
  int16_t years_raw[5] = {1990, 2000, 1995, 2000, 1995};
  ARROW_RETURN_NOT_OK(int16builder.AppendValues(years_raw, 5));
  std::shared_ptr<arrow::Array> years;
  ARROW_ASSIGN_OR_RAISE(years, int16builder.Finish());

  // 获取包含我们的数组的向量
  std::vector<std::shared_ptr<arrow::Array>> columns = {days, months, years};

  // 创建一个用于初始化表格的模式
  std::shared_ptr<arrow::Field> field_day, field_month, field_year;
  std::shared_ptr<arrow::Schema> schema;

  field_day = arrow::field("Day", arrow::int8());
  field_month = arrow::field("Month", arrow::int8());
  field_year = arrow::field("Year", arrow::int16());

  schema = arrow::schema({field_day, field_month, field_year});
  // 使用模式和数据创建表格
  std::shared_ptr<arrow::Table> table;
  table = arrow::Table::Make(schema, columns);

  // 为示例创建 IPC、CSV 和 Parquet 格式的测试文件。
  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_in.arrow"));
  ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::ipc::RecordBatchWriter> ipc_writer,
                        arrow::ipc::MakeFileWriter(outfile, schema));
  ARROW_RETURN_NOT_OK(ipc_writer->WriteTable(*table));
  ARROW_RETURN_NOT_OK(ipc_writer->Close());

  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_in.csv"));
  ARROW_ASSIGN_OR_RAISE(auto csv_writer,
                        arrow::csv::MakeCSVWriter(outfile, table->schema()));
  ARROW_RETURN_NOT_OK(csv_writer->WriteTable(*table));
  ARROW_RETURN_NOT_OK(csv_writer->Close());

  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_in.parquet"));
  PARQUET_THROW_NOT_OK(
      parquet::arrow::WriteTable(*table, arrow::default_memory_pool(), outfile, 5));

  return arrow::Status::OK();
}
// (文档部分：生成初始文件)

// (文档部分：运行主函数)
arrow::Status RunMain() {
  // (文档部分：运行主函数)
  // (文档部分：生成文件)
  // 使用辅助函数生成各种格式的初始文件，不用担心，我们也会在本示例中写入一个表格。
  ARROW_RETURN_NOT_OK(GenInitialFile());
  // (文档部分：生成文件)

  // (文档部分：可读文件定义)
  // 首先，我们需要设置一个可读文件对象，它允许我们将读取器指向磁盘上的正确数据。我们将重复使用这个对象，并将其重新绑定到多个文件中。
  std::shared_ptr<arrow::io::ReadableFile> infile;
  // (文档部分：可读文件定义)
  // (文档部分：Arrow 可读文件打开)
  // 打开 "test_in.arrow" 文件并将其分配给文件指针
  ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open(
                                    "test_in.arrow", arrow::default_memory_pool()));
  // (文档部分：Arrow 可读文件打开)
  // (文档部分：Arrow 读取文件打开)
  // 使用库的 IPC 特性打开文件，得到一个读取器对象。
  ARROW_ASSIGN_OR_RAISE(auto ipc_reader, arrow::ipc::RecordBatchFileReader::Open(infile));
  // (文档部分：Arrow 读取文件打开)
  // (文档部分：Arrow 读取)
  // 使用读取器，我们可以读取记录批次。请注意，这是特定于 IPC 的；对于其他格式，我们专注于表格，但在这里使用了记录批次。
  std::shared_ptr<arrow::RecordBatch> rbatch;
  ARROW_ASSIGN_OR_RAISE(rbatch, ipc_reader->ReadRecordBatch(0));
  // (文档部分：Arrow 读取)

  // (文档部分：Arrow 写入文件打开)
  // 就像输入一样，我们获得一个输出文件对象。
  std::shared_ptr<arrow::io::FileOutputStream> outfile;
  // 绑定到 "test_out.arrow"
  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_out.arrow"));
  // (文档部分：Arrow 写入文件打开)
  // (文档部分：Arrow 写入器)
  // 使用输出文件和模式设置写入器。我们在这里定义了一切，准备就绪。
  ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::ipc::RecordBatchWriter> ipc_writer,
                        arrow::ipc::MakeFileWriter(outfile, rbatch->schema()));
  // (文档部分：Arrow 写入器)
  // (文档部分：Arrow 写入)
  // 写入记录批次。
  ARROW_RETURN_NOT_OK(ipc_writer->WriteRecordBatch(*rbatch));
  // (文档部分：Arrow 写入)
  // (文档部分：Arrow 关闭)
  // 对于 IPC，写入器需要显式关闭。
  ARROW_RETURN_NOT_OK(ipc_writer->Close());
  // (文档部分：Arrow 关闭)

  // (文档部分：CSV 读取文件打开)
  // 绑定输入文件到 "test_in.csv"
  ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open("test_in.csv"));
  // (文档部分：CSV 读取文件打开)
  // (文档部分：CSV 表格声明)
  std::shared_ptr<arrow::Table> csv_table;
  // (文档部分：CSV 表格声明)
  // (文档部分：CSV 读取器创建)
  // CSV 读取器有多个对象，用于不同选项。现在，我们将使用默认值。
  ARROW_ASSIGN_OR_RAISE(
      auto csv_reader,
      arrow::csv::TableReader::Make(
          arrow::io::default_io_context(), infile, arrow::csv::ReadOptions::Defaults(),
          arrow::csv::ParseOptions::Defaults(), arrow::csv::ConvertOptions::Defaults()));
  // (文档部分：CSV 读取器创建)
  // (文档部分：CSV 读取)
  // 读取表格。
  ARROW_ASSIGN_OR_RAISE(csv_table, csv_reader->Read())
  // (文档部分：CSV 读取)

  // (文档部分：CSV 写入)
  // 绑定输出文件到 "test_out.csv"
  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_out.csv"));
  // CSV 写入器有更简单的默认值，更复杂的使用请参考 API 文档。
  ARROW_ASSIGN_OR_RAISE(auto csv_writer,
                        arrow::csv::MakeCSVWriter(outfile, csv_table->schema()));
  ARROW_RETURN_NOT_OK(csv_writer->WriteTable(*csv_table));
  // 不是必需的，但是一个安全的做法。
  ARROW_RETURN_NOT_OK(csv_writer->Close());
  // (文档部分：CSV 写入)

  // (文档部分：Parquet 读取文件打开)
  // 绑定输入文件到 "test_in.parquet"
  ARROW_ASSIGN_OR_RAISE(infile, arrow::io::ReadableFile::Open("test_in.parquet"));
  // (文档部分：Parquet 读取文件打开)
  // (文档部分：Parquet 文件读取器)
  std::unique_ptr<parquet::arrow::FileReader> reader;
  // (文档部分：Parquet 文件读取器)
  // (文档部分：Parquet 打开文件)
  // 请注意，Parquet 的 OpenFile() 接受引用的读取器，而不是返回一个读取器。
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  // (文档部分：Parquet 打开文件)

  // (文档部分：Parquet 读取)
  std::shared_ptr<arrow::Table> parquet_table;
  // 读取表格。
  PARQUET_THROW_NOT_OK(reader->ReadTable(&parquet_table));
  // (文档部分：Parquet 读取)

  // (文档部分：Parquet 写入)
  // Parquet 写入不需要声明的写入器对象。只需绑定输出文件，然后传入表格、内存池、输出和分块大小，以在磁盘上拆分表格。
  ARROW_ASSIGN_OR_RAISE(outfile, arrow::io::FileOutputStream::Open("test_out.parquet"));
  PARQUET_THROW_NOT_OK(parquet::arrow::WriteTable(
      *parquet_table, arrow::default_memory_pool(), outfile, 5));
  // (文档部分：Parquet 写入)
  // (文档部分：返回)
  return arrow::Status::OK();
}
// (文档部分：返回)

// (文档部分：主函数)
int main() {
  arrow::Status st = RunMain();
  if (!st.ok()) {
    std::cerr << st << std::endl;
    return 1;
  }
  return 0;
}
// (文档部分：主函数)