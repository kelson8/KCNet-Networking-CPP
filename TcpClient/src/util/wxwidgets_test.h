#pragma once

#include "defines.h"

#ifdef WXWIDGETS_GUI
#include <wx/wx.h>
#endif // WXWIDGETS_GUI

#ifdef WXWIDGETS_GUI

// Wxwidgets test
// https://www.binarytides.com/install-wxwidgets-ubuntu/

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class TcpClientApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() override;
    
private:
    // Will be a random number from 1-6 generated with the DiceRoll function.
    int randomNumber;
};

// Define a new frame type: this is going to be our main frame
class TcpClientFrame : public wxFrame
{
public:
    // ctor(s)
    TcpClientFrame(const wxString &title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    void OnDialogboxTest(wxCommandEvent &event);    


    // TODO Make this function private or something later.
    void SetProgramLogo(TcpClientFrame *frame);

private:
    // wxBoxSizer *boxSizerVerticalOne = new wxBoxSizer(wxVERTICAL);

    // This makes sure the program logo only gets set once.
    bool logoSetup;

    // wxButton *testBtn;
    wxButton *currentChoiceBtn;
    wxButton *dialogTestBtn;

    wxButton *curlTestBtn;

    wxChoice *tcpMessageChoice;

    wxStatusBar *statusBar;

    wxStaticText *text;
    // wxStaticText* passwordLabel;

    // wxTextEntryDialog *passwordEntry;

    // wxTextCtrl *textControlTest;

    bool SetupTcpClientCommands(wxPanel *panel);

    /**
     * Set the choice selections
     */
    void OnChoiceClick(wxCommandEvent &e)
    {
        tcpMessageChoice->SetSelection(static_cast<wxChoice *>(e.GetEventObject())->GetSelection());
    }

    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT,

    BUTTON_Send_Message = wxID_HIGHEST + 1 // declares an id which will be used to call our button
};

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
    ID_CLOSE = wxID_HIGHEST + 1,
    
    // ID_TESTBTN,
    ID_MESSAGE_CHOICE,
    ID_SEND_MESSAGE_BTN,
    // ID_CURRENT_CHOICE_BTN,
    ID_STATIC_TEXT_TEST1,
    ID_UPDATE_TEST_TEXT_BTN,
    ID_CURL_TEST_BTN,
};

#endif // WXWIDGETS_GUI
