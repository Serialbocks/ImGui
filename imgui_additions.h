#pragma once
#include <string>
#include <vector>
#include "imgui.h"

#pragma warning(push, 0)
#include "bakkesmod/wrappers/WrapperStructs.h"
#pragma warning(pop)


namespace ImGui
{
    float GetFrameWidth();
    float GetFrameWidthWithSpacing();
    bool IsItemActiveLastFrame();
    void BeginDisabled();
    void EndDisabled();
    void BeginSuccesBorder();
    void EndSuccesBorder();
    void BeginWarnBorder();
    void EndWarnBorder();
    void BeginErrorBorder();
    void EndErrorBorder();
    bool Combo(const char* label, size_t* current_item, std::vector<std::string> items, const char* default_preview_text, int popup_max_height_in_items = -1);
    bool SliderArray(const char* label, size_t* current_item, std::vector<std::string> array, const char* format = "%s");
    bool DragTime(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%s");
    bool SwitchCheckbox(const char* label, bool v);
    bool DragVector(const char* label, Vector* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    void FramedText(const char* label, ImVec2 size_arg = ImVec2(0, 0));
    void TextColoredWrapped(const ImVec4& col, const char* text);
    void RLColorPicker(const char* label, char* current_item, std::vector<ImVec4> customColors, unsigned char num_colums, ImVec4 defaultColor = ImVec4(), ImVec2 size = ImVec2());
}
