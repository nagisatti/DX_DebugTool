/*
*   inspector.cpp
*   ƒQ[ƒ€Às’†‚É•Ï”“™‚ğŠm”FA•ÏX‚Å‚«‚é‹@”\
*/

//********************************************************************
// ƒCƒ“ƒNƒ‹[ƒh
//********************************************************************
#include "inspector.h"
#include "imgui.h"

//********************************************************************
// •Ï”“o˜^
//********************************************************************
#pragma region •Ï”“o˜^
// float
void Inspector::RegisterFloat(const std::string& name, float* value)
{
	// ƒK[ƒhˆ—
	if (value == nullptr)
		return;

	// d•¡“o˜^–h~
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({name,m_currentGroup,ValueType::Float,value});

}

// int
void Inspector::RegisterInt(const std::string& name, int* value)
{
	// ƒK[ƒhˆ—
	if (value == nullptr)
		return;

	// d•¡“o˜^–h~
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({name,m_currentGroup,ValueType::Int,value});

}

// bool
void Inspector::RegisterBool(const std::string& name, bool* value)
{
	// ƒK[ƒhˆ—
	if (value == nullptr)
		return;

	// d•¡“o˜^–h~
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({name,m_currentGroup,ValueType::Bool,value});

}

// Vector2
void Inspector::RegisterVector2(const std::string& name, Vector2* value)
{
	// ƒK[ƒhˆ—
	if (value == nullptr)
		return;

	// d•¡“o˜^–h~
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({name,m_currentGroup,ValueType::VECTOR2,value});
}

// vector3
void Inspector::RegisterVector3(const std::string& name, Vector3* value)
{
	// ƒK[ƒhˆ—
	if (value == nullptr)
		return;

	// d•¡“o˜^–h~
	for (const auto& item : m_items)
	{
		if (item.groupName == m_currentGroup && item.name == name)
			return;
	}

	m_items.push_back({name,m_currentGroup,ValueType::VECTOR3,value});
}

#pragma endregion

//********************************************************************
// Inspector‚Ì•`‰æ
//********************************************************************
void Inspector::Render()
{
	for (const auto& gropItem : m_items)
	{

		const std::string& group = gropItem.groupName;

		// d•¡‚µ‚½•\¦‚Ì–h~
		bool isDisplay = false;
		
		for (const auto& previous : m_items)
		{
			// Œ»İ‚Ì—v‘f‚Ü‚Å’²‚×‚½‚çŒŸõI—¹
			if (&previous == &gropItem)
				break;

			// ˆÈ‘O‚É“¯‚¶ƒOƒ‹[ƒv‚ª‘¶İ‚µ‚½‚çd•¡‚Æ”»’è
			if (previous.groupName == group)
			{
				isDisplay = true;
				break;
			}
		}

		// •\¦‚µ‚Ä‚¢‚½‚ç”ò‚Î‚·
		if (isDisplay)
			continue;


		ImGui::PushID(group.c_str());

		if (ImGui::TreeNode(group.empty() ? "General" : group.c_str()))
		{
			for (const auto& item : m_items)
			{
				if (item.groupName != group)
					continue;

				ImGui::PushID(item.name.c_str());

				// ¶‘¤‚É•Ï”–¼‚ğ•\¦
				ImGui::TextUnformatted(item.name.c_str());

				// Ÿ‚Ì—v‘f‚ğ“¯‚¶s‚É”z’u
				ImGui::SameLine(150.0f);

				// “ü—Í—“‚Ì•
				ImGui::SetNextItemWidth(-1.0f);

				switch (item.type)
				{
				case ValueType::Float:
					//ImGui::DragFloat(item.name.c_str(), static_cast<float*>(item.value));
					ImGui::SliderFloat("##Value", static_cast<float*>(item.value), -100.0f, 5000.0f);
					break;

				case ValueType::Int:
					//ImGui::DragInt(item.name.c_str(), static_cast<int*>(item.value));
					ImGui::SliderInt("##Value", static_cast<int*>(item.value), -100, 5000);
					break;

				case ValueType::Bool:
					ImGui::Checkbox("##Value", static_cast<bool*>(item.value));
					break;

				case ValueType::VECTOR2:
				{
					Vector2* vec = static_cast<Vector2*>(item.value);

					float values[2] = { vec->x,vec->y };

					// ’l‚ª•ÏX‚³‚ê‚½ê‡‚Ì‚İ‘ã“ü
					if (ImGui::SliderFloat2("##Value", values, -100.0f, 5000.0f))
					{
						vec->x = values[0];
						vec->y = values[1];
					}
					break;
				}

				case ValueType::VECTOR3:
				{
					Vector3* vec = static_cast<Vector3*>(item.value);

					float values[3] = { vec->x,vec->y,vec->z };

					// ’l‚ª•ÏX‚³‚ê‚½ê‡‚Ì‚İ‘ã“ü
					if (ImGui::SliderFloat3("##Value", values, -100.0f, 5000.0f))
					{
						vec->x = values[0];
						vec->y = values[1];
						vec->z = values[2];
					}

					break;
				}
				default:
					break;
				}
				ImGui::PopID();
			}			
			// TreeNode‚ğŠJ‚¢‚½ê‡‚¾‚¯ŒÄ‚Ño‚·
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}


//********************************************************************
// •Ï”íœ
//********************************************************************
#pragma region “o˜^‚µ‚Ä‚¢‚é•Ï”‚Ìíœ
// “o˜^‚µ‚½•Ï”‚Ìíœ
void Inspector::Delete()
{
	m_items.clear();
}

// “o˜^‚µ‚½ƒOƒ‹[ƒv‚Ì•Ï”íœ
void Inspector::GroupDelete(const std::string& groupName)
{
	m_items.erase(std::remove_if(m_items.begin(),m_items.end(),[&groupName](const InspectorItem& item)
	{
		return item.groupName == groupName;
	}),
		m_items.end()
	);
}

// “o˜^‚µ‚½1‚Â‚Ì•Ï”‚Ìíœ
void Inspector::OnceDelete(const std::string& name, const std::string& groupName)
{
	m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&name, &groupName](const InspectorItem& item)
	{
		return (item.name == name && item.groupName == groupName);
	}),
		m_items.end()
	);
}

#pragma endregion

//********************************************************************
// ƒOƒ‹[ƒvì¬ŠJn
//********************************************************************
void Inspector::BeginGroup(const std::string& groupName)
{
	m_currentGroup = groupName;
}

//********************************************************************
// ƒOƒ‹[ƒvì¬I—¹
//********************************************************************
void Inspector::EndGroup()
{
	m_currentGroup.clear();
}