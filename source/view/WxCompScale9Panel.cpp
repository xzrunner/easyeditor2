#include "ee2/WxCompScale9Panel.h"

#include <node2/CompScale9.h>

#include <wx/sizer.h>
#include <wx/spinctrl.h>

namespace ee2
{

WxCompScale9Panel::WxCompScale9Panel(wxWindow* parent, n2::CompScale9& cscale9,
	                                 ee0::SubjectMgr& sub_mgr)
	: ee0::WxCompPanel(parent, "Scale9")
	, m_cscale9(cscale9)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompScale9Panel::RefreshNodeComp()
{
	m_width->SetValue(m_cscale9.GetWidth());
	m_height->SetValue(m_cscale9.GetHeight());
}

void WxCompScale9Panel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// size
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.GetWidth()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.GetWidth()));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(m_cscale9.GetHeight()),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1024, m_cscale9.GetHeight()));

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
	if (id == m_width->GetId()) {
		m_cscale9.SetWidth(m_width->GetValue());
	} else if (id == m_height->GetId()) {
		m_cscale9.SetHeight(m_height->GetValue());
	}
}

}