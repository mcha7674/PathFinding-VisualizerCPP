#include "UI.h"

UI::UI(std::shared_ptr<Utils::OrthographicCameraController> m_CameraController)
	:m_CameraController(m_CameraController)
{
    io = &ImGui::GetIO();
    viewport = ImGui::GetMainViewport();
    style = &ImGui::GetStyle();

    InitImGuiGlobalStyling();
    UpdateWorkSize();
}

void UI::UpdateWorkSize()
{
    work_pos = viewport->Pos;
    work_size = viewport->Size;
}

// Initialization Functions
void UI::InitImGuiGlobalStyling()
{
    // Style Text Color
    style->Colors[ImGuiCol_Text] = ImColor(153, 178, 242);
    // Border Color
    style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0);
    // Button Related Coloring
    style->Colors[ImGuiCol_Button] = ImColor(100.0f, 150.0f, 150.0f, 1.0f);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(153.0f, 178.0f, 242.0f, 0.0f);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(100.0f, 150.0f, 150.0f, 1.0f);
    //Background coloring
    style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0);
    style->Colors[ImGuiCol_FrameBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Title bar for window is transparent
    style->Colors[ImGuiCol_TitleBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Text Styling
    style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    // Border Styling
    style->WindowBorderSize = 0.0f;
    // Drop Down (Combo)
    //style->SelectableTextAlign = ImVec2(0.5f, 0.5f);
    style->Colors[ImGuiCol_PopupBg] = ImColor(100.0f, 150.0f, 150.0f, 0.0f);
    style->PopupBorderSize = 0.0f;
    // hovering of items in drop down
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(100.0f, 150.0f, 150.0f, 0.4f);    
    //style->Colors[ImGuiCol_HeaderHovered] = ImColor(50.0f, 50.0f, 50.0f, 0.f);    
    style->Colors[ImGuiCol_HeaderActive] = ImColor(200.0f, 150.0f, 150.0f, 0.0f);    
    
    //style->Colors[ImGuiCol_Header] = ImColor(100.0f, 150.0f, 150.0f, 0.5f);    
    //style->Colors[ImGuiCol_ChildBg] = ImColor(100.0f, 150.0f, 150.0f, 0.0f);
    //style->Colors[ImGuiCol_TextSelectedBg] = ImColor(100.0f, 150.0f, 150.0f, 0.0f);
}

void UI::Demo() {
    ImGui::ShowDemoWindow();
}

void UI::StartAndResets(bool& isAlgoRunning, bool isStartAndEndSet, std::shared_ptr<Grid> grid)
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
    ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) *0.5, work_pos.y), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    if (ImGui::Begin("Buttons", NULL, window_flags)) {
    	ImGui::SetWindowFontScale(1.8f);
    	if (ImGui::Button("Clear Path") && !isAlgoRunning)
    	{	// Reset App
            grid->clearPath();
        }ImGui::SameLine();
        if (ImGui::Button("Clear Board") && !isAlgoRunning)
        {	// Reset App
            grid->clearBoard();
        }ImGui::SameLine();
        if (ImGui::Button("Reset") && !isAlgoRunning)
        {	// Reset App
            grid->reset();
        } ImGui::SameLine(); ImGui::Dummy(ImVec2(0, ImGui::GetWindowHeight()/2 - 5));
        ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 3, 0)); ImGui::SameLine();

        if (ImGui::Button("Start", ImVec2(ImGui::GetColumnWidth()/3 + 20, 40)))
        {
            if (!isAlgoRunning && isStartAndEndSet)
            {
                // only set to true if algorithm chosen
                isAlgoRunning = true;
                std::cout << "Started Algorithm" << std::endl;
            }
        }
    }ImGui::End(); ImGui::PopStyleColor();
}
void UI::AlgoChoices(UserInput &input)
{
    const char* algos[] = { "Choose Algorithm", "  \tBFS", "  \tDFS", "  \tAStar" };
    static const char* current_item = algos[0];
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    static ImGuiComboFlags comboFlags =  ImGuiComboFlags_NoArrowButton;
    ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x), work_pos.y), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(200,40));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(100.0f, 150.0f, 150.0f, 1.0f));
    if (ImGui::Begin("Dropdown", NULL, window_flags)) {
        ImGui::SetWindowFontScale(1.6f);
        if (ImGui::BeginCombo("##combo", current_item, comboFlags)) // The second parameter is the label previewed before opening the combo.
        {   
            for (int n = 1; n < IM_ARRAYSIZE(algos); n++)
            {
                bool is_selected = (current_item == algos[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(algos[n], is_selected, 0, ImVec2(180,25) )) {
                    current_item = algos[n];
                    if (n == 1) input.algoType = Algorithms::Type::BFS;
                    if (n == 2) input.algoType = Algorithms::Type::DFS;
                    if (n == 3) input.algoType = Algorithms::Type::AStar;
                    std::cout << current_item << " Selected!" << std::endl;
                    //if (is_selected)
                        //ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                } 
            }
            ImGui::EndCombo();
        }
    }ImGui::End(); ImGui::PopStyleColor();
}
void UI::Status(std::string status)
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x+work_size.x)*0.5, work_pos.y+120), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(300, 60));
    if (ImGui::Begin("Status", NULL, window_flags)) 
    {
        ImGui::SetWindowFontScale(1.7f);
        float win_width = ImGui::GetWindowSize().x;
        float text_width = ImGui::CalcTextSize(status.c_str()).x;

        // calculate the indentation that centers the text on one line, relative
        // to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
        float text_indentation = (win_width - text_width) * 0.5f;

        // if text is too long to be drawn on one line, `text_indentation` can
        // become too small or even negative, so we check a minimum indentation
        float min_indentation = 20.0f;
        if (text_indentation <= min_indentation) {
            text_indentation = min_indentation;
        }

        ImGui::SameLine(text_indentation);
        ImGui::PushTextWrapPos(win_width - text_indentation);
        ImGui::TextWrapped(status.c_str());
        ImGui::PopTextWrapPos();
    }ImGui::End();
}

void UI::HelpMenu()
{

}

