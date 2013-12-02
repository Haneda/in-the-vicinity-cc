#pragma once

// Other
#include "v8.h"

// App
#include "point.h"

class Palette {
 public:
  static const int MAX_SIZE = 32;
  Palette() {}
  int array_[MAX_SIZE];
  Point point_;
  Point array[MAX_SIZE];
};

using namespace v8;

class V8Palette {
public:
  // Args:
  //   ������ ������ ����� � Isolate and Context.
  //   ��� ���-���� �������� ������ ���� ����.
  V8Palette(
      Isolate* isolate,
      Persistent<Context>* context);

  static void GetPointValue(
      Local<String> name,
      const PropertyCallbackInfo<Value>& info);

  static void V8Palette::GetIntArrayValue(
      Local<String> name,
      const PropertyCallbackInfo<Value>& info);

  // ������ � �������� ��������
  static void ArrayIndexGetter(
      uint32_t index,
      const PropertyCallbackInfo<Value>& info);

  static void ArrayIndexSetter(
      uint32_t index,
      Local<Value> value,
      const PropertyCallbackInfo<Value>& info);

  v8::Handle<v8::ObjectTemplate> MakeBlueprint(
      v8::Isolate* isolate, 
      v8::Persistent<v8::Context>* context);

  Handle<Object> ForgePalette(
      Palette* palette,
      Isolate* isolate, 
      Persistent<Context>* context,
      Persistent<ObjectTemplate>* blueprint);

public:
  // ��� ��� ����� ��� ������ � ������.
  static v8::Persistent<v8::ObjectTemplate> point_blueprint_;

  // ��� ��� ����� ��� ������ � �������� �����.
  static v8::Persistent<v8::ObjectTemplate> point_array_blueprint_;
};
