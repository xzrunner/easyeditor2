#include "ee2/WxCompScale9Panel.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompScale9.h>
#else
#include <ecsx/World.h>
#include <entity2/CompScale9.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/spinctrl.h>

namespace ee2
{

WxCompScale9Panel::WxCompScale9Panel(wxWindow* parent, 
#ifdef GAME_OBJ_ECS
                                     ecsx::World& world,
#endif // GAME_OBJ_ECS
	                                 const ee0::GameObj& obj
)
	: ee0::WxCompPanel(parent, "Scale9")
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompScale9Panel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& cscale9 = m_obj->GetSharedComp<n2::CompScale9>();
	m_width->SetValue(cscale9.GetWidth());
	m_height->SetValue(cscale9.GetHeight());
#else
	auto& cscale9 = m_world.GetComponent<e2::CompScale9>(m_obj);
	m_width->SetValue(cscale9.width);
	m_height->SetValue(cscale9.height);
#endif // GAME_OBJ_ECS
}

void WxCompScale9Panel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

#ifndef GAME_OBJ_ECS
	auto& cscale9 = m_obj->GetSharedComp<n2::CompScale9>();
#else
	auto& cscale9 = m_world.GetComponent<e2::CompScale9>(m_obj);
#endif // GAME_OBJ_ECS

	// size
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(cscale9.GetWidth()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, cscale9.GetWidth()));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(cscale9.GetHeight()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, cscale9.GetHeight()));
#else
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(cscale9.width),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, cscale9.width));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(cscale9.height),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, cscale9.height));
#endif // GAME_OBJ_ECS

		Connect(m_width->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompScale9Panel::UpdateSpinValue));
		Connect(m_height->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompScale9Panel::UpdateSpinValue));

		pane_sizer->Add(sizer);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompScale9Panel::UpdateSpinValue(wxSpinEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& cscale9 = m_obj->GetSharedComp<n2::CompScale9>();
#else
	auto& cscale9 = m_world.GetComponent<e2::CompScale9>(m_obj);
#endif // GAME_OBJ_ECS


	int id = event.GetId();
	if (id == m_width->GetId()) 
	{
#ifndef GAME_OBJ_ECS
		cscale9.SetWidth(m_width->GetValue());
#else
		cscale9.SetWidth(m_world, m_width->GetValue());
#endif // GAME_OBJ_ECS
	} 
	else if (id == m_height->GetId()) 
	{
#ifndef GAME_OBJ_ECS
		cscale9.SetHeight(m_height->GetValue());
#else
		cscale9.SetHeight(m_world, m_height->GetValue());
#endif // GAME_OBJ_ECS
	}
}

}