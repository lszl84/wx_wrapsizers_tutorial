#include <wx/wx.h>
#include <wx/wrapsizer.h>

#include <string>
#include <vector>

#include "colorpane.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxPanel *BuildControlsPanel(wxWindow *parent);

    void SetupColorPanes(wxWindow *parent, wxSizer *sizer);

    std::vector<ColorPane *> colorPanes{};

    const std::vector<std::string> niceColors = {"#000000", "#ffffff", "#fd7f6f",
                                                 "#7eb0d5", "#b2e061", "#bd7ebe",
                                                 "#ffb55a", "#ffee65", "#beb9db",
                                                 "#fdcce5", "#8bd3c7"};

    const std::string lightBackground = "#f4f3f3";
    const std::string darkBackground = "#2c2828";
};

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

void MyFrame::SetupColorPanes(wxWindow *parent, wxSizer *sizer)
{
    for (const auto &color : niceColors)
    {
        auto colorPane = new ColorPane(parent, wxID_ANY, wxColour(color));

        colorPanes.push_back(colorPane);
        sizer->Add(colorPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
    }
}

wxPanel *MyFrame::BuildControlsPanel(wxWindow *parent)
{
    auto controlsPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
    controlsPanel->SetScrollRate(0, FromDIP(10));

    bool isDark = wxSystemSettings::GetAppearance().IsDark();
    controlsPanel->SetBackgroundColour(wxColour(isDark ? darkBackground : lightBackground));

    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto text = new wxStaticText(controlsPanel, wxID_ANY, "Colors");
    mainSizer->Add(text, 0, wxALL, FromDIP(5));

    auto colorPaneSizer = new wxWrapSizer(wxHORIZONTAL);
    SetupColorPanes(controlsPanel, colorPaneSizer);

    mainSizer->Add(colorPaneSizer, 0, wxALL, FromDIP(5));

    auto button = new wxButton(controlsPanel, wxID_ANY, "Save As...");

    mainSizer->AddStretchSpacer();
    mainSizer->Add(button, 0, wxALL, FromDIP(5));
    mainSizer->AddSpacer(FromDIP(5));

    controlsPanel->SetSizer(mainSizer);

    return controlsPanel;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    BuildControlsPanel(this);
}