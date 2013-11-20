//#include "config.h"
#include "v8/src/scripts/virtual_machine.h"

namespace scenarios {

void ScriptsV8Impl::runScript(std::string file) {
	
	Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

	Handle<Context> context = Context::New(isolate);
	Context::Scope context_scope(context);

  //Handle<v8::String> source = ReadFile(file);

	Local<Integer> value = Integer::New(1);
	int out_value = value->ToObject()->Int32Value();
	cout << "value : " << out_value << endl;

}

dblite* dblite::unwrap_dblite(Handle<Object> obj) {

	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<dblite*>(ptr);
}

Handle<v8::Value> dblite::SomeProperty(Local<v8::String> name, const const PropertyCallbackInfo<void>& info) {
// info.Holder() ��������� �� ������ V8 (����� Handle<Object>)
// name ��� ��� ��������, � �������� ����������
	return Integer::New(unwrap_dblite(info.Holder())->error_code());
}

Handle<Value> dblite::Open(const FunctionCallbackInfo<Value>& args) {
//v8::Handle<v8::Value> dblite::Open(v8::FunctionCallback args) {
	
	if (args.Length() < 1) return Undefined(); // ���� ����������? �����!
  dblite* db = unwrap_dblite(args.This()); // �������� ��������� �� dblite
	std::string sql = to_string(args[0]); // �������� ������ � C++, �������� to_string ��������� � ������ ����� ���� ������
	return v8::Boolean::New(db->open(sql.data())); // �������� open() � ���������� ���������

	/*HandleScope scope(args.GetIsolate());
  Handle<Value> arg = args[0];
  String::Utf8Value value(arg);
  HttpRequestProcessor::Log(*value);
	*/
}  

Handle<ObjectTemplate> dblite::CreateDbLiteTemplate() {
		Isolate* isolate = Isolate::GetCurrent();
		HandleScope handle_scope(isolate);
		//HandleScope handle_scope;
    Local<ObjectTemplate> result = ObjectTemplate::New(); // ������� ����� ��������
    result->SetInternalFieldCount(1); // ������� � ��� ���� ���� ��� �������� ������� (��� V8) ������ (void*)
            // � ����� ������ � ������� ������ ����� ��������� dblite*

// ����������� �������� ������� (�� �������) - ����� ��� ������ ������?
//		result->SetAccessor(v8::String::NewSymbol("errorCode"), ErrorCode); // �������� ��� ������

// ����������� ������� - ������ �������
		result->Set(v8::String::NewSymbol("open"), FunctionTemplate::New(&::scenarios::dblite::Open));    
  //  result->Set(String::NewSymbol("close"), FunctionTemplate::New(Close));    
   // result->Set(String::NewSymbol("execute"), FunctionTemplate::New(Execute));    

	//Handle<ObjectTemplate> global_templ = ObjectTemplate::New();
  //global_templ->SetAccessor(String::New("x"), XGetter, XSetter);
	//global_templ->Set(v8::String::New("open"), FunctionTemplate::New(Open));	
	
  //Handle<Context> context = Context::New(isolate, NULL, global_templ);

  //Context::Scope context_scope(context);


// ���������� ��������� ����� ������ �������, ������� ��������� ��� ����� � ���������� HandleScope � �� ���� ��� 
// ������������ ��� ����������� "������" HandleScope - handle_scope
    return handle_scope.Close(result);
}


std::string dblite::to_string(Local<Value> value) {
	
	v8::String::AsciiValue data(value);
    const char* p = *data;
		if (p == 0) return std::string();
		return std::string(p);
}




}
