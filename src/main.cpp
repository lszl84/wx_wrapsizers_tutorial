#include <wx/wx.h>

#include <string>
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

wxPanel *MyFrame::BuildControlsPanel(wxWindow *parent)
{
    auto controlsPanel = new wxPanel(parent, wxID_ANY);

    bool isDark = wxSystemSettings::GetAppearance().IsDark();
    controlsPanel->SetBackgroundColour(wxColour(isDark ? darkBackground : lightBackground));

    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto text = new wxStaticText(controlsPanel, wxID_ANY, "Colors");
    mainSizer->Add(text, 0, wxALL, FromDIP(5));

    auto singleColorPane = new ColorPane(controlsPanel, wxID_ANY, wxColour(100, 100, 200));
    singleColorPane->selected = true;

    mainSizer->Add(singleColorPane, 0, wxALL, FromDIP(5));

    controlsPanel->SetSizer(mainSizer);

    return controlsPanel;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    BuildControlsPanel(this);
}