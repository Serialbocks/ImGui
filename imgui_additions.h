#pragma once
#include "imgui.h"

#include <map>
#include <string>
#include <vector>

#pragma warning(push, 0)
#include "bakkesmod/wrappers/wrapperstructs.h"
#pragma warning(pop)


namespace ImGui
{
    float GetFrameWidth();
    float GetFrameWidthWithSpacing();
    bool IsItemActiveLastFrame();
    void BeginDisabled();
    void EndDisabled();
    void BeginSuccessBorder();
    void EndSuccessBorder();
    void BeginWarnBorder();
    void EndWarnBorder();
    void BeginErrorBorder();
    void EndErrorBorder();
    void EndBorder();
    bool Banner(const char* label, ImVec4 color, const ImVec2& size_arg = ImVec2(0, 0));
    bool Combo(const char* label, size_t* current_item, std::vector<std::string> items, const char* default_preview_text, int popup_max_height_in_items = -1);
    bool Combo(const char* label, std::string& current_item, std::map<std::string, std::string> items, const char* default_preview_text, int popup_max_height_in_items = -1);
    bool SliderArray(const char* label, size_t* current_item, std::vector<std::string> array, const char* format = "%s");
    bool DragTime(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%s");
    bool SwitchCheckbox(const char* label, bool* v);
    bool DragVector(const char* label, Vector* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    void FramedText(const char* label, ImVec2 size_arg = ImVec2(0, 0));
    void SetTooltip(std::string text);
    void TextWrapped(std::string text);
    void TextColoredWrapped(const ImVec4& col, const char* fmt, ...);
    void TextColoredWrapped(const ImVec4& col, std::string text);
    void RLColorPicker(const char* label, char* current_item, std::vector<ImVec4> custom_colors, int hue_count, ImVec4 default_color = ImVec4(), ImVec2 size = ImVec2());
}
