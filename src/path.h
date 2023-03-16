#pragma once

#include <wx/wx.h>

#include <vector>

struct Path
{
    std::vector<wxPoint2DDouble> points;
    wxColour color;
    int width;
};