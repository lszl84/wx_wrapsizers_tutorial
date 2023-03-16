#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxWindow(parent, id, pos, size)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);

    BuildContextMenu();

    this->Bind(wxEVT_CONTEXT_MENU, &DrawingCanvas::OnContextMenuEvent, this);
}

void DrawingCanvas::OnPaint(wxPaintEvent &)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if (gc)
    {
        for (const auto &pointsVector : squiggles)
        {
            if (pointsVector.size() > 1)
            {
                gc->SetPen(wxPen(
                    *wxBLACK,
                    this->FromDIP(8)));
                gc->StrokeLines(pointsVector.size(), pointsVector.data());
            }
        }

        delete gc;
    }
}

void DrawingCanvas::OnMouseDown(wxMouseEvent &)
{
    squiggles.push_back({});
    isDrawing = true;
}

void DrawingCanvas::OnMouseMove(wxMouseEvent &event)
{
    if (isDrawing)
    {
        auto pt = event.GetPosition();
        auto &currentSquiggle = squiggles.back();

        currentSquiggle.push_back(pt);
        Refresh();
    }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent &)
{
    isDrawing = false;
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &)
{
    isDrawing = false;
}

void DrawingCanvas::BuildContextMenu()
{
    auto clear = contextMenu.Append(wxID_ANY, "&Clear");
    contextMenu.Append(wxID_ANY, "Save &As...");

    this->Bind(
        wxEVT_MENU,
        [this](wxCommandEvent &)
        {
            this->squiggles.clear();
            this->Refresh();
        },
        clear->GetId());
}

void DrawingCanvas::OnContextMenuEvent(wxContextMenuEvent &e)
{
    auto clientPos = e.GetPosition() == wxDefaultPosition
                         ? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
                         : this->ScreenToClient(e.GetPosition());
    PopupMenu(&this->contextMenu, clientPos);
}