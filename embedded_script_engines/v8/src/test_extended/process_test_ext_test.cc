// #in.. config.h
#include "test_extended/process_test_ext.h"

// C++
#include <map>

// Other
#include <gtest/gtest.h>

using std::pair;


TEST(ProcessTestExt, Create) {
  int argc = 2;
  char* argv[] = {"", "..\\scripts\\test_extended.js"};

  v8::V8::InitializeICU();
  map<string, string> options;
  map<string, string> output;

  options.insert(pair<string, string>("key", "value"));

  string file;
  ParseOptions(argc, argv, options, &file);
  EXPECT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();

  // ������ ����� ������� - ��� ��� �� ���� ���� ��� V8
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  EXPECT_NE(true, source.IsEmpty());

  // ������� ����� ��� ������������
  JsHttpRequestProcessorTestExt processor(isolate, source);

  // ������ ����� ������������������
  processor.Initialize(&options, &output);
}