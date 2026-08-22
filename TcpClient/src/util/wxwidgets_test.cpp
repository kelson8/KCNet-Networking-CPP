#include "wxwidgets_test.h"

#include "defines.h"
#include "urandom_test.h"

#include "tcp_client.h"

#ifdef WXWIDGETS_GUI
#include <wx/wx.h>
// #include "logo.h"
#endif // WXWIDGETS_GUI

// When this is enabled, it disables 'int main' in main.cpp and switches this to the new main file.
// TODO Use this later.

// Credit to wxwidgets for this base file
// https://github.com/wxWidgets/wxWidgets/blob/master/samples/minimal/minimal.cpp

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#ifdef WXWIDGETS_GUI
// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
// #ifndef wxHAS_IMAGES_IN_RESOURCES
// #include "../sample.xpm"
// #endif

#endif // WXWIDGETS_GUI

#ifdef WXWIDGETS_GUI

// Wxwidgets test
// https://www.binarytides.com/install-wxwidgets-ubuntu/

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit, MyFrame::OnQuit)
        EVT_MENU(Minimal_About, MyFrame::OnAbout)
            wxEND_EVENT_TABLE()

    // wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    //     EVT_BUTTON(BUTTON_Send_Message, TcpClient::getInstance().ConnectToServer())
    // wxEND_EVENT_TABLE() // The button is pressed

    // Create a new application object: this macro will allow wxWidgets to create
    // the application object during program execution (it's better than using a
    // static object for many reasons) and also implements the accessor function
    // wxGetApp() which will return the reference of the right type (i.e. MyApp and
    // not wxApp)
    wxIMPLEMENT_APP(MyApp);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if (!wxApp::OnInit())
        return false;

    // create the main application window
    // MyFrame *frame = new MyFrame("Minimal wxWidgets App");
    MyFrame *frame = new MyFrame(PROGRAM_NAME);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
// Disabled window resizing for now
// https://forums.wxwidgets.org/viewtopic.php?t=6349
MyFrame::MyFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    // set the frame icon
    // SetIcon(wxICON(sample));
    // SetIcon(wxICON(KELSONCRAFT_LOGO));

    // Set the window size
    SetClientSize(WINDOW_WIDTH, WINDOW_HEIGHT);

#if wxUSE_MENUBAR
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#else  // !wxUSE_MENUBAR
    // If menus are not available add a button to access the about box

    // wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    // wxButton *aboutBtn = new wxButton(this, wxID_ANY, "About...");
    // aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
    // sizer->Add(aboutBtn, wxSizerFlags().Center());
    // SetSizer(sizer);
#endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

#if wxUSE_STATUSBAR

    // Create a status bar (by default with 1 pane only)
    statusBar = CreateStatusBar(2);
    // statusBar->SetSize(wxSize(100, 22));
    // statusBar->SetScrollbar(wxVERTICAL, wxALIGN_RIGHT, true, 20, true);

    statusBar->SetStatusText("Welcome to " + std::string(PROGRAM_NAME));

    // TODO Figure out how to add more text to this, this gets cut off.
    // statusBar->SetStatusText("Welcome to " + std::string(PROGRAM_NAME) + " " + std::string(PROGRAM_VERSION));
#endif // wxUSE_STATUSBAR

    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/HelloWorlds/HelloWorldSay/HelloWorldSay.cpp
    // Panel testing
    // If the buttons have panel1 set, the sizer has to use panel1 instead of 'this'
    wxPanel *panel1 = new wxPanel(this, wxID_ANY);

    // TODO Make these buttons look a bit nicer on here.
    // TODO Make these buttons not resize and be giant with the screen.

    // dialogTestBtn = new wxButton(panel1, wxID_ANY, "Dialog test");
    // wxButton *dialogTestBtn = new wxButton(this, wxID_ANY, "Dialog test");
    // dialogTestBtn->Bind(wxEVT_BUTTON, &MyFrame::OnDialogboxTest, this);

    // https://forums.wxwidgets.org/viewtopic.php?t=15526
    // wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    //

    //-------
    // TCPClient message sending to TcpServer.
    //-------
    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Controls/Choice/Choice.cpp
    tcpMessageChoice = new wxChoice(panel1, ID_MESSAGE_CHOICE);
    tcpMessageChoice->Append(LAST_COMMAND, std::vector<wxString>{
                           "Send /dev/urandom generator Message", // 0
                           "Reload command",                      // 1
                           "Shutdown command"                     // 2
                       }
                           .data());

    tcpMessageChoice->SetSelection(0);
    tcpMessageChoice->Bind(wxEVT_CHOICE, &MyFrame::OnChoiceClick, this);

    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Controls/Button/Button.cpp
    currentChoiceBtn = new wxButton(panel1, ID_SEND_MESSAGE_BTN, "Send message");
    currentChoiceBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                           {
        // Print out the current selected choice number, starting from 0.
        // std::cout << "Current choice selected: " << tcpMessageChoice->GetCurrentSelection() << std::endl; 
        TcpClient::getInstance().ConnectToServer(static_cast<ServerCommand>(tcpMessageChoice->GetCurrentSelection()));
    });

    // Text testing
    // https://docs.wxwidgets.org/stable/classwx_static_text.html
    // wxStaticText *text = new wxStaticText(panel1, ID_STATIC_TEXT_TEST1, "Test text");

    // TODO Fix this, why does it crash?
    // wxButton *updateTestTextBtn = new wxButton(panel1, ID_UPDATE_TEST_TEXT_BTN, "Update test text");
    // updateTestTextBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
    //                         { 
    //                             // text->SetLabel(wxString("New test text")); 
    //                             text->SetLabel(wxT("New test text")); 
    //                         });

    //------
    // Align the buttons, text and other items
    //------

    // https://neume.sourceforge.net/sizerdemo/
    // This works for aligning multiple buttons onto the same section.
    // sizer->Add(tcpMessageChoice, 1, wxALIGN_CENTER);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_RIGHT, 0, 0);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_CENTER, 0, 0);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_LEFT, 0, 0);
    sizer->Add(tcpMessageChoice, 1, wxALIGN_LEFT);
    sizer->Add(currentChoiceBtn, 1, wxALIGN_LEFT);

    // Future testing
    // sizer->Add(text, 1, wxALIGN_CENTER);
    // sizer->Add(updateTestTextBtn, 1, wxALIGN_CENTER);

    // Required if using the window directly for this.
    // this->SetSizer(sizer);

    // Required if using the panel
    panel1->SetSizer(sizer);
}

/**
 * Dialog box testing
 *
 * https://wiki.wxwidgets.org/WxMessageBox
 */
void MyFrame::OnDialogboxTest(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox(wxT("Hello World!"));

    int i = 7;
    wxString Foobar;
    // Foobar.Printf(wxT("Hello I have %d cookies."), i);
    // wxMessageBox(Foobar);

    // wxMessageBox(wxT("This is the message."), wxT("This is the title"), wxICON_INFORMATION);
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
                     "Welcome to %s!\n"
                     "Running version: %s"
                     "\n"
                     "This is the TcpClient for KCNet-Networking-CPP\n"
                     "Running under %s.",
                     PROGRAM_NAME,
                     PROGRAM_VERSION,
                     wxGetOsDescription()),
                 "About TcpClient",
                 wxOK | wxICON_INFORMATION,
                 this);
}

#endif // WXWIDGETS_GUI
