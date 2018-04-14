#include "ee2/WxCompScale9Panel.h"

#ifndef GAME_OBJ_ECS
#include <node2/CompScale9.h>
#else
#include <entity2/CompScale9.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/spinctrl.h>

namespace ee2
{

WxCompScale9Panel::WxCompScale9Panel(wxWindow* parent, 
#ifndef GAME_OBJ_ECS
	                                 n2::CompScale9& cscale9
#else
                                     ecsx::World& world,
		                             e2::CompScale9& cscale9
#endif // GAME_OBJ_ECS
)
	: ee0::WxCompPanel(parent, "Scale9")
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_cscale9(cscale9)
{
	InitLayout();
	Expand();
}

void WxCompScale9Panel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	m_width->SetValue(m_cscale9.GetWidth());
	m_height->SetValue(m_cscale9.GetHeight());
#else
	m_width->SetValue(m_cscale9.width);
	m_height->SetValue(m_cscale9.height);
#endif // GAME_OBJ_ECS
}

void WxCompScale9Panel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// size
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.GetWidth()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.GetWidth()));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.GetHeight()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.GetHeight()));
#else
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.width),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.width));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.height),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.height));
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
	int id = event.GetId();
	if (id == m_width->GetId()) 
	{
#ifndef GAME_OBJ_ECS
		m_cscale9.SetWidth(m_width->GetValue());
#else
		m_cscale9.SetWidth(m_world, m_width->GetValue());
#endif // GAME_OBJ_ECS
	} 
	else if (id == m_height->GetId()) 
	{
#ifndef GAME_OBJ_ECS
		m_cscale9.SetHeight(m_height->GetValue());
#else
		m_cscale9.SetHeight(m_world, m_height->GetValue());
#endif // GAME_OBJ_ECS
	}
}

}