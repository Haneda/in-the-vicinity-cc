//#include "config.h"
#include "virtual_machine.h"

namespace scenarios {

/*
void ScriptsV8Impl::runScript(std::string file) {
	
	Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

	Handle<Context> context = Context::New(isolate);
	Context::Scope context_scope(context);

  //Handle<v8::String> source = ReadFile(file);

	Local<Integer> value = Integer::New(1);
	int out_value = value->ToObject()->Int32Value();
	cout << "value : " << out_value << endl;

}*/

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

void dblite::Open(const FunctionCallbackInfo<Value>& info) {
  dblite* db = unwrap_dblite(info.This()); // �������� ��������� �� dblite
	std::string sql = to_string(info[0]); // �������� ������ � C++, �������� to_string ��������� � ������ ����� ���� ������
	info.GetReturnValue().Set(v8::BooleanObject::New(db->open(sql.data())));	
}  

v8::Handle<v8::FunctionTemplate> dblite::CreateDbLiteTemplate() {	

	v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
	v8::Handle<v8::FunctionTemplate> templ = v8::FunctionTemplate::New();
	v8::Handle<v8::ObjectTemplate> instTempl = templ->InstanceTemplate();
	instTempl->SetInternalFieldCount(1);

	v8::Handle<v8::ObjectTemplate> proto = templ->PrototypeTemplate();
	proto->SetAccessorProperty(v8::String::NewSymbol("open"), v8::FunctionTemplate::New(
			static_cast<v8::FunctionCallback>(&::scenarios::dblite::Open)));

// ���������� ��������� ����� ������ �������, ������� ��������� ��� ����� � ���������� HandleScope � �� ���� ��� 
// ������������ ��� ����������� "������" HandleScope - handle_scope
 return handle_scope.Close(templ);
}

std::string dblite::to_string(Local<Value> value) {
	
	v8::String::AsciiValue data(value);
    const char* p = *data;
		if (p == 0) return std::string();
		return std::string(p);
}
} // namespace scenarios
