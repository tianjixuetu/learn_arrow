// 包含头文件
#include <arrow/api.h>
#include <arrow/compute/api.h>
#include <iostream>

// 主程序入口
arrow::Status RunMain() {
  // 创建两个包含32位整数的数组
  arrow::Int32Builder int32builder;
  int32_t some_nums_raw[5] = {34, 624, 2223, 5654, 4356};
  ARROW_RETURN_NOT_OK(int32builder.AppendValues(some_nums_raw, 5));
  std::shared_ptr<arrow::Array> some_nums;
  ARROW_ASSIGN_OR_RAISE(some_nums, int32builder.Finish());

  int32_t more_nums_raw[5] = {75342, 23, 64, 17, 736};
  ARROW_RETURN_NOT_OK(int32builder.AppendValues(more_nums_raw, 5));
  std::shared_ptr<arrow::Array> more_nums;
  ARROW_ASSIGN_OR_RAISE(more_nums, int32builder.Finish());

  // 创建一个包含两列的Arrow表格
  std::shared_ptr<arrow::Field> field_a, field_b;
  std::shared_ptr<arrow::Schema> schema;
  field_a = arrow::field("A", arrow::int32());
  field_b = arrow::field("B", arrow::int32());
  schema = arrow::schema({field_a, field_b});
  std::shared_ptr<arrow::Table> table;
  table = arrow::Table::Make(schema, {some_nums, more_nums}, 5);

  // 创建一个用于存储计算结果的Arrow Datum
  arrow::Datum sum;

  // 使用arrow::compute::Sum函数计算列"A"的总和
  ARROW_ASSIGN_OR_RAISE(sum, arrow::compute::Sum({table->GetColumnByName("A")}));

  // 打印计算结果
  std::cout << "Datum kind: " << sum.ToString()
            << " content type: " << sum.type()->ToString() << std::endl;
  std::cout << sum.scalar_as<arrow::Int64Scalar>().value << std::endl;

  // 创建另一个用于存储逐元素相加的计算结果的Arrow Datum
  arrow::Datum element_wise_sum;

  // 使用arrow::compute::CallFunction函数计算列"A"和列"B"的逐元素相加
  ARROW_ASSIGN_OR_RAISE(element_wise_sum, arrow::compute::CallFunction(
                                              "add", {table->GetColumnByName("A"),
                                                      table->GetColumnByName("B")}));

  // 打印计算结果
  std::cout << "Datum kind: " << element_wise_sum.ToString()
            << " content type: " << element_wise_sum.type()->ToString() << std::endl;
  std::cout << element_wise_sum.chunked_array()->ToString() << std::endl;

  // 创建另一个用于存储搜索值的结果的Arrow Datum
  arrow::Datum third_item;

  // 创建搜索选项
  arrow::compute::IndexOptions index_options;

  // 设置要搜索的值
  index_options.value = arrow::MakeScalar(2223);

  // 使用arrow::compute::CallFunction函数搜索值的位置
  ARROW_ASSIGN_OR_RAISE(
      third_item, arrow::compute::CallFunction("index", {table->GetColumnByName("A")},
                                               &index_options));

  // 打印搜索结果
  std::cout << "Datum kind: " << third_item.ToString()
            << " content type: " << third_item.type()->ToString() << std::endl;
  std::cout << third_item.scalar_as<arrow::Int64Scalar>().value << std::endl;

  // 返回OK状态表示程序成功完成
  return arrow::Status::OK();
}

// 主函数
int main() {
  arrow::Status st = RunMain();
  if (!st.ok()) {
    std::cerr << st << std::endl;
    return 1;
  }
  return 0;
}
