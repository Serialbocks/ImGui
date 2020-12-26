#pragma once
#include "imgui.h"

#include <map>
#include <vector>       // std::vector<>
#include <string>       // std::string
#include <algorithm>    // transform
#include <filesystem>   // std::filesystem::path

namespace ImGui
{
    IMGUI_API bool          BeginSearchableCombo(const char* label, const char* preview_value, char* input, int input_size, const char* input_preview_value, bool* buffer_changed = NULL, ImGuiComboFlags flags = 0);
    IMGUI_API void          EndSearchableCombo();
    IMGUI_API bool          SearchableCombo(const char* label, size_t* current_item, std::vector<std::string> items, const char* default_preview_text, const char* input_preview_value, int popup_max_height_in_items = -1);
    template<typename _Key, typename _Value>
    IMGUI_API bool          SearchableCombo(const char* label, _Key& current_item, std::map<_Key, _Value> items, const char* default_preview_text, const char* input_preview_value, int popup_max_height_in_items = -1);
} // namespace ImGui


// Function templates
template bool ImGui::SearchableCombo<std::string, std::string>(const char* label, std::string& current_item, std::map<std::string, std::string> items, const char* default_preview_text, const char* input_preview_value, int popup_max_height_in_items);
template bool ImGui::SearchableCombo<std::filesystem::path, std::string>(const char* label, std::filesystem::path& current_item, std::map<std::filesystem::path, std::string> items, const char* default_preview_text, const char* input_preview_value, int popup_max_height_in_items);
