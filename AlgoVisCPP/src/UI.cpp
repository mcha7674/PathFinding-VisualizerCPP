#include "UI.h"

static float massMultiplier = 1.0f;
float bodyMass = 3.00273f; // in Micro Solar Masses

UI::UI(float* UniverseTime, float* TimeStep, GLCore::Utils::OrthographicCameraController* m_CameraController, Orbit* bodyOrbit)
	:UniverseTime(UniverseTime), TimeStep(TimeStep), m_CameraController(m_CameraController), bodyOrbit(bodyOrbit)
{
    io = &ImGui::GetIO();
    viewport = ImGui::GetMainViewport();
    style = &ImGui::GetStyle();
    plotStyle = &ImPlot::GetStyle();

    InitImGuiGlobalStyling();
    UpdateWorkSize();

    newMass = new float(earthMass);
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
    style->Colors[ImGuiCol_Button] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(153.0f, 178.0f, 242.0f, 0.2f);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(153.0f, 178.0f, 242.0f, 0.2f);
    //style->Colors[ImGuiCol_ButtonActive] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);

    //Background coloring
    style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0);
    style->Colors[ImGuiCol_FrameBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Title bar for window is transparent
    style->Colors[ImGuiCol_TitleBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);

    style->WindowBorderSize = 0.0f;

    // Plot Styles
    // Set Styles
    plotStyle->MarkerSize = 1.0f;
    plotStyle->Marker = ImPlotMarker_Asterisk;
    plotStyle->FillAlpha = 0.0f;
    plotStyle->Colors[ImPlotCol_AxisBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    plotStyle->Colors[ImPlotCol_FrameBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
    plotStyle->Colors[ImPlotCol_PlotBg] = ImColor(0.0f, 0.0f, 0.0f, 0.0f);
}


void UI::TimeOverlay()
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    // Universe Time //
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x * 0.5f, work_pos.y), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    if (ImGui::Begin("Time", NULL, window_flags)) {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("Time: %f Years", *UniverseTime);
    } ImGui::End();
    // FPS //
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 15.0f, work_pos.y + 1.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    if (ImGui::Begin("FPS", NULL, window_flags)) {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("FPS : %d", (uint8_t)(1.0f / Application::Get().GetDeltaTime()));
    } ImGui::End();
}


void UI::fastForwardOverlay(uint16_t &fastForward, int &fastForwardActive)
{
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    	ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    	ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x * 0.5f, work_pos.y + 25.0f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(20, 20, 100, 1));
        if (ImGui::Begin("##FF", NULL, window_flags)) 
        {     
            ImGui::SetWindowFontScale(1.2f);
            // Fast Forward Toggles    
            if (ImGui::RadioButton("x1", &fastForwardActive, 0)) { fastForward = 1; } ImGui::SameLine();
            if (ImGui::RadioButton("x10", &fastForwardActive, 1)) { fastForward = 10; } ImGui::SameLine();
            if (ImGui::RadioButton("x50", &fastForwardActive, 2)) { fastForward = 50; } ImGui::SameLine();
            if (ImGui::RadioButton("x100", &fastForwardActive, 3)) { fastForward = 100; } ImGui::SameLine();
            if (ImGui::RadioButton("x500", &fastForwardActive, 4)) { fastForward = 500; }
        } ImGui::End(); ImGui::PopStyleColor();
}


void UI::ButtonOverlay(bool &orbitReset, bool &showEnergyPlot, bool &trailReset)
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    static float fontScale = 1.5f;
    static float topPadding = 20.0f;
    // Reset Orbit Button // 
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 15.0f, work_pos.y + 35.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("BDReset", NULL, window_flags)) { 
        ImGui::SetWindowFontScale(fontScale);
        if (ImGui::Button("Reset Orbit")) { orbitReset = true; }  
    }ImGui::End(); 
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 15.0f, work_pos.y + 50.0f + topPadding), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);
    if (ImGui::Begin("TrailReset", NULL, window_flags)) {
        ImGui::SetWindowFontScale(fontScale);
        if (ImGui::Button("Reset Trail")) { trailReset = true; }
    }ImGui::End();
    // Center Cam Button
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 25.0f, work_pos.y + 85.0f+ topPadding), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    if (ImGui::Begin("CenterCam", NULL, window_flags)){
        ImGui::SetWindowFontScale(fontScale);
        if (ImGui::Button("Center Cam")) { m_CameraController->ResetCamera(); }
    } ImGui::End();
    // Show Plot CheckBox Button
    ImGui::SetNextWindowPos(ImVec2(work_pos.x + work_size.x - 7.0f, work_pos.y  + 125.0f + topPadding), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.15));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(255.0f, 255.0f, 255.0f, 0.5));
    if (ImGui::Begin("ShowPlot", NULL, window_flags))
    {
        ImGui::SetWindowFontScale(fontScale);
        ImGui::Text("Show Plot");
        ImGui::SameLine();
        if (ImGui::Checkbox("##ShowEnergy", &showEnergyPlot)) { }
    } ImGui::End(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
    
    
}


void UI::StatsOverlay()
{
    static int corner = 0;
    static float windowScale = 1.3;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1) {
        const float PAD = 15.0f;
        ImVec2 window_pos, window_pos_pivot;
        // if corner is 1, then window_pos.x = work_pos.x + work_size.x - PAD else it gets work_pos.x + PAD
        window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.02f); // Transparent background
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(189, 204, 242, 1));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
    ImGui::SetNextWindowSize(ImVec2(260, 220));
    if (ImGui::Begin("Stats", NULL, window_flags)) {
        // Set This Condition to prevent key interactiong with sim
        // Window Settings
        ImGui::SetWindowFontScale(windowScale);
        // Orbit Stats //
        ImGui::Text("Statistics");
        ImGui::Separator();
        ImGui::Text("r: %.4f AU", bodyOrbit->r);
        ImGui::Dummy(ImVec2(0.0,5.0f));
        ImGui::Text("v: %.2f AU/yr", bodyOrbit->v); ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(2.0f, 2.0f, 2.0f, 0.3f));
        if (ImGui::Button("- v")) { bodyOrbit->vx *= 0.9f; bodyOrbit->vy *= 0.9f; } ImGui::SameLine();
        if (ImGui::Button("v +")) { bodyOrbit->vx *= 1.1f; bodyOrbit->vy *= 1.1f; }
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0.0, 0.5f));
        ImGui::Text("(vx,vy): (%.2f, %.2f)", bodyOrbit->vx, bodyOrbit->vy);
        ImGui::Dummy(ImVec2(0.0, 5.0f));
        static int E_expCount = 0;
        static int KE_expCount = 0;
        static int PE_expCount = 0;
        if (bodyOrbit->E < 1) { 
            E_expCount = scientificMultCount(bodyOrbit->E); 
            ImGui::Text("E: %.4fe-%i", bodyOrbit->E * pow(10, E_expCount), E_expCount);
        }
        else{ 
            ImGui::Text("E: %.4f", bodyOrbit->E);
        }
        if (bodyOrbit->KE < 1) { 
            KE_expCount = scientificMultCount(bodyOrbit->KE);
            ImGui::Text("KE: %.4fe-%i", bodyOrbit->KE * pow(10, KE_expCount), KE_expCount);
        }
        else { 
            ImGui::Text("KE: %.4f", bodyOrbit->KE );
        }
        if (bodyOrbit->PE < 1) { 
            PE_expCount = scientificMultCount(bodyOrbit->PE);
            ImGui::Text("PE: %.4fe-%i", bodyOrbit->PE * pow(10, PE_expCount), PE_expCount);
        }
        else { 
            ImGui::Text("PE: %.4f", bodyOrbit->PE);
        }
        // Body MASS OUTPUT //
        ImGui::Dummy(ImVec2(0.0, 5.0f));
        static int M_expCount = 0;
        if (bodyOrbit->body->mass > 1) { 
            M_expCount = scientificDivCount(bodyOrbit->body->mass); 
            ImGui::Text("Body mass: %.1f+e%i Solar",bodyOrbit->body->mass/pow(10,M_expCount), M_expCount);
        } else { 
            M_expCount = scientificMultCount(bodyOrbit->body->mass); 
            ImGui::Text("Body mass: %.1f-e%i Solar", bodyOrbit->body->mass * pow(10, M_expCount), M_expCount);
        }
    } ImGui::End(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
}


void UI::InputsOverlay(bool &orbitReset)
{
    static float windowScale = 1.3;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.02f); // Transparent background
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.0f + 15.0f, work_pos.y + 250.0f), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(189, 204, 242, 1));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
    if (ImGui::Begin("Body Inputs", NULL, window_flags)) {
        // Set This Condition to prevent key interactiong with sim
        ImGui::SetWindowFontScale(windowScale);
        // INPUTS //
        ImGui::Text("Variables");
        ImGui::Separator();
        // Time Step Toggle // - ERASE FOR FINAL RELEASE
        ImGui::Text("dt: ");ImGui::SameLine(0.0f, 20.0f);ImGui::PushItemWidth(100.0f);
        if (ImGui::InputFloat("Yrs", TimeStep, 0.0f, 0.0f, "%.5f")) { orbitReset = true; }
        // Time Step constraints //
        if (*TimeStep > 0.01f) { *TimeStep = 0.01f; }
        if (*TimeStep <= 0.00001f) { *TimeStep = 0.00001f; }
        // Position Toggles //
        ImGui::Text("x0: "); ImGui::SameLine(0.0f, 20.0f); ImGui::PushItemWidth(100.0f);
        if (ImGui::InputFloat("##x0", &bodyOrbit->x0)) { orbitReset = true; }ImGui::SameLine(0.0f); ImGui::Text("AU");
        ImGui::Text("y0: "); ImGui::SameLine(0.0f, 20.0f); ImGui::PushItemWidth(100.0f);
        if (ImGui::InputFloat("##y0", &bodyOrbit->y0)) { orbitReset = true; }ImGui::SameLine(0.0f); ImGui::Text("AU");
        // Velocity Toggles //
        ImGui::Text("vx0: "); ImGui::SameLine(0.0f,12.0f); ImGui::PushItemWidth(100.0f);
        if (ImGui::InputFloat("##vx", &bodyOrbit->vx0)) { orbitReset = true; }ImGui::SameLine(0.0f); ImGui::Text("AU/Yr");
        ImGui::Text("vy0: "); ImGui::SameLine(0.0f, 12.0f); ImGui::PushItemWidth(100.0f);
        if (ImGui::InputFloat("##vy", &bodyOrbit->vy0)) { orbitReset = true; }ImGui::SameLine(0.0f); ImGui::Text("AU/Yr");
        // Star Mass
        ImGui::Text("Star Mass: "); ImGui::SameLine(); ImGui::PushItemWidth(70.0f);
        if (ImGui::InputFloat("##starMass", &bodyOrbit->starMass)) {
            if (bodyOrbit->starMass <= 0.0f) { bodyOrbit->starMass = 1.0f; }
        }ImGui::SameLine(0.0f); ImGui::Text("Solar");
        // Body Mass - in micro solar masses
        ImGui::Text("Body Mass: "); ImGui::SameLine(); ImGui::PushItemWidth(70.0f);
        if (ImGui::InputFloat("##bodyMass", &bodyMass)) {
            if (bodyMass <= 0.0f) { bodyMass = 1.0f; }
            bodyOrbit->body->mass = bodyMass * 1e-6f;
        }ImGui::SameLine(0.0f); ImGui::Text("Micro Solar");
        

    } ImGui::End(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
}

void UI::PauseMenu()
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
    ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
    // center the window
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.5f, (work_pos.y + work_size.y) * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Paused", NULL, window_flags)) {
        ImGui::SetWindowFontScale(3.0f);
        ImGui::Text("PAUSED");
    } ImGui::End();
}


void UI::CrashMenu()
{
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
    ImGui::SetNextWindowBgAlpha(0.0f); 
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.5f, (work_pos.y + work_size.y) * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Crashed", NULL, window_flags)) {
        ImGui::SetWindowFontScale(3.0f);
        ImGui::Text("     Crashed");
        ImGui::SetWindowFontScale(2.0f); ImGui::NewLine();
        ImGui::Text("Press 'Space Bar' to Reset");
    } ImGui::End();
}

void UI::EnergyPlot(bool &pauseUniverse)
{
    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoMenus;
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
    static RollingBuffer   rdata1, rdata2;
    if (!pauseUniverse){ // Add points To Data Buffer
        rdata1.AddPoint(*UniverseTime, bodyOrbit->KE);
        rdata2.AddPoint(*UniverseTime, bodyOrbit->PE);
    }

    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::SetNextWindowPos(ImVec2((work_pos.x + work_size.x) * 0.5f, (work_pos.y + work_size.y)), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
    ImGui::Begin("Plot Window", NULL, window_flags);
    if (ImPlot::BeginPlot("##RollingPlot", ImVec2(1000, 200)))
    {
        static float y_min = std::min(rdata1.Data[0].y, rdata2.Data[0].y) * 2.0f;
        static float y_max = std::max(rdata1.Data[0].y, rdata2.Data[0].y) * 2.0f;
        //Plot Setup
        ImPlot::SetupLegend(ImPlotLocation_SouthEast);
        ImPlot::SetupAxes("Time (Years)", "Energy (Ms*AU^2*yr^-2)", flags, flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, *UniverseTime, ImGuiCond_Always);
        // Determine realtime y limits
        if (abs(y_min) < y_max){y_min = y_max;} 
        else if (abs(y_min) > y_max){y_max = abs(y_min);}
        ImPlot::SetupAxisLimits(ImAxis_Y1, y_min, y_max);
        // Draw Scatter Plots
        ImPlot::PlotScatter("Kinetic Energy", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 3 * sizeof(float));
        ImPlot::PlotScatter("Potential Energy", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 3 * sizeof(float));

        ImPlot::EndPlot();
    } ImGui::End();
}


/////////////// NON-MEMBER HELPER FUNCTIONS /////////////


int scientificDivCount(float &og_num)
{   // number must be greater than 1
    float num = og_num;
    int count = 0;
    while ((static_cast<int>(num)) / 10 != 0)
    {
        num /= 10;
        count++;
    }
    return count;

}

int scientificMultCount(float& og_num)
{   // number must be less than 1
    float num = og_num;
    int count = 0;
    while (static_cast<int>(num) == 0)
    {
        num *= 10;
        count++;
    }
    return count;

}