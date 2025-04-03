/**
* @file MainFrame.h
 * @author matthew vazquez
 *
 * The main frame of the application
 */

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

/**
 * The main frame class for the application
 */
class MainFrame : public wxFrame
{
private:


public:
 void Initialize();
 void OnExit(wxCommandEvent& event);
 void OnAbout(wxCommandEvent& event);
};

#endif //MAINFRAME_H