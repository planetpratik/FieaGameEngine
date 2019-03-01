#include "pch.h"
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <utility>
#include <json/json.h>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonCppTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState end_mem_state, diff_mem_state;
			_CrtMemCheckpoint(&end_mem_state);
			if (_CrtMemDifference(&diff_mem_state, &s_start_mem_state, &end_mem_state))
			{
				_CrtMemDumpStatistics(&diff_mem_state);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(SimpleDeserialization)
		{
			stringstream input_stream;
			input_stream << R"({ "Health": { "type": "integer", "value": 100 }})";

			Json::Value root;
			input_stream >> root;

			stringstream output_stream;
			outputJsonToStream(output_stream, root);
			Logger::WriteMessage(output_stream.str().c_str());

		}

		void indent(ostream& t_output_stream, int t_indent)
		{
			for (int i = 0; i < t_indent; ++i)
			{
				t_output_stream << ' ';
			}
		}

		void outputJsonToStream(ostream& t_output_stream, const Json::Value& t_val, int t_indent = 0)
		{
			switch (t_val.type())
			{
			case Json::nullValue:
				t_output_stream << "null";
				break;
			case Json::booleanValue:
				t_output_stream << (t_val.asBool() ? "true" : "false");
				break;
			case Json::intValue:
				t_output_stream << t_val.asLargestInt(); 
				break;
			case Json::uintValue:
				t_output_stream << t_val.asLargestUInt();
				break;
			case Json::realValue:
				t_output_stream << t_val.asDouble();
				break;
			case Json::stringValue:
				t_output_stream << '"' << t_val.asString() << '"';
				break;
			case Json::arrayValue:
			{
				Json::ArrayIndex size = t_val.size();
				if (size == 0)
				{
					t_output_stream << "[]";
				}
				else
				{
					t_output_stream << "[\n";
					int new_indent = t_indent + 4;
					for (Json::ArrayIndex i = 0; i < size; ++i)
					{
						indent(t_output_stream, new_indent);
						outputJsonToStream(t_output_stream, t_val[i], new_indent);
						t_output_stream << (i + 1 == size ? "\n" : ",\n");
					}
					indent(t_output_stream, t_indent);
					t_output_stream << ']';
				}
				break;
			}
			case Json::objectValue:
			{
				if (t_val.empty())
				{
					t_output_stream << "{}";
				}
				else
				{
					t_output_stream << "{\n";
					int new_indent = t_indent + 4;
					vector<string> keys = t_val.getMemberNames();
					for (size_t i = 0; i < keys.size(); ++i)
					{
						indent(t_output_stream, new_indent);
						const string& key = keys[i];
						t_output_stream << '"' << key << '"' << " : ";
						outputJsonToStream(t_output_stream, t_val[key], new_indent);
						t_output_stream << (i + 1 == keys.size() ? "\n" : ",\n");
					}
					indent(t_output_stream, t_indent);
					t_output_stream << '}';
				}
				break;
			}
			default:
				cerr << "Wrong type!" << endl;
				exit(0);

			}
		}


	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonCppTest::s_start_mem_state;
}