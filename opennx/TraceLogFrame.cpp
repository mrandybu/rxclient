/////////////////////////////////////////////////////////////////////////////
// Name:        TraceLogFrame.cpp
// Purpose:     
// Author:      Fritz Elfert
// Modified by: 
// Created:     19/10/2008 02:51:47
// RCS-ID:      
// Copyright:   (C) 2008 by Fritz Elfert
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <wx/textfile.h>

#include "TraceLogFrame.h"
#include "Icon.h"
#include "tracelogApp.h"

////@begin XPM images
////@end XPM images


/*!
 * TraceLogFrame type definition
 */

IMPLEMENT_CLASS( TraceLogFrame, wxFrame )


/*!
 * TraceLogFrame event table definition
 */

BEGIN_EVENT_TABLE( TraceLogFrame, wxFrame )

////@begin TraceLogFrame event table entries
    EVT_CLOSE( TraceLogFrame::OnCloseWindow )

    EVT_MENU( wxID_CLOSE, TraceLogFrame::OnCLOSEClick )

    EVT_MENU( wxID_SAVEAS, TraceLogFrame::OnSAVEASClick )

    EVT_MENU( wxID_EXIT, TraceLogFrame::OnEXITClick )

////@end TraceLogFrame event table entries

END_EVENT_TABLE()


/*!
 * TraceLogFrame constructors
 */

TraceLogFrame::TraceLogFrame()
{
    Init();
}

TraceLogFrame::TraceLogFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*!
 * TraceLogFrame creator
 */

bool TraceLogFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin TraceLogFrame creation
    SetParent(parent);
    CreateControls();
    SetIcon(GetIconResource(wxT("res/nx.png")));
    Centre();
////@end TraceLogFrame creation
    return true;
}


/*!
 * TraceLogFrame destructor
 */

TraceLogFrame::~TraceLogFrame()
{
////@begin TraceLogFrame destruction
////@end TraceLogFrame destruction
}


/*!
 * Member initialisation
 */

void TraceLogFrame::Init()
{
////@begin TraceLogFrame member initialisation
    m_pCtrlTraceLog = NULL;
////@end TraceLogFrame member initialisation
}


/*!
 * Control creation for TraceLogFrame
 */

void TraceLogFrame::CreateControls()
{    
////@begin TraceLogFrame content construction
    if (!wxXmlResource::Get()->LoadFrame(this, GetParent(), wxT("ID_TRACELOGFRAME")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlTraceLog = XRCCTRL(*this, "ID_LISTCTRL_TRACELOG", wxListCtrl);
////@end TraceLogFrame content construction

    // Create custom windows not generated automatically here.
////@begin TraceLogFrame content initialisation
////@end TraceLogFrame content initialisation

    m_pCtrlTraceLog->InsertColumn(0, _("Time"));
    m_pCtrlTraceLog->InsertColumn(1, _("PID"), wxLIST_FORMAT_RIGHT);
    m_pCtrlTraceLog->InsertColumn(2, _("Text"));
}


void TraceLogFrame::AddEntry(const wxDateTime &stamp, int pid, const wxString &txt)
{
    wxListItem i;
    i.SetColumn(0);
    i.SetText(stamp.Format(wxT("%c")));
    i.SetId(m_pCtrlTraceLog->GetItemCount());
    long idx = m_pCtrlTraceLog->InsertItem(i);
    i.SetId(idx);
    i.SetColumn(1);
    i.SetText(wxString::Format(wxT("%d"), pid));
    m_pCtrlTraceLog->SetItem(i);
    i.SetColumn(2);
    i.SetText(txt);
    m_pCtrlTraceLog->SetItem(i);
}

/*!
 * Should we show tooltips?
 */

bool TraceLogFrame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap TraceLogFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin TraceLogFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end TraceLogFrame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TraceLogFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin TraceLogFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end TraceLogFrame icon retrieval
}




/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_TRACELOGFRAME
 */

void TraceLogFrame::OnCloseWindow( wxCloseEvent& event )
{
    if (event.CanVeto()) {
        Show(false);
        event.Veto();
    } else {
        Destroy();
        event.Skip();
    }
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
 */

void TraceLogFrame::OnCLOSEClick( wxCommandEvent& event )
{
    Close(false);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
 */

void TraceLogFrame::OnEXITClick( wxCommandEvent& event )
{
    Close(true);
    ::wxGetApp().Terminate();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVEAS
 */

void TraceLogFrame::OnSAVEASClick( wxCommandEvent& event )
{
    wxFileDialog fd(this, _("Save logfile"), wxEmptyString,
            wxT("opennx-trace.log"), _("Log files (*.log)"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (fd.ShowModal() == wxID_OK) {
        wxTextFile tf(fd.GetPath());
        if (tf.Exists())
            tf.Open();
        else
            tf.Create();
        tf.Clear();
        wxListItem it;
        wxString line;
        for (int i = 0; i < m_pCtrlTraceLog->GetItemCount(); i++) {
            it.SetId(i);
            it.SetColumn(0);
            it.SetMask(wxLIST_MASK_TEXT);
            m_pCtrlTraceLog->GetItem(it);
            line = it.GetText();
            it.SetColumn(1);
            m_pCtrlTraceLog->GetItem(it);
            line.Append(wxT("\t")).Append(it.GetText());
            it.SetColumn(2);
            m_pCtrlTraceLog->GetItem(it);
            line.Append(wxT("\t")).Append(it.GetText());
            tf.AddLine(line);
        }
        tf.Write();
        tf.Close();
    }
}
