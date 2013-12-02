#include "palette.h"

V8Palette::V8Palette(
    Palette* palette,  
    Isolate* isolate,
    Persistent<Context>* context) : palette_(palette), isolate_(isolate), context_(context) 
  { 
  // ����� ������� Persistent!
  HandleScope handle_scope(isolate_);

  Context::Scope scope(isolate_, *context_);
  if (point_blueprint_.IsEmpty()) {
    V8Point point;
    Handle<ObjectTemplate> raw_template = 
        point.CreateBlueprint(isolate, context_);

    // ���������, �� ������ ����� � ������� ������������
    point_blueprint_.Reset(isolate, raw_template);
  }

  // ������� ������ �� �������.
  Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, point_blueprint_);
  Handle<Object> result = templ->NewInstance();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Handle<External> map_ptr = External::New(&this->palette_->point_);

  // Store the map pointer in the JavaScript wrapper.
  result->SetInternalField(0, map_ptr);

  if (point_field_.IsEmpty()) {
    point_field_.Reset(isolate, result);
  }
}

v8::Handle<v8::ObjectTemplate> V8Palette::MakeBlueprint(
      v8::Isolate* isolate, v8::Persistent<v8::Context>* context) 
    {
    HandleScope handle_scope(isolate);

    Context::Scope scope(isolate, *context);

    Handle<ObjectTemplate> result = ObjectTemplate::New();
    result->SetInternalFieldCount(1);

    // Connect getter/setter
    result->SetAccessor(String::New("point"), GetPointX);

    return handle_scope.Close(result);
  }

Handle<Object> V8Palette::ForgePalette(
      Palette* palette,
      Isolate* isolate, 
      Persistent<Context>* context,
      Persistent<ObjectTemplate>* blueprint) 
    {
    HandleScope handle_scope(isolate);
    Context::Scope scope(isolate, *context);

    if (blueprint->IsEmpty()) {
      Handle<ObjectTemplate> raw_template = 
          this->MakeBlueprint(isolate, context);

      // ���������, �� ������ ����� � ������� ������������
      blueprint->Reset(isolate, raw_template);
    }

    // ����� ����������� �������� ������
    // ������ ����� ������� ������ �������
    // Create an empty map wrapper.
    Handle<ObjectTemplate> templ =
        Local<ObjectTemplate>::New(isolate, *blueprint);
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Handle<External> map_ptr = External::New(palette);

    // Store the map pointer in the JavaScript wrapper.
    result->SetInternalField(0, map_ptr);
    return handle_scope.Close(result);
  }

