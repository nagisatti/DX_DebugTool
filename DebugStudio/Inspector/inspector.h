/*
*   inspector.h
*   ƒQ[ƒ€Às’†‚É•Ï”“™‚ğŠm”FA•ÏX‚Å‚«‚é‹@”\
*/
#ifndef INSPECTOR_H
#define INSPECTOR_H

//********************************************************************
// ƒCƒ“ƒNƒ‹[ƒh
//********************************************************************
#include <string>
#include <vector>

//********************************************************************
// ƒNƒ‰ƒX
//********************************************************************
class Inspector
{
public:
	// •Ï”‚ğ“o˜^
	void RegisterFloat(const std::string& name, float* value);
	void RegisterInt(const std::string& name, int* value);
	void RegisterBool(const std::string& name, bool* value);

	// Inspector‚Ì•`‰æ
	void Render();

	// “o˜^‚µ‚½•Ï”‚Ìíœ
	void Delete();

private:
	// “o˜^‚·‚é•Ï”‚ÌŒ^
	enum class ValueType
	{
		Float,
		Int,
		Bool,
	};

	// “o˜^‚·‚é•Ï”‚Ìî•ñ
	struct InspectorItem
	{
		std::string name;
		ValueType type;
		void* value;
	};

	// “o˜^‚³‚ê‚½•Ï”‚Ìˆê——
	std::vector<InspectorItem> m_items;
};


#endif // INSPECTOR_H
