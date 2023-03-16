#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>

#include <string>
#include <vector>

#include "colorpane.h"
#include "pensizepane.h"

#include "drawingcanvas.h"

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
    void SetupPenPanes(wxWindow *parent, wxSizer *sizer);

    void SelectColorPane(ColorPane *pane);
    void SelectPenPane(PenSizePane *pane);

    std::vector<ColorPane *> colorPanes{};
    std::vector<PenSizePane *> penPanes{};

    DrawingCanvas *canvas;

    const std::vector<std::string> niceColors = {"#000000", "#ffffff", "#fd7f6f",
                                                 "#7eb0d5", "#b2e061", "#bd7ebe",
                                                 "#ffb55a", "#ffee65", "#beb9db",
                                                 "#fdcce5", "#8bd3c7"};

    const int penCount = 6;

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

        colorPane->Bind(wxEVT_LEFT_DOWN, [this, colorPane](wxMouseEvent &event)
                        { SelectColorPane(colorPane); });

        colorPanes.push_back(colorPane);
        sizer->Add(colorPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
    }
}

void MyFrame::SetupPenPanes(wxWindow *parent, wxSizer *sizer)
{
    for (int i = 0; i < penCount; i++)
    {
        auto penPane = new PenSizePane(parent, wxID_ANY, i * FromDIP(4) + FromDIP(1));

        penPane->Bind(wxEVT_LEFT_DOWN, [this, penPane](wxMouseEvent &event)
                      { SelectPenPane(penPane); });

        penPanes.push_back(penPane);
        sizer->Add(penPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
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

    text = new wxStaticText(controlsPanel, wxID_ANY, "Pens");
    mainSizer->Add(text, 0, wxALL, FromDIP(5));

    auto penPaneSizer = new wxWrapSizer(wxHORIZONTAL);
    SetupPenPanes(controlsPanel, penPaneSizer);
    mainSizer->Add(penPaneSizer, 0, wxALL, FromDIP(5));

    auto button = new wxButton(controlsPanel, wxID_ANY, "Save As...");

    mainSizer->AddStretchSpacer();
    mainSizer->Add(button, 0, wxALL, FromDIP(5));
    mainSizer->AddSpacer(FromDIP(5));

    controlsPanel->SetSizer(mainSizer);

    button->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                 { canvas->ShowSaveDialog(); });

    return controlsPanel;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);

    splitter->SetMinimumPaneSize(FromDIP(150));

    auto controlsPanel = BuildControlsPanel(splitter);
    canvas = new DrawingCanvas(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    splitter->SplitVertically(controlsPanel, canvas);
    splitter->SetSashPosition(FromDIP(220));

    this->SetSize(FromDIP(800), FromDIP(500));
    this->SetMinSize({FromDIP(400), FromDIP(200)});

    SelectColorPane(colorPanes[0]);
    SelectPenPane(penPanes[0]);
}

void MyFrame::SelectColorPane(ColorPane *pane)
{
    for (auto colorPane : colorPanes)
    {
        colorPane->selected = (colorPane == pane);
        colorPane->Refresh();
    }

    canvas->currentColor = pane->color;
}

void MyFrame::SelectPenPane(PenSizePane *pane)
{
    for (auto penPane : penPanes)
    {
        penPane->selected = (penPane == pane);
        penPane->Refresh();
    }

    canvas->currentWidth = pane->penWidth;
}