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
    //style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0);
    //style->Colors[ImGuiCol_FrameBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Title bar for window is transparent
    // Text Styling
    style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    // Border Styling
    style->WindowBorderSize = 0.0f;
    // Drop Down (Combo)
    //style->SelectableTextAlign = ImVec2(0.5f, 0.5f);
    style->Colors[ImGuiCol_PopupBg] = ImColor(100.0f, 150.0f, 150.0f, 0.0f);
    style->PopupBorderSize = 0.0f;
    // hovering of items in drop down
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(100.0f, 150.0f, 150.0f, 0.8f);    
    style->Colors[ImGuiCol_HeaderActive] = ImColor(200.0f, 150.0f, 150.0f, 0.0f);    
}

void UI::Demo() {
    ImGui::ShowDemoWindow();
}

void UI::StartAndResets(ProgState &progState, std::shared_ptr<Algorithms::PathFinder> &currAlgo,  std::shared_ptr<Grid> grid)
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) *0.5, work_pos.y-5), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    if (ImGui::Begin("Buttons", NULL, window_flags)) {
    	ImGui::SetWindowFontScale(1.8f);
    	if (ImGui::Button("Clear Path") && !progState.isAlgoRunning)
    	{	// Reset App
            grid->clearPath();
            if (grid->isStartAndEndSet())
                currAlgo->Init(grid->getStartCoord());
        }ImGui::SameLine();
        if (ImGui::Button("Clear Board") && !progState.isAlgoRunning)
        {	// Reset App
            grid->clearBoard();
            if (grid->isStartAndEndSet())
                currAlgo->Init(grid->getStartCoord());
        }
        if (ImGui::Button("Reset Weights") && !progState.isAlgoRunning)
        {	// Reset App
            grid->resetWeights();
        } ImGui::SameLine(0, 45);
        if (ImGui::Button("Reset") && !progState.isAlgoRunning)
        {	// Reset App
            grid->reset();
            progState.Reset();
        }  
        ImGui::Dummy(ImVec2(ImGui::GetWindowWidth() / 3, 0)); ImGui::SameLine();
        // Reset Weights Button
        // Start/ Pause Button
        static char* buttonName = "Start";
        if (progState.isAlgoRunning) { buttonName = "Pause"; }
        else { buttonName = "Start"; }
        if (ImGui::Button(buttonName, ImVec2(ImGui::GetColumnWidth()/3 + 20, 40)))
        {
            if (!progState.isAlgoRunning && grid->isStartAndEndSet())
            {
                // only set to true if algorithm chosen
                // if algorithm totally complete, Rerun!
                if (progState.algoFinished) {
                    grid->clearPath(); // clear the last path as we are executing again
                    currAlgo->Init(grid->getStartCoord()); // Initiate algorithm and clear data from a previous run
                }
                progState.isAlgoRunning = true;
                std::cout << "Started Algorithm" << std::endl;
            }
            else if (progState.isAlgoRunning) { // Stop Button Functionality is algorithm is Running
                progState.isAlgoRunning = false;
                std::cout << "Stopped Algorithm" << std::endl;
            }
        }
    }ImGui::End(); ImGui::PopStyleColor();

}

void UI::AlgoChoices(ProgState& progState, std::shared_ptr<Algorithms::PathFinder> &currAlgo, std::shared_ptr<Grid> grid)
{
    const char* algos[] = {"  \tBFS", "  \tDFS", "\tDijkstra", "  \tAStar" };
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
            for (int n = 0; n < IM_ARRAYSIZE(algos); n++)
            {
                bool is_selected = (current_item == algos[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(algos[n], is_selected, 0, ImVec2(180,25) )) {
                    current_item = algos[n];
                    grid->clearPath(); // clear the last path as we are executing again
                    
                    if (n == 0) {
                        currAlgo.reset(new Algorithms::BFS(grid, progState.numSearchDirections));
                    }
                    else if (n == 1) {
                        currAlgo.reset(new Algorithms::DFS(grid, progState.numSearchDirections));
                    }
                    else if (n == 2) {
                        currAlgo.reset(new Algorithms::Dijkstra(grid, progState.numSearchDirections));
                    }
                    else if (n == 3) {
                        currAlgo.reset(new Algorithms::Astar(grid, progState.numSearchDirections));
                        currAlgo->SetHeuristic(Algorithms::Heuristic::MANHATTAN);
                    }
                   
                    // Initiate the chosen algorithm
                    if (grid->isStartAndEndSet()) {
                        grid->clearPath(); // clear the last path as we are executing again
                        currAlgo->Init(grid->getStartCoord()); // Initiate algorithm and clear data from a previous run
                    }
                    //std::cout << current_item << " Selected!" << std::endl;
                    std::cout << currAlgo->getName() << " Selected!!!" << std::endl;
                } 
            }
            ImGui::EndCombo();
        }
    }ImGui::End(); ImGui::PopStyleColor();
}

void UI::Status(ProgState &progState)
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.1f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x+work_size.x)*0.5, work_pos.y+120), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(390, 90));
    if (ImGui::Begin("Status", NULL, window_flags)) 
    {
        ImGui::SetWindowFontScale(1.6f);
        float win_width = ImGui::GetWindowSize().x;
        float text_width = ImGui::CalcTextSize(progState.status.c_str()).x;
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
        ImGui::TextWrapped(progState.status.c_str());
        ImGui::PopTextWrapPos();
    }ImGui::End();

    // Speed Status
    std::string speed = "Speed: " + std::to_string(progState.speed);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) *0.5 - 200, work_pos.y + 20), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Speed Stat", NULL, window_flags)) {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text(speed.c_str());
    } ImGui::End();
}

void UI::Toggles(std::shared_ptr<Layout> &layout, ProgState& progState, std::shared_ptr<Algorithms::PathFinder>& currAlgo, std::shared_ptr<Grid> grid) {

    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.0f); 
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 40 , work_pos.y + 30.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
    if (ImGui::Begin("##SearchDirections", NULL, window_flags))
    {
        ImGui::SetWindowFontScale(1.1f);
        // Directional search Toggles    
        if (ImGui::RadioButton("8-Way\nSearch", &progState.numSearchDirections, 8)) { currAlgo->setNumSearchDirections(8); } ImGui::SameLine();
        if (ImGui::RadioButton("4-Way\nSearch", &progState.numSearchDirections, 4)) { currAlgo->setNumSearchDirections(4); } ImGui::SameLine();
    } ImGui::End(); ImGui::PopStyleColor();

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x, work_pos.y + 70.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
    if (ImGui::Begin("##GridSizes", NULL, window_flags))
    {
        ImGui::SetWindowFontScale(1.1f);
        
        // Grid Size Toggles    
        if (ImGui::RadioButton("Small \nGrid", temp, 0)) {layout->setCoordSys(20); 
        grid->reset();
        grid->Init(layout->uiAdjustedGridHeight(),
            layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
        } ImGui::SameLine();
        if (ImGui::RadioButton("Medium \nGrid", temp, 1)) {layout->setCoordSys(50); 
        grid->reset();
        grid->Init(layout->uiAdjustedGridHeight(),
            layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
        } ImGui::SameLine();
        if (ImGui::RadioButton("Large \nGrid", temp, 2)) {layout->setCoordSys(100); 
        grid->reset();
        grid->Init(layout->uiAdjustedGridHeight(),
            layout->getCoordSysDim(), m_CameraController->GetCamera().GetViewProjectionMatrix());
        } ImGui::SameLine();
        
    } ImGui::End(); ImGui::PopStyleColor();

    static int currNode = 1;
    if (grid->isStartAndEndSet()) {
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.5, work_pos.y + 230.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
        if (ImGui::Begin("##NodePlacement", NULL, window_flags))
        {
            ImGui::SetWindowFontScale(1.5f);
            // Directional search Toggles    
            if (ImGui::RadioButton("Place\nWall Node", &currNode,  1)) { progState.currNodePlaceType = NodePlacement::WALL; } ImGui::SameLine();
            if (ImGui::RadioButton("Place\nWeight Node", &currNode, 2)) { progState.currNodePlaceType = NodePlacement::WEIGHT; } ImGui::SameLine();
        } ImGui::End(); ImGui::PopStyleColor();
    }
    else { currNode = 1; }
    // Heurestic
    static int currHeuristic = 1;
    if (currAlgo->getName() == "Astar") {
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(ImVec2(work_pos.x + 5, work_pos.y + 230.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
        if (ImGui::Begin("##Heuristic", NULL, window_flags))
        {
            ImGui::SetWindowFontScale(1.0f);
            // Directional search Toggles    
            if (ImGui::RadioButton("Manhattan\nHeuristic", &currHeuristic, 1)) { currAlgo->SetHeuristic(Algorithms::Heuristic::MANHATTAN); } ImGui::SameLine();
            if (ImGui::RadioButton("Chebyshev\nHeuristic", &currHeuristic, 2)) { currAlgo->SetHeuristic(Algorithms::Heuristic::CHEBYSHEV); } ImGui::SameLine();
            if (ImGui::RadioButton("Euclidean\nHeuristic", &currHeuristic, 3)) { currAlgo->SetHeuristic(Algorithms::Heuristic::EUCLIDEAN); } ImGui::SameLine();
        } ImGui::End(); ImGui::PopStyleColor();
    }
    
    
}

void UI::HelpMenu()
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    window_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoSavedSettings ;
    ImGui::SetNextWindowBgAlpha(0.9f); 
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x), work_pos.y + 0), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(250, 195));
    ImGui::SetNextWindowCollapsed(1, ImGuiCond_Once);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 1));
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::Begin("Help Menu", NULL, window_flags);
        ImGui::SetWindowFontScale(1.2f);
        ImGui::TextWrapped("\t\tInstructions:"); 
        ImGui::TextWrapped("Left click to place Start, End, Wall, or Weight cells.");
        ImGui::TextWrapped("");
        ImGui::TextWrapped("Right click to remove Walls or Weights.");
        ImGui::TextWrapped("");
        ImGui::TextWrapped("Increase the weight by left clicking/moving mouse multiple times on a cell");
        ImGui::TextWrapped("when Weight node is toggled");
        ImGui::TextWrapped("");
        ImGui::TextWrapped("\t\tKey Bindings:"); 
        ImGui::TextWrapped("Change Grid Size:\n\tUP/DOWN Arrow Keys");
        ImGui::TextWrapped("Change Algorithm Speed:\n\tLEFT/RIGHT Arrow Keys");
        ImGui::TextWrapped("Start/Pause Algorithm:\n\tSPACE BAR");
   ImGui::End(); ImGui::PopStyleColor();
}

