#pragma once
#include "tsl/ordered_map.h"
#include "VinaCommonCtls.hpp"
#include "VertexUI/vui.parser/vui.parser.hpp"
//#include "Vui.Foundation/Encoding.h"
const wchar_t* testChar = LR"(
Text{id("114514"),x(230),y(220),cx(100),cy(40),text("TextQAQ")}
FontIcon{id("114519"),x(230),y(410),size(40),text("test-right")}
FontIcon{id("114524"),x(280),y(410),size(25),text("test-right")}
FontIcon{id("114534"),x(310),y(410),size(15),text("test-right")}
Button{id("QwQ"),x(100),y(20),cx(100),cy(100),text("模拟点击"),internalEvent("NIUZHI")}
Button{id("QwQ2"),x(100),y(140),cx(100),cy(100),text("层2")}
Button{id("QwQ3"),x(100),y(260),cx(200),cy(100),text("层3（nznb")}
Button{id("QwQ91"),x(160),y(180),cx(100),cy(100),text("层4")}
Button{id("QwQ31"),x(200),y(260),cx(200),cy(100),text("层5")}
Button{id("QwQ4"),x(100),y(380),cx(100),cy(100),text("层6")}

)";
tsl::ordered_map < std::wstring, std::variant < std::monostate, std::shared_ptr<VinaButton>, std::shared_ptr<VinaText>, std::shared_ptr<VinaFAIcon>>> comVina;
int ParseVinaCom() {
	auto parser = vui::parser::wparser(testChar);
	parser.parse();
	for (auto obj : parser)
	{
		if (obj.name() == std::wstring(L"Button"))
		{
			std::wstring id;
			std::wstring text;
			std::wstring internalEvent=L"";
			int x, y, cx, cy = 0;
			for (auto pair : obj)
			{
				if (pair.name() == std::wstring(L"id"))pair.get(id);
				if (pair.name() == std::wstring(L"x"))pair.get(x);
				if (pair.name() == std::wstring(L"y"))pair.get(y);
				if (pair.name() == std::wstring(L"cx"))pair.get(cx);
				if (pair.name() == std::wstring(L"cy"))pair.get(cy);
				if (pair.name() == std::wstring(L"text"))pair.get(text);
				if (pair.name() == std::wstring(L"internalEvent"))pair.get(internalEvent);
			}
			std::shared_ptr<VinaButton> cb=std::make_shared <VinaButton>();
			cb->Set(x, y, cx, cy, text.c_str());
			cb->SetInternalEvent(internalEvent);
			comVina[id.c_str()] = cb;
		}
		if (obj.name() == std::wstring(L"Text"))
		{
			std::wstring id;
			std::wstring text;
			int x, y, cx, cy = 0;
			for (auto pair : obj)
			{
				if (pair.name() == std::wstring(L"id"))pair.get(id);
				if (pair.name() == std::wstring(L"x"))pair.get(x);
				if (pair.name() == std::wstring(L"y"))pair.get(y);
				if (pair.name() == std::wstring(L"cx"))pair.get(cx);
				if (pair.name() == std::wstring(L"cy"))pair.get(cy);
				if (pair.name() == std::wstring(L"text"))pair.get(text);

			}
			std::shared_ptr<VinaText> cb = std::make_shared <VinaText>();
			cb->Set(x, y, cx, cy, text.c_str());
			comVina[id.c_str()] = cb;
		}
		if (obj.name() == std::wstring(L"FontIcon"))
		{
			std::wstring id;
			std::wstring text;
			int x, y,  cy = 0;
			for (auto pair : obj)
			{
				if (pair.name() == std::wstring(L"id"))pair.get(id);
				if (pair.name() == std::wstring(L"x"))pair.get(x);
				if (pair.name() == std::wstring(L"y"))pair.get(y);
				if (pair.name() == std::wstring(L"size"))pair.get(cy);
				if (pair.name() == std::wstring(L"text"))pair.get(text);

			}
			std::shared_ptr<VinaFAIcon> cb = std::make_shared <VinaFAIcon>();
			cb->Set(x, y, text.c_str(),cy);
			comVina[id.c_str()] = cb;
		}
	}

	return 0;
}
int LoadVinaCom() {
	ParseVinaCom();
	return 0;
}