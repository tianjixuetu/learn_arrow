// (文档部分: 包含)
#include <arrow/api.h>
#include <arrow/dataset/api.h>
// 我们在示例设置中使用Parquet头文件，但在使用数据集时它们不是必需的。
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>

#include <iostream>
// (文档部分: 包含)

// (文档部分: 辅助函数)
// 为接下来的示例生成一些数据。
arrow::Result<std::shared_ptr<arrow::Table>> CreateTable() {
  // 这段代码应该看起来很熟悉，与基本的Arrow示例类似，不是本示例的重点。然而，我们需要数据来处理，这就是生成数据的目的！
  auto schema =
      arrow::schema({arrow::field("a", arrow::int64()), arrow::field("b", arrow::int64()),
                     arrow::field("c", arrow::int64())});
  std::shared_ptr<arrow::Array> array_a;
  std::shared_ptr<arrow::Array> array_b;
  std::shared_ptr<arrow::Array> array_c;
  arrow::NumericBuilder<arrow::Int64Type> builder;
  ARROW_RETURN_NOT_OK(builder.AppendValues({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
  ARROW_RETURN_NOT_OK(builder.Finish(&array_a));
  builder.Reset();
  ARROW_RETURN_NOT_OK(builder.AppendValues({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
  ARROW_RETURN_NOT_OK(builder.Finish(&array_b));
  builder.Reset();
  ARROW_RETURN_NOT_OK(builder.AppendValues({1, 2, 1, 2, 1, 2, 1, 2, 1, 2}));
  ARROW_RETURN_NOT_OK(builder.Finish(&array_c));
  return arrow::Table::Make(schema, {array_a, array_b, array_c});
}

// 通过编写两个Parquet文件来设置一个数据集。
arrow::Result<std::string> CreateExampleParquetDataset( const std::shared_ptr<arrow::fs::FileSystem>& filesystem, const std::string& root_path)
   
{
  // 与CreateTable()类似，这是一个实用程序，使我们可以读取的数据集，不必担心，示例中我们还会编写一个数据集。
  auto base_path = root_path + "parquet_dataset";
  ARROW_RETURN_NOT_OK(filesystem->CreateDir(base_path));
  // 创建一个Arrow表
  ARROW_ASSIGN_OR_RAISE(auto table, CreateTable());
  // 将其写入两个Parquet文件
  ARROW_ASSIGN_OR_RAISE(auto output,
                        filesystem->OpenOutputStream(base_path + "/data1.parquet"));
  ARROW_RETURN_NOT_OK(parquet::arrow::WriteTable(
      *table->Slice(0, 5), arrow::default_memory_pool(), output, 2048));
  ARROW_ASSIGN_OR_RAISE(output,
                        filesystem->OpenOutputStream(base_path + "/data2.parquet"));
  ARROW_RETURN_NOT_OK(parquet::arrow::WriteTable(
      *table->Slice(5), arrow::default_memory_pool(), output, 2048));
  return base_path;
}

arrow::Status PrepareEnv() {
  // 通过设置一些快速写入来准备好我们的环境以供读取。
  ARROW_ASSIGN_OR_RAISE(auto src_table, CreateTable())
  std::shared_ptr<arrow::fs::FileSystem> setup_fs;
  // 请注意，这是在构建可执行文件的目录中运行的。
  char setup_path[256];
  char* result = getcwd(setup_path, 256);
  if (result == NULL) {
    return arrow::Status::IOError("获取PWD失败。");
  }

  ARROW_ASSIGN_OR_RAISE(setup_fs, arrow::fs::FileSystemFromUriOrPath(setup_path));
  ARROW_ASSIGN_OR_RAISE(auto dset_path, CreateExampleParquetDataset(setup_fs, ""));

  return arrow::Status::OK();
}
// (文档部分: 辅助函数)

// (文档部分: RunMain)
arrow::Status RunMain() {
  // (文档部分: RunMain)
  // (文档部分: PrepareEnv)
  ARROW_RETURN_NOT_OK(PrepareEnv());
  // (文档部分: PrepareEnv)

  // (文档部分: FileSystem声明)
  // 首先，我们需要一个文件系统对象，它允许我们从给定的路径开始与本地文件系统交互。为了简单起见，这将是当前目录。
  std::shared_ptr<arrow::fs::FileSystem> fs;
  // (文档部分: FileSystem声明)

  // (文档部分: FileSystem初始化)
  // 获取当前工作目录并使用它来创建FileSystem对象。
  char init_path[256];
  char* result = getcwd(init_path, 256);
  if (result == NULL) {
    return arrow::Status::IOError("获取PWD失败。");
  }
  ARROW_ASSIGN_OR_RAISE(fs, arrow::fs::FileSystemFromUriOrPath(init_path));
  // (文档部分: FileSystem初始化)

  // (文档部分: FileSelector声明)
  // 文件选择器允许我们实际遍历多文件数据集。
  arrow::fs::FileSelector selector;
  // (文档部分: FileSelector声明)
  // (文档部分: FileSelector配置)
  selector.base_dir = "parquet_dataset";
  // 如果不知道数据集的嵌套结构，递归是一个安全的选择。
  selector.recursive = true;
  // (文档部分: FileSelector配置)
  // (文档部分: FileSystemFactoryOptions)
  // 创建选项对象允许我们配置数据集读取。
  arrow::dataset::FileSystemFactoryOptions options;
  // 我们将使用Hive风格的分区。我们将让Arrow Datasets推断分区模式。我们不会设置其他选项，使用默认值就足够了。
  options.partitioning = arrow::dataset::HivePartitioning::MakeFactory();
  // (文档部分: FileSystemFactoryOptions)
  // (文档部分: 文件格式设置)
  auto read_format = std::make_shared<arrow::dataset::ParquetFileFormat>();
  // (文档部分: 文件格式设置)
  // (文档部分: FileSystemDatasetFactory创建)
  // 现在，我们获得一个工厂，它允许我们获取数据集，我们还没有数据集！
  ARROW_ASSIGN_OR_RAISE(auto factory, arrow::dataset::FileSystemDatasetFactory::Make(
                                          fs, selector, read_format, options));
  // (文档部分: FileSystemDatasetFactory创建)
  // (文档部分: FileSystemDatasetFactory完成)
  // 现在，我们从工厂构建数据集。
  ARROW_ASSIGN_OR_RAISE(auto read_dataset, factory->Finish());
  // (文档部分: FileSystemDatasetFactory完成)
  // (文档部分: 数据集片段)
  // 打印出数据片段
  ARROW_ASSIGN_OR_RAISE(auto fragments, read_dataset->GetFragments());
  for (const auto& fragment : fragments) {
    std::cout << "找到片段：" << (*fragment)->ToString() << std::endl;
    std::cout << "分区表达式：" << (*fragment)->partition_expression().ToString() << std::endl;
  }
  // (文档部分: 数据集片段)
  // (文档部分: 读取扫描生成器)
  // 将数据集扫描为表格，完成后，您可以对其执行常规表格操作，如计算和打印。不过，现在您也需要在内存中运行。
  ARROW_ASSIGN_OR_RAISE(auto read_scan_builder, read_dataset->NewScan());
  // (文档部分: 读取扫描生成器)
  // (文档部分: 读取扫描器)
  ARROW_ASSIGN_OR_RAISE(auto read_scanner, read_scan_builder->Finish());
  // (文档部分: 读取扫描器)
  // (文档部分: 转为表格)
  ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::Table> table, read_scanner->ToTable());
  std::cout << table->ToString();
  // (文档部分: 转为表格)

  // (文档部分: TableBatchReader)
  // 现在，让我们将表格输出到磁盘作为数据集！
  // 我们从表格中创建一个RecordBatchReader，然后设置扫描器，以便我们可以保存到文件。
  std::shared_ptr<arrow::TableBatchReader> write_dataset =
      std::make_shared<arrow::TableBatchReader>(table);
  // (文档部分: TableBatchReader)
  // (文档部分: 写扫描器)
  auto write_scanner_builder =
      arrow::dataset::ScannerBuilder::FromRecordBatchReader(write_dataset);
  ARROW_ASSIGN_OR_RAISE(auto write_scanner, write_scanner_builder->Finish())
  // (文档部分: 写扫描器)
  // (文档部分: 分区模式)
  // 分区模式确定用于分区的键是哪些字段。
  auto partition_schema = arrow::schema({arrow::field("a", arrow::utf8())});
  // (文档部分: 分区模式)
  // (文档部分: 分区创建)
  // 我们将使用Hive风格的分区，这将创建具有 "key=value" 对的目录。
  auto partitioning =
      std::make_shared<arrow::dataset::HivePartitioning>(partition_schema);
  // (文档部分: 分区创建)
  // (文档部分: 写入格式)
  // 现在，声明我们将写入Parquet文件。
  auto write_format = std::make_shared<arrow::dataset::ParquetFileFormat>();
  // (文档部分: 写入格式)
  // (文档部分: 写入选项)
  // 这一次，我们创建用于写入的选项，但会进行更多配置。
  arrow::dataset::FileSystemDatasetWriteOptions write_options;
  // 默认值开始。
  write_options.file_write_options = write_format->DefaultWriteOptions();
  // (文档部分: 写入选项)
  // (文档部分: 选项文件系统)
  // 使用我们已经有的文件系统。
  write_options.filesystem = fs;
  // (文档部分: 选项文件系统)
  // (文档部分: 选项目标)
  // 写入到当前目录中的 "write_dataset" 文件夹。
  write_options.base_dir = "write_dataset";
  // (文档部分: 选项目标)
  // (文档部分: 选项分区)
  // 使用上面声明的分区。
  write_options.partitioning = partitioning;
  // (文档部分: 选项分区)
  // (文档部分: 选项文件名称模板)
  // 定义构成数据集的文件的名称。
  write_options.basename_template = "part{i}.parquet";
  // (文档部分: 选项文件名称模板)
  // (文档部分: 选项已存在数据行为)
  // 设置行为以覆盖现有数据 - 具体来说，这使得可以多次运行此示例，并允许您的代码覆盖已有的内容。
  write_options.existing_data_behavior =
      arrow::dataset::ExistingDataBehavior::kOverwriteOrIgnore;
  // (文档部分: 选项已存在数据行为)
  // (文档部分: 写入数据集)
  // 将数据写入磁盘！
  ARROW_RETURN_NOT_OK(
      arrow::dataset::FileSystemDataset::Write(write_options, write_scanner));
  // (文档部分: 写入数据集)
  // (文档部分: 返回)
  return arrow::Status::OK();
}
// (文档部分: 返回)
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


