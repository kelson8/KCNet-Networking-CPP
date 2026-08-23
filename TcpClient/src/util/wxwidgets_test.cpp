#include "wxwidgets_test.h"

#include "defines.h"
#include "urandom_test.h"

#include "tcp_client.h"
#include <fmt/format.h>

#include "curl_test.h"

#include <random>

#ifdef WXWIDGETS_GUI
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/stdpaths.h>
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
wxBEGIN_EVENT_TABLE(TcpClientFrame, wxFrame)
    EVT_MENU(Minimal_Quit, TcpClientFrame::OnQuit)
        EVT_MENU(Minimal_About, TcpClientFrame::OnAbout)
            wxEND_EVENT_TABLE()

    // wxBEGIN_EVENT_TABLE(TcpClientFrame, wxFrame)
    //     EVT_BUTTON(BUTTON_Send_Message, TcpClient::getInstance().ConnectToServer())
    // wxEND_EVENT_TABLE() // The button is pressed

    // Create a new application object: this macro will allow wxWidgets to create
    // the application object during program execution (it's better than using a
    // static object for many reasons) and also implements the accessor function
    // wxGetApp() which will return the reference of the right type (i.e. TcpClientApp and
    // not wxApp)
    wxIMPLEMENT_APP(TcpClientApp);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

/**
 * Dice rolling method
 * 
 * TODO Move this elsewhere, move this into KCNet-Library-CPP on GitHub later.
 * 
 * @returns The value of the dice roll.
 */
int DiceRoll()
{
    // Define range
    int min = 1;
    int max = 6;

    // Initialize a random number generator
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> distrib(min, max);

    // Generate random number in the range [min, max]
    int randomValue = distrib(gen);
    return randomValue;
}

// 'Main program' equivalent: the program execution "starts" here
bool TcpClientApp::OnInit()
{
    // Display the program starting message in the console.
    std::string runString = fmt::format("{} Running GUI for {} {}", LOG_PREFIX, PROGRAM_NAME, PROGRAM_VERSION);
    std::cout << runString << std::endl;

    // Very basic random number generator, just store a random number for later use.
    randomNumber = DiceRoll();

    if(!randomNumber == 0)
    {
        // Enable this to log the random number output
        // std::cout << "[DBG]: Dice roll result: " << randomNumber << std::endl;
    }
    else
    {
        std::cout << "[DBG]: Random number was 0 or not valid!" << std::endl;
        // Disable random number generator functions if I add any here.
    }

    // TODO Try to fix this to work
    // This should log if the GUI is enabled or not, if it isn't then it's using console mode.
    // https://stackoverflow.com/questions/13204177/how-to-find-out-if-running-from-terminal-or-gui
    // if (NULL == getenv("DISPLAY"))
    // std::cout << "Gui is not enabled." << std::endl;
    // else
    // std::cout << "Gui is enabled." << std::endl;

    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if (!wxApp::OnInit())
        return false;

    // create the main application window
    TcpClientFrame *frame = new TcpClientFrame(PROGRAM_NAME);

    // Add the logo for TcpClient
    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Applications/ApplicationIcon/ApplicationIcon.cpp
    frame->SetProgramLogo(frame);
    //

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
TcpClientFrame::TcpClientFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
      logoSetup(false)
{
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
    // aboutBtn->Bind(wxEVT_BUTTON, &TcpClientFrame::OnAbout, this);
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

    // Set the background and foreground color
    // https://docs.wxwidgets.org/latest/classwx_colour_database.html
    // panel1->SetBackgroundColour({wxTheColourDatabase->Find("Navy")});
    // panel1->SetBackgroundColour({wxTheColourDatabase->Find("Firebrick")});
    // panel1->SetForegroundColour();

    // TODO Make these buttons look a bit nicer on here.
    // TODO Make these buttons not resize and be giant with the screen.

    // dialogTestBtn = new wxButton(panel1, wxID_ANY, "Dialog test");
    // wxButton *dialogTestBtn = new wxButton(this, wxID_ANY, "Dialog test");
    // dialogTestBtn->Bind(wxEVT_BUTTON, &TcpClientFrame::OnDialogboxTest, this);

    // https://forums.wxwidgets.org/viewtopic.php?t=15526
    // wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    //

    //-------
    // TCPClient message sending to TcpServer.
    //-------

    if (!SetupTcpClientCommands(panel1))
    {
        fmt::print("SetupTcpClientCommands() didn't run correctly! Exiting Client.\n");
        exit(EXIT_FAILURE);
    }

#ifdef CURL_TEST
    // New for curl testing

    curlTestBtn = new wxButton(panel1, ID_CURL_TEST_BTN, "Curl Test");
    curlTestBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
    {
        CurlTest::getInstance().TestUrlRequest("https://kelsoncraft.net");
    });
#endif // CURL_TEST

    //-------
    // Text testing
    // https://docs.wxwidgets.org/stable/classwx_static_text.html
    // Moving this into the header fixes it, if I keep the pointer in this constructor it crashes.
    // text = new wxStaticText(panel1, ID_STATIC_TEXT_TEST1, "Test text");

    // This now works for updating the text on the screen, I could use this for messages sent from
    //  the server.
    // wxButton *updateTestTextBtn = new wxButton(panel1, ID_UPDATE_TEST_TEXT_BTN, "Update test text");
    // updateTestTextBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                            // { text->SetLabel(wxT("New test text")); });

    //------
    // Bitmap testing
    // Taken from here
    // https://github.com/bartoszwarzocha/wxwidgets_book_examples/blob/master/Hangman%20game/Common/main.cpp
    //------

    // wxStaticBitmap *bitmapTest = new wxStaticBitmap(panel1, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize,
    //     wxBORDER_SIMPLE);

    // // Create context...
    // int bitmapHeight = 10;
    // int bitmapWidth = 300;

    // wxBitmap bmp(wxSize(bitmapWidth, bitmapHeight));
    // wxMemoryDC dc(bmp);
    // dc.SetBackground(*wxWHITE_BRUSH);
    // dc.Clear();

    // bitmapTest->SetBitmap(bmp);

    //------
    // Text control
    // This looks like I can use this for text inputs.
    // TODO Make a limit for the text with this.
    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Others/DisplayInformations/DisplayInformations.cpp
    //------

    long textStyle = wxTE_MULTILINE | wxTE_READONLY;

    // Add a flag
    // textStyle |= wxTE_RICH2;
    // Remove a flag
    // textStyle &= ~wxTE_READONLY;

    // textControlTest = new wxTextCtrl(panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, textStyle);

    // textControlTest->AppendText(wxString::Format("Test number: %d", 2));
    // textControlTest->AppendText(wxString::Format("Test number: %d", 2));

    //------
    // Password hiding
    // TODO Fix this, it keeps crashing.
    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Others/TextEntryDialogPassword/TextEntryDialogPassword.cpp
    //------

    // passwordLabel->SetLabel(wxT("Test Password..."));
    // passwordEntry = new wxTextEntryDialog(this, "user: admin", "User password", passwordLabel->GetLabel(), wxTextEntryDialogStyle|wxTE_PASSWORD);
    // passwordEntry = new wxTextEntryDialog(this, "user: admin", "User password", "Test", wxTextEntryDialogStyle|wxTE_PASSWORD);

    //------
    // Align the buttons, text and other items
    //------

    // https://neume.sourceforge.net/sizerdemo/
    // This works for aligning multiple buttons onto the same section.
    // sizer->Add(tcpMessageChoice, 1, wxALIGN_CENTER);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_RIGHT, 0, 0);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_CENTER, 0, 0);
    // sizer->Add(dialogTestBtn, 1, wxALIGN_LEFT, 0, 0);

    if(tcpMessageChoice && currentChoiceBtn)
    {
        sizer->Add(tcpMessageChoice, 1, wxALIGN_LEFT);
        sizer->Add(currentChoiceBtn, 1, wxALIGN_LEFT);
    }

#ifdef CURL_TEST
    sizer->Add(curlTestBtn, 1, wxALIGN_RIGHT);
#endif // CURL_TEST

    // Future testing
    // sizer->Add(text, 1, wxALIGN_CENTER);
    // sizer->Add(updateTestTextBtn, 1, wxALIGN_CENTER);

    // sizer->Add(bitmapTest, 1, wxALIGN_RIGHT);
    // sizer->Add(textControlTest, 1, wxALIGN_RIGHT);

    // Required if using the window directly for this.
    // this->SetSizer(sizer);

    // Required if using the panel
    panel1->SetSizer(sizer);
    sizer->Show(true);

    // CenterOnScreen();
}

/**
 * Setup the client TCP commands message choices and send message button.
 * TODO Fix this to have better error handling.
 */
bool TcpClientFrame::SetupTcpClientCommands(wxPanel *panel)
{
    //-------
    // TCPClient message sending to TcpServer.
    //-------
    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Controls/Choice/Choice.cpp
    tcpMessageChoice = new wxChoice(panel, ID_MESSAGE_CHOICE);
    tcpMessageChoice->Append(LAST_COMMAND, std::vector<wxString>{
                                               "Send /dev/urandom generator Message", // 0
                                               "Reload command",                      // 1
                                               "Shutdown command"                     // 2
                                           }
                                               .data());

    tcpMessageChoice->SetSelection(0);
    tcpMessageChoice->Bind(wxEVT_CHOICE, &TcpClientFrame::OnChoiceClick, this);

    // https://github.com/gammasoft71/Examples_wxWidgets/blob/master/wxCore/Controls/Button/Button.cpp
    currentChoiceBtn = new wxButton(panel, ID_SEND_MESSAGE_BTN, "Send message");
    currentChoiceBtn->Bind(wxEVT_BUTTON, [&](wxCommandEvent &event)
                           {
        // Print out the current selected choice number, starting from 0.
        // std::cout << "Current choice selected: " << tcpMessageChoice->GetCurrentSelection() << std::endl; 
        TcpClient::getInstance().ConnectToServer(static_cast<ServerCommand>(tcpMessageChoice->GetCurrentSelection())); });

    // Some error handling for this
    if (!tcpMessageChoice || !currentChoiceBtn)
    {
        return false;
    }

    return true;
}

/**
 * Set the program logo
 */
void TcpClientFrame::SetProgramLogo(TcpClientFrame *frame)
{
    // Do nothing if logo is already set.
    if (logoSetup)
        return;

    wxImage::AddHandler(new wxPNGHandler);
    auto logoPath = wxFileName{wxStandardPaths::Get().GetExecutablePath()};
    // Path in folder directory: Resources/Logo.png
    logoPath.AppendDir("Resources");
    logoPath.SetFullName("KCNet-Networking-CPP");
    logoPath.SetExt("png");
    frame->SetIcon({logoPath.GetFullPath(), wxBITMAP_TYPE_PNG});

    logoSetup = true;
}

/**
 * Dialog box testing
 *
 * https://wiki.wxwidgets.org/WxMessageBox
 */
void TcpClientFrame::OnDialogboxTest(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox(wxT("Hello World!"));

    int i = 7;
    wxString Foobar;
    // Foobar.Printf(wxT("Hello I have %d cookies."), i);
    // wxMessageBox(Foobar);

    // wxMessageBox(wxT("This is the message."), wxT("This is the title"), wxICON_INFORMATION);
}

// event handlers

void TcpClientFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void TcpClientFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
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
