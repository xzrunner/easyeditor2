#include "ee2/WxCompScriptPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node2/CompScript.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompScriptPanel::WxCompScriptPanel(wxWindow* parent, n2::CompScript& cscript,
	                                 const ee0::SubjectMgrPtr& sub_mgr,
	                                 const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Script")
	, m_cscript(cscript)
	, m_sub_mgr(sub_mgr)
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompScriptPanel::RefreshNodeComp()
{
	m_filepath->SetValue(m_cscript.GetFilepath());
}

void WxCompScriptPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, m_cscript.GetFilepath(),
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(WxCompScriptPanel::OnSetFilepath));

		pane_sizer->Add(sizer);
	}
	// reload
	{
		wxButton* btn = new wxButton(win, wxID_ANY, wxT("reload"), wxDefaultPosition, wxSize(60, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompScriptPanel::OnReloadScript));

		pane_sizer->Add(btn, 0, wxALIGN_CENTER_HORIZONTAL);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompScriptPanel::OnSetFilepath(wxCommandEvent& event)
{
	std::string filter = "*.lua";
	wxFileDialog dlg(this, wxT("Choose script"), wxEmptyString, filter);
	if (dlg.ShowModal() == wxID_OK)
	{
		auto& path = dlg.GetPath();
		m_cscript.SetFilepath(path.ToStdString());
		m_cscript.Reload(m_obj);
	}
}

void WxCompScriptPanel::OnReloadScript(wxCommandEvent& event)
{
	m_cscript.Reload(m_obj);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}