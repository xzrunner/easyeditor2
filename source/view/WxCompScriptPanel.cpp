#include "ee2/WxCompScriptPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompScript.h>
#else
#include <ee0/CompEntityEditor.h>
#include <entity0/World.h>
#include <entity2/CompScript.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompScriptPanel::WxCompScriptPanel(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr,
	                                 ECS_WORLD_PARAM const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Script")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompScriptPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& filepath = m_obj->GetUniqueComp<n0::CompIdentity>().GetFilepath();
#else
	auto& filepath = *m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath;
#endif // GAME_OBJ_ECS
	m_filepath->SetValue(filepath);
}

void WxCompScriptPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		auto& filepath = m_obj->GetUniqueComp<n0::CompIdentity>().GetFilepath();
#else
		auto& filepath = *m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath;
#endif // GAME_OBJ_ECS
		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, filepath,
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
		auto filepath = dlg.GetPath().ToStdString();
#ifndef GAME_OBJ_ECS
		m_obj->GetUniqueComp<n0::CompIdentity>().SetFilepath(filepath);
#else
		m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath
			= std::make_unique<std::string>(filepath);
#endif // GAME_OBJ_ECS

		// todo
#ifndef GAME_OBJ_ECS
		auto& cscript = m_obj->GetUniqueComp<n2::CompScript>();
		cscript.SetFilepath(filepath);
		cscript.Reload(m_obj);
#endif // GAME_OBJ_ECS
	}
}

void WxCompScriptPanel::OnReloadScript(wxCommandEvent& event)
{
	// todo
#ifndef GAME_OBJ_ECS
	auto& cscript = m_obj->GetUniqueComp<n2::CompScript>();
	auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
	cscript.SetFilepath(cid.GetFilepath());
	cscript.Reload(m_obj);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}