#pragma once

#include <wx/wx.h>
#include <vector>

#include "path.h"

class DrawingCanvas : public wxWindow
{
public:
    DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    virtual ~DrawingCanvas() noexcept {}

    void ShowSaveDialog();

    int currentWidth{1};
    wxColour currentColor{*wxBLACK};

private:
    void OnPaint(wxPaintEvent &);
    void DrawOnContext(wxGraphicsContext *gc);

    void OnMouseDown(wxMouseEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    void OnMouseLeave(wxMouseEvent &);

    bool isDrawing{};
    std::vector<Path> squiggles;

    wxMenu contextMenu;
    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent &);
};