#include "ee2/WxCompScriptPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <ee0/CompNodeEditor.h>
#include <node0/SceneNode.h>
#include <node2/CompScript.h>
#else
#include <ee0/CompEntityEditor.h>
#include <ecsx/World.h>
#include <entity2/CompScript.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompScriptPanel::WxCompScriptPanel(wxWindow* parent, 
#ifndef GAME_OBJ_ECS
	                                 n2::CompScript& cscript,
#else
	                                 const ecsx::World& world,
		                             e2::CompScript& cscript,
#endif // GAME_OBJ_ECS
	                                 const ee0::SubjectMgrPtr& sub_mgr,
	                                 const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Script")
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_cscript(cscript)
	, m_sub_mgr(sub_mgr)
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompScriptPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& filepath = m_obj->GetUniqueComp<ee0::CompNodeEditor>().GetFilepath();
#else
	auto& filepath = m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath;
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
		auto& filepath = m_obj->GetUniqueComp<ee0::CompNodeEditor>().GetFilepath();
#else
		auto& filepath = m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath;
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
		m_obj->GetUniqueComp<ee0::CompNodeEditor>().SetFilepath(filepath);
#else
		m_world.GetComponent<ee0::CompEntityEditor>(m_obj).filepath = filepath;
#endif // GAME_OBJ_ECS

		// todo
#ifndef GAME_OBJ_ECS
		m_cscript.SetFilepath(filepath);
		m_cscript.Reload(m_obj);
#endif // GAME_OBJ_ECS
	}
}

void WxCompScriptPanel::OnReloadScript(wxCommandEvent& event)
{
	// todo
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
	m_cscript.SetFilepath(ceditor.GetFilepath());
	m_cscript.Reload(m_obj);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}