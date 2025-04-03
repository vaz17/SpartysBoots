/**
 * @file Product.cpp
 *
 * @author Karan Choudhary
 */
 
#include "pch.h"
#include "Product.h"
#include "Game.h"
#include "Conveyor.h"

using namespace std;

const std::map<std::wstring, Product::Properties> Product::NamesToProperties = {
    {L"red", Product::Properties::Red},
    {L"green", Product::Properties::Green},
    {L"blue", Product::Properties::Blue},
    {L"white", Product::Properties::White},
    {L"square", Product::Properties::Square},
    {L"circle", Product::Properties::Circle},
    {L"diamond", Product::Properties::Diamond},
    {L"izzo", Product::Properties::Izzo},
    {L"smith", Product::Properties::Smith},
    {L"basketball", Product::Properties::Basketball},
    {L"football", Product::Properties::Football},
    {L"none", Product::Properties::None},
};

const std::map<Product::Properties, Product::Types> Product::PropertiesToTypes = {
    {Product::Properties::Red, Product::Types::Color},
    {Product::Properties::Green, Product::Types::Color},
    {Product::Properties::Blue, Product::Types::Color},
    {Product::Properties::White, Product::Types::Color},
    {Product::Properties::Square, Product::Types::Shape},
    {Product::Properties::Circle, Product::Types::Shape},
    {Product::Properties::Diamond, Product::Types::Shape},
    {Product::Properties::Izzo, Product::Types::Content},
    {Product::Properties::Smith, Product::Types::Content},
    {Product::Properties::Football, Product::Types::Content},
    {Product::Properties::Basketball, Product::Types::Content},
    {Product::Properties::None, Product::Types::Content}
};

const std::map<Product::Properties, std::wstring> Product::PropertiesToContentImages = {
    {Product::Properties::Izzo, L"izzo.png"},
    {Product::Properties::Smith, L"smith.png"},
    {Product::Properties::Football, L"football.png"},
    {Product::Properties::Basketball, L"basketball.png"}
};

/// Default product size in pixels.
std::wstring ProductDefaultSize = L"80";

/// Default product size as double.
const double ProductDefaultSizeDouble = 80.0;

/// Size to draw content relative to the product size
double ContentScale = 0.8;

/**
 * @return Color to use for "red"
 */
const wxColour OhioStateRed(187, 0, 0);

/**
 * @return Color to use for "green"
 */
const wxColour MSUGreen(24, 69, 59);

/**
 * @return Color to use for "blue"
 */
const wxColor UofMBlue(0, 39, 76);

/// Delay after last product has left beam or
/// been kicked before we end the level.
const double LastProductDelay = 3;


/**
 * Constructor
 * @param game pointer towards game
 */

Product::Product(Game* game) : Item(game)
{
}


/**
 * Constructor with the implementation of filname
 * @param game pointer towards game
 * @param filename filename for image of the product
 */
Product::Product(Game* game, const std::wstring& filename) : Item(game, filename)
{

}


/**
 * Draw the product on the conveyor
 * @param graphics graphics contezt where to draw on
 */
void Product::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (!graphics)
        return;


    graphics->PushState();

    graphics->Translate(GetX(), GetY());

    double size = std::stod(ProductDefaultSize);
    wxBrush brush;
    wxPen pen(*wxBLACK, 2);
    for (auto prop : mProperties)
    {
        if (PropertiesToTypes.at(prop) == Types::Color)
        {
            switch (prop)
            {
            case Properties::Red:
                brush.SetColour(OhioStateRed);
                break;
            case Properties::Green:
                brush.SetColour(MSUGreen);
                break;
            case Properties::Blue:
                brush.SetColour(UofMBlue);
                break;
            default:
                brush.SetColour(*wxWHITE);
                break;
            }
            break;
        }
    }

    graphics->SetBrush(brush);
    graphics->SetPen(pen);


    for (auto prop : mProperties)
    {
        if (PropertiesToTypes.at(prop) == Types::Shape)
        {
            switch (prop)
            {
            case Properties::Square:
                graphics->DrawRectangle(-size/2, -size/2, size, size);
                break;
            case Properties::Circle:
                graphics->DrawEllipse(-size/2, -size/2, size, size);
                break;
            case Properties::Diamond:
                {
                    wxPoint2DDouble points[4] = {
                        wxPoint2DDouble(0, -size/2),
                        wxPoint2DDouble(size/2, 0),
                        wxPoint2DDouble(0, size/2),
                        wxPoint2DDouble(-size/2, 0)
                    };
                    graphics->DrawLines(4, points);
                }
                break;
            default:
                break;
            }
            break;
        }
    }
    for (auto prop : mProperties)
    {
        if (PropertiesToTypes.at(prop) == Types::Content && prop != Properties::None)
        {
            wxString filename = PropertiesToContentImages.at(prop);
            wxImage img(L"images/" + filename, wxBITMAP_TYPE_ANY);
            if (img.IsOk())
            {
                int dim = wxRound(size * ContentScale);
                img.Rescale(dim, dim);
                wxBitmap bmp(img);
                graphics->DrawBitmap(bmp, wxDouble(-dim/2), wxDouble(-dim/2), wxDouble(dim), wxDouble(dim));
            }
            break;
        }
    }

    graphics->PopState();
}



/**
 * See if a point touches the prouct
 * @param x X-coordinate that we are testing
 * @param y Y-coordinate that we are testing
 * @return true if hit otherwise its false
 */
bool Product::HitTest(double x, double y)
{
    double size = ProductDefaultSizeDouble;  // Using the default size
    double halfSize = size / 2;

    // Calculate the relative position of the test point to the product's location
    double testX = x - GetX();
    double testY = y - GetY();

    // Check if the point is within the bounding box of the product
    return (abs(testX) <= halfSize && abs(testY) <= halfSize);
}


/**
 * Loads the product from xml into program
 * @param node xml node we are loading from
 */
void Product::XmlLoad(wxXmlNode* node)
{
    mKick = (node->GetAttribute(L"kick", L"no") == L"yes");

    wxString shape = node->GetAttribute(L"shape", L"");
    wxString color = node->GetAttribute(L"color", L"");
    wxString content = node->GetAttribute(L"content", L"");

    if (!shape.empty() && NamesToProperties.find(shape.ToStdWstring()) != NamesToProperties.end())
        mProperties.push_back(NamesToProperties.at(shape.ToStdWstring()));
    if (!color.empty() && NamesToProperties.find(color.ToStdWstring()) != NamesToProperties.end())
        mProperties.push_back(NamesToProperties.at(color.ToStdWstring()));
    if (!content.empty() && NamesToProperties.find(content.ToStdWstring()) != NamesToProperties.end())
        mProperties.push_back(NamesToProperties.at(content.ToStdWstring()));
}


/**
 * Updating the currents tate of the product
 * @param elapsed how much time has passed since previous update
 */
void Product::Update(double elapsed)
{
    SetLocation(GetX() - mKickSpeed * elapsed, GetY());
}


/**
 * Resets the products to deafult state
 */
void Product::Reset()
{
    SetLocation(mInitialPlacementX, mInitialPlacementY);
    mWasKicked = false;
    mKickSpeed = 0;
}


/**
 * Handles the clicking on a product (not really used)
 * @param x X-coordinate of the clicking
 * @param y Y-coordinate of the clicking
 */
void Product::OnClick(double x, double y)
{
    // Update this later (pleaceholder for now)
    mKick = !mKick;
}


/**
 * You get the sizeo of the prodcut here
 *
 * @return It returns the deafult size of the product
 */
double Product::GetSize() const
{
    return ProductDefaultSizeDouble;
}

void Product::SetInitalPosition(double x, double y)
{
    mInitialPlacementX = x;
    mInitialPlacementY = y;
}

void Product::MovePosition(double x, double y)
{
    if (mKickSpeed == 0)
    {
        SetLocation(GetX() + x, GetY() + y);
    }

}


/**
 * IT kicks the product and boots it out of the conveyor
 *
 * @param kick speed of the kick from the sparty
 */
void Product::Kick(double kick)
{
    mKickSpeed = kick;
    mWasKicked = true;
    // If the product was kicked but shouldn't have been, give bad score.
    if (!mKick)
    {
        GetGame()->GetScore()->SetLevelScoreBad();
        mScoreUpdated = true; // Mark product score as updated so it doesn't happen more than once.
    }
    else
    {
        GetGame()->GetScore()->SetLevelScoreGood();
        mScoreUpdated = true;
    }
}
