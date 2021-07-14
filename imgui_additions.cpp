#include "imgui_additions.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
    #define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include "utils/stringify.h"


/// <summary>Converts int to human readable time format.</summary>
/// <param name="seconds">Time in seconds</param>
/// <returns><see cref="seconds"/> formatted as string</returns>
std::string intToTime(int seconds)
{
    std::string converted_time = "";
    // Hours
    if (seconds >= 3600) {
        int hours = int(seconds / 3600);
        converted_time += std::to_string(hours) + ":";
        seconds -= 3600 * hours;
    }
    // Minutes
    if (seconds >= 60) {
        int minutes = int(seconds / 60);
        converted_time += std::to_string(minutes) + ":";
        seconds -= 60 * minutes;
    }
    else if (!converted_time.empty())
        converted_time += "0:";
    // Seconds
    converted_time += std::to_string(seconds);

    return converted_time;
}


/// <summary>Returns the width from the previous frame.</summary>
/// <returns>Frame width</returns>
float ImGui::GetFrameWidth()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.x * 2.0f;
}


/// <summary>Returns the width with item spacing from the previous frame.</summary>
/// <returns>Frame width with item spacing</returns>
float ImGui::GetFrameWidthWithSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.x * 2.0f + g.Style.ItemSpacing.x;
}


/// <summary>Check if the current item was active last frame.</summary>
/// <returns>Bool with if the current item was active last frame</returns>
bool ImGui::IsItemActiveLastFrame() {
    ImGuiContext& g = *GImGui;
    if (g.ActiveIdPreviousFrame)
        return g.ActiveIdPreviousFrame == GImGui->CurrentWindow->DC.LastItemId;

    return false;
}


/// <summary>Check if the current item was released last frame.</summary>
/// <returns>Bool with if the current item was released last frame</returns>
bool ImGui::IsItemJustReleased() {
    return IsItemActiveLastFrame() && !IsItemActive();
}


/// <summary>Begins a disabled block.</summary>
/// <remarks>Everything between <c>ImGui::BeginDisabled()</c> and <c>ImGui::EndDisabled()</c> will be disabled and greyed out.</remarks>
void ImGui::BeginDisabled()
{
    PushItemFlag(ImGuiItemFlags_Disabled, true);
    PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}


/// <summary>End the disabled block.</summary>
void ImGui::EndDisabled()
{
    PopItemFlag();
    PopStyleVar();
}


/// <summary>Begins a success border block.</summary>
/// <remarks>Everything between <c>ImGui::BeginSuccessBorder()</c> and <c>ImGui::EndSuccessBorder()</c> will get a green border to indicate that there was no error.</remarks>
void ImGui::BeginSuccessBorder()
{
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor(56, 142, 60));
    PushStyleColor(ImGuiCol_BorderShadow, ImVec4());
}



/// <summary>End the success block.</summary>
void ImGui::EndSuccessBorder()
{
    EndBorder();
}


/// <summary>Begins a warning border block.</summary>
/// <remarks>Everything between <c>ImGui::BeginErrorBorder()</c> and <c>ImGui::EndErrorBorder()</c> will get a orange border to indicate that there is an warning there.</remarks>
void ImGui::BeginWarnBorder()
{
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor(245, 124, 0));
    PushStyleColor(ImGuiCol_BorderShadow, ImVec4());
}


/// <summary>End the warning border block.</summary>
void ImGui::EndWarnBorder()
{
    EndBorder();
}


/// <summary>Begins a error border block.</summary>
/// <remarks>Everything between <c>ImGui::BeginErrorBorder()</c> and <c>ImGui::EndErrorBorder()</c> will get a red border to indicate that there is an error there.</remarks>
void ImGui::BeginErrorBorder()
{
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor(211, 47, 47));
    PushStyleColor(ImGuiCol_BorderShadow, ImVec4());
}


/// <summary>End the error border block.</summary>
void ImGui::EndErrorBorder()
{
    EndBorder();
}


/// <summary>End any border block.</summary>
void ImGui::EndBorder()
{
    PopStyleColor(2);
    PopStyleVar();
}


bool ImGui::Banner(const char* label, const ImVec4 color, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y * 1.5f);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ImRect bb_inner(pos, pos + size);
    bb_inner.Min.x += style.ItemSpacing.x;
    ItemSize(size, 0.0f);

    // Fill horizontal space.
    ImVec2 window_padding = window->WindowPadding;
    float max_x = GetWindowContentRegionMax().x;
    float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - pos.x);
    ImVec2 size_draw(w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
    ImRect bb(pos, pos + size_draw);
    bb.Max.x += window_padding.x;

    const float spacing_y = style.ItemSpacing.y;
    const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
    bb.Min.y -= spacing_U;
    bb.Max.y += (spacing_y - spacing_U);

    if (!ItemAdd(bb, id))
        return false;
    SetItemAllowOverlap();

    // RenderFrame
    const ImU32 col = ColorConvertFloat4ToU32(color);
    window->DrawList->AddRectFilled(bb.Min, bb.Max, col, 0.0f);
    const float border_size = g.Style.ChildBorderSize;
    if (border_size > 0.0f)
    {
        window->DrawList->AddRect(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), 0.0f, ImDrawCornerFlags_All, border_size);
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), 0.0f, ImDrawCornerFlags_All, border_size);
    }
    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

    RenderTextClipped(bb_inner.Min, bb_inner.Max, label, NULL, &label_size, ImVec2(0, 0.5f), &bb);

    const ImVec2 pad = style.FramePadding;
    const ImVec2 close_button_size = ImVec2(g.FontSize, g.FontSize) + g.Style.FramePadding * 2.0f;
    const ImVec2 close_button_pos = ImVec2(bb.Max.x - close_button_size.x, bb.Min.y + (bb.Max.y - bb.Min.y - close_button_size.y) / 2);
    PushStyleColor(ImGuiCol_ButtonHovered, col & (0x33u << IM_COL32_A_SHIFT));
    PushStyleColor(ImGuiCol_ButtonActive, col & (0x66u << IM_COL32_A_SHIFT));
    const bool clicked = CloseButton(window->GetID("#CLOSE"), close_button_pos);
    PopStyleColor(2);

    return clicked;
}


static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

/// <summary>Modified instance of <see cref="ImGui::Combo"/> that supports <c>std::vector&lt;std::string&gt;</c> instead of <c>const char*</c> arrays.</summary>
/// <param name="label">Combo label</param>
/// <param name="current_item">Index of selected item</param>
/// <param name="items"><c>std::vector</c> of <c>std::string</c> labels for items</param>
/// <param name="default_preview_text">Text previewed when label for the selected item can not be displayed</param>
/// <param name="popup_max_height_in_items">Height of dropdown in items</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::Combo(const char* label, size_t* current_item, std::vector<std::string> items, const char* default_preview_text, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    const char* preview_text = default_preview_text;
    if (*current_item >= items.size())
        *current_item = 0;
    if (*current_item < items.size())
        preview_text = items[*current_item].c_str();

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!BeginCombo(label, preview_text, 0))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (auto i = 0u; i < items.size(); i++)
    {
        PushID((void*)(intptr_t)i);
        const bool item_selected = (i == *current_item);
        std::string item_text = items[i];
        if (!item_text.empty())
        {
            if (Selectable(item_text.c_str(), item_selected))
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
        }
        PopID();
    }

    EndCombo();
    return value_changed;
}


/// <summary>Modified instance of <see cref="ImGui::Combo"/> that supports <c>std::map&lt;std::string, std::string&gt;</c> instead of <c>const char*</c> arrays.</summary>
/// <param name="label">Combo label</param>
/// <param name="current_item">Key of selected item</param>
/// <param name="items"><c>std::vector</c> of <c>std::string</c> labels for items</param>
/// <param name="default_preview_text">Text previewed when label for the selected item can not be displayed</param>
/// <param name="popup_max_height_in_items">Height of dropdown in items</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::Combo(const char* label, std::string& current_item, std::map<std::string, std::string> items, const char* default_preview_text, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    const char* preview_text = NULL;
    if (items.empty())
        preview_text = default_preview_text;
    else if (items.find(current_item) == items.end())
    {
        current_item = items.begin()->first;
        preview_text = items[current_item].c_str();
    }
    else
        preview_text = items[current_item].c_str();

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!BeginCombo(label, preview_text, 0))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (const auto& [key, value] : items)
    {
        PushID(key.data());
        const bool item_selected = (key == current_item);
        std::string item_text = value;
        if (!item_text.empty())
        {
            if (Selectable(item_text.c_str(), item_selected))
            {
                value_changed = true;
                current_item = key;
            }
            if (item_selected)
                SetItemDefaultFocus();
        }
        PopID();
    }

    EndCombo();
    return value_changed;
}


/// <summary>Modified instance of <see cref="ImGui::SliderFloat"/> that supports <c>std::vector&lt;std::string&gt;</c> instead of floats.</summary>
/// <param name="label">Slider label</param>
/// <param name="current_item">Index of selected item</param>
/// <param name="array"><c>std::vector</c> of <c>std::string</c> labels for items</param>
/// <param name="format">Display format string</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::SliderArray(const char* label, size_t* current_item, std::vector<std::string> array, const char* format)
{
    size_t a_min = 0;
    size_t a_max = array.empty() ? 0 : array.size() - 1;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (!format)
        format = "%s";

    const bool hovered = ItemHoverable(frame_bb, id);
    const bool tab_focus_requested = FocusableItemRegister(window, id);
    if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, ImGuiDataType_U64, current_item, &a_min, &a_max, "%I64u", 1, ImGuiSliderFlags_None, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* preview_value = "";
    if (*current_item < array.size()) {
        preview_value = array[*current_item].c_str();
    }
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, preview_value);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}


/// <summary>Modified instance of <see cref="ImGui::DragInt"/> that displays time instead of ints.</summary>
/// <param name="label">Widget label</param>
/// <param name="v">Time in seconds</param>
/// <param name="v_speed">How fast values change on drag</param>
/// <param name="v_min">Min value allowed by widget</param>
/// <param name="v_max">Max value allowed by widget</param>
/// <param name="format">Display format string</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::DragTime(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format)
{
    if (*v < v_min)
        *v = v_min;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (!format)
        format = "%s";

    // Tabbing or CTRL-clicking on Drag turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = TempInputTextIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = FocusableItemRegister(window, id);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        const bool double_clicked = (hovered && g.IO.MouseDoubleClicked[0]);
        if (focus_requested || clicked || double_clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || double_clicked || g.NavInputId == id)
            {
                temp_input_start = true;
                FocusableItemUnregister(window);
            }
        }
    }
    if (temp_input_is_active || temp_input_start)
        return TempInputTextScalar(frame_bb, id, label, ImGuiDataType_S32, v, "%d");

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Drag behavior
    const bool value_changed = DragBehavior(id, ImGuiDataType_S32, v, v_speed, &v_min, &v_max, "%d", 1, ImGuiDragFlags_None);
    if (value_changed)
        MarkItemEdited(id);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, intToTime(*v).c_str());
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}


/// <summary>Modified instance of <see cref=" ImGui::Checkbox"/> with a different look.</summary>
/// <remarks>Hardcoded to fit with the current GUI, and does nothing but look pritty on its own.</remarks>
/// <param name="label">Widget label</param>
/// <param name="v">Bool if the checkbox is checked</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::SwitchCheckbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !*v;
        MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    ImU32 check_bg;
    ImVec2 check_pos;
    if (*v)
    {
        check_bg = GetColorU32(ImVec4(0.13f, 0.59f, 0.95f, 1.0f));
        check_pos = ImVec2(check_bb.Min.x + 2 + 14, check_bb.Min.y + 9);
    }
    else
    {
        check_bg = GetColorU32(ImVec4(0.20f, 0.25f, 0.30f, 1.0f));
        check_pos = ImVec2(check_bb.Min.x + 2 + 6, check_bb.Min.y + 9);
    }

    window->DrawList->AddRectFilled(ImVec2(check_bb.Min.x + 1, check_bb.Min.y + 2), ImVec2(check_bb.Max.x + 4, check_bb.Max.y - 3), check_bg, 10.0f);
    window->DrawList->AddCircleFilled(check_pos, 8, GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 12);

    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

    return pressed;
}


/// <summary>Modified instance of <see cref=" ImGui::InputFloat3"/> with support for vectors.</summary>
/// <remarks>Hardcoded to fit with the current GUI.</remarks>
/// <param name="label">Widget label</param>
/// <param name="v">Vector of floats</param>
/// <returns>Bool with if the value changed</returns>
bool ImGui::DragVector(const char* label, Vector* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(3, CalcItemWidth());

    // X
    PushID("x");
    value_changed |= DragFloat("##v", &v->X, v_speed, v_min, v_max, format, power);
    PopID();
    PopItemWidth();

    // Y
    PushID("y");
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragFloat("##v", &v->Y, v_speed, v_min, v_max, format, power);
    PopID();
    PopItemWidth();

    // Z
    PushID("z");
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragFloat("##v", &v->Z, v_speed, v_min, v_max, format, power);
    PopID();
    PopItemWidth();

    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0.0f, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }
    EndGroup();

    return value_changed;
}


/// <summary>Renders <see cref="label"/> within a frame.</summary>
/// <remarks>This is used to draw borders around text.</remarks>
/// <param name="label">Widget label</param>
/// <param name="size_arg">Size of the frame</param>
void ImGui::FramedText(const char* label, ImVec2 size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

    ImGuiWindow* draw_window = window;

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return;

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_ChildBg), true, style.FrameRounding);

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 render_pos = frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.f, 0.f);

    const char* text_display_end = FindRenderedTextEnd(label, NULL);

    draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), label, text_display_end, 0.0f, &clip_rect);
}


void ImGui::SetTooltip(std::string text)
{
    SetTooltip("%s", text.c_str());
}


void ImGui::TextWrapped(std::string text)
{
    bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);    // Keep existing wrap position is one ia already set
    if (need_wrap) PushTextWrapPos(0.0f);
    Text("%s", text.c_str());
    if (need_wrap) PopTextWrapPos();
}


/// <summary>Wraps colored text.</summary>
/// <remarks>Combination of <see cref="ImGui::TextColored"/> and <see cref="ImGui::TextWrapped"/>.</remarks>
/// <param name="col">Color of the text</param>
/// <param name="fmt">String to be formatted</param>
void ImGui::TextColoredWrapped(const ImVec4& col, const char* fmt, ...)
{
    bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);    // Keep existing wrap position is one ia already set
    if (need_wrap) PushTextWrapPos(0.0f);
    va_list args;
    va_start(args, fmt);
    TextColoredV(col, fmt, args);
    va_end(args);
    if (need_wrap) PopTextWrapPos();
}


/// <summary>Wraps colored text.</summary>
/// <remarks>Combination of <see cref="ImGui::TextColored"/> and <see cref="ImGui::TextWrapped"/>.</remarks>
/// <param name="col">Color of the text</param>
/// <param name="text">String to be displayed</param>
void ImGui::TextColoredWrapped(const ImVec4& col, std::string text)
{
    bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);    // Keep existing wrap position is one ia already set
    if (need_wrap) PushTextWrapPos(0.0f);
    TextColored(col, "%s", text.c_str());
    if (need_wrap) PopTextWrapPos();
}


/// <summary>Color picker.</summary>
/// <param name="label">Widget label</param>
/// <param name="current_item">Index of selected item</param>
/// <param name="custom_colors"><c>std::vector</c> of <c>ImVec4</c> colors</param>
/// <param name="hue_count">Number of hue columns to split the custom_colors in</param>
/// <param name="default_color">Default color for the ColorButton</param>
/// <param name="size">Size if the ColorButton</param>
void ImGui::RLColorPicker(const char* label, int8_t* current_item, std::vector<ImVec4> custom_colors, int hue_count, ImVec4 default_color, ImVec2 size)
{
    const ImVec4 current_color = current_item < 0 || *current_item >= custom_colors.size() ? default_color : custom_colors[*current_item];

    PushID(label);
    if (ColorButton("##CurrentRLColor", current_color, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoDragDrop, size))
        OpenPopup("RLColorPicker");
    if (BeginPopup("RLColorPicker")) {
        if (ColorButton("##DefaultRLColor", default_color)) {
            *current_item = -1;
            CloseCurrentPopup();
        }
        SameLine();
        Text("Default");

        for (char i = 0; i < static_cast<char>(custom_colors.size()); i++) {
            PushID(i);
            if (ColorButton("##CustomRLColor", custom_colors[i])) {
                *current_item = i;
                CloseCurrentPopup();
            }
            PopID();
            if (i % hue_count != hue_count - 1)
                SameLine(0, 4);
        }
        EndPopup();
    }
    PopID();
}


struct InputTextCallback_UserData
{
    std::string* Str;
    ImGuiInputTextCallback  ChainCallback;
    void* ChainCallbackUserData;
};


static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}


bool ImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}


bool ImGui::InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}


bool ImGui::InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}
