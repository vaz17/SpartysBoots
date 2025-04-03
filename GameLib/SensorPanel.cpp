/**
 * @file SensorPanel.cpp
 * @author Silas Hodges
 */

#include "pch.h"
#include "SensorPanel.h"
#include "Game.h"
#include "Sensor.h"
#include "Gates.h"
#include <algorithm>

using namespace std;

/// Size of Property on panel in virtual pixels.
const wxSize SensorPanel::PropertySize = wxSize(100, 40);

/// Offset used to draw the Outputpin
const double OutputPinOffset = 1;

/**
 * Constructor for SensorPanel
 * @param game Pointer to game instance
 * @param property Name of property
 * @param x X-coordinate of panel center
 * @param y Y-coordinate of panel center
 */
SensorPanel::SensorPanel(Game* game, const wstring& property, double x, double y)
 : Item(game), mProperty(property), mX(x), mY(y)
{
    wxPoint pinLocation(x + PropertySize.GetWidth() / 2 + OutputPinOffset, y);
	   mOutputPin = make_shared<OutputPin>(this, pinLocation);
}

/**
 * Draws property panel and OutputPin
 * @param graphics Graphics context to draw on
 */
void SensorPanel::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw the property box
    double rectX = mX - PropertySize.GetWidth() / 2;
    double rectY = mY - PropertySize.GetHeight() / 2;
    double rectWidth = PropertySize.GetWidth();
    double rectHeight = PropertySize.GetHeight();

    double padding = 5.0;

    // Color properties
    if (mProperty == L"red" || mProperty == L"green" || mProperty == L"blue" || mProperty == L"white")
    {
        wxColour color;
        if (mProperty == L"red")
        {
            color = wxColour(187, 0, 0);
        }
        else if (mProperty == L"green")
        {
            color = wxColour(24, 69, 59);
        }
        else if (mProperty == L"blue")
        {
            color = wxColour(0, 39, 76);
        }
        else if (mProperty == L"white")
        {
            color = *wxWHITE;
        }

        graphics->SetBrush(wxBrush(color));
        graphics->SetPen(*wxBLACK_PEN);
        graphics->DrawRectangle(rectX, rectY, rectWidth, rectHeight);
    }
    else if (mProperty == L"square" || mProperty == L"circle")
    {
        graphics->SetPen(*wxBLACK_PEN);
        graphics->SetBrush(*wxWHITE_BRUSH);

        double shapeSize = rectHeight - 2 * padding;

        double shapeX = rectX + (rectWidth - shapeSize) / 2;
        double shapeY = rectY + (rectHeight - shapeSize) / 2;

        if (mProperty == L"square")
        {
            graphics->DrawRectangle(shapeX, shapeY, shapeSize, shapeSize);
        }
        else
        {
            graphics->DrawEllipse(shapeX, shapeY, shapeSize, shapeSize);
        }

        graphics->SetPen(*wxBLACK_PEN);
        graphics->SetBrush(*wxTRANSPARENT_BRUSH);
        graphics->DrawRectangle(rectX, rectY, rectWidth, rectHeight);
    }
    else
    {
        // Image properties
        std::wstring imagePath = L"images/" + mProperty + L".png";
        wxImage image;
        if (image.LoadFile(imagePath, wxBITMAP_TYPE_PNG))
        {
            wxBitmap bitmap(image);
            double imageWidth = bitmap.GetWidth();
            double imageHeight = bitmap.GetHeight();

            double availableWidth = rectWidth - 2 * padding;
            double availableHeight = rectHeight - 2 * padding;
            double scaleX = availableWidth / imageWidth;
            double scaleY = availableHeight / imageHeight;
            double scale = std::min(scaleX, scaleY);

            double imageDisplayWidth = imageWidth * scale;
            double imageDisplayHeight = imageHeight * scale;
            double offsetX = rectX + padding + (availableWidth - imageDisplayWidth) / 2;
            double offsetY = rectY + padding + (availableHeight - imageDisplayHeight) / 2;

            graphics->PushState();
            graphics->Translate(offsetX, offsetY);
            graphics->Scale(scale, scale);
            graphics->DrawBitmap(bitmap, 0, 0, imageWidth, imageHeight);
            graphics->PopState();

            graphics->SetPen(*wxBLACK_PEN);
            graphics->SetBrush(*wxTRANSPARENT_BRUSH);
            graphics->DrawRectangle(rectX, rectY, rectWidth, rectHeight);
        }
    }

    // Draw the OutputPin
    if (mOutputPin)
    {
        mOutputPin->Draw(graphics);
    }
}

/**
 * Updates the OutputPin state based on detected properties
 * @param detectedProperties Vector of detected properties
 */
void SensorPanel::UpdateState(const std::vector<Product::Properties>& detectedProperties)
{
    // Map property name to enum
    static const std::unordered_map<std::wstring, Product::Properties> NameToPropertyMap = {
        {L"red", Product::Properties::Red},
        {L"green", Product::Properties::Green},
        {L"blue", Product::Properties::Blue},
        {L"white", Product::Properties::White},
        {L"square", Product::Properties::Square},
        {L"circle", Product::Properties::Circle},
        {L"diamond", Product::Properties::Diamond},
        {L"izzo", Product::Properties::Izzo},
        {L"smith", Product::Properties::Smith},
        {L"football", Product::Properties::Football},
        {L"basketball", Product::Properties::Basketball}
    };

    auto it = NameToPropertyMap.find(mProperty);
    if (it != NameToPropertyMap.end())
    {
        Product::Properties propEnum = it->second;
        bool isActive = (std::find(detectedProperties.begin(), detectedProperties.end(), propEnum) != detectedProperties.end());

        if (mOutputPin)
        {
            mOutputPin->SetState(isActive ? States::One : States::Zero);
        }
    }
    mOutputPin->Update();
}

/**
 * Handles mouse click events on sensor
 * @param x X-coordinate of click
 * @param y Y-coordinate of click
 */
void SensorPanel::OnClick(double x, double y)
{
    wxLogMessage(L"Sensor clicked at %f, %f)", x, y);
}

