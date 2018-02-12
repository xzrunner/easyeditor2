#include "ee2/WxCompColorCommonPanel.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace ee2
{

WxCompColorCommonPanel::WxCompColorCommonPanel(wxWindow* parent, 
	                                           n2::CompColorCommon& col, 
	                                           ee0::SubjectMgr& sub_mgr)
	: ee0::WxCompPanel(parent, "ColorCommon")
	, m_ccol(col)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompColorCommonPanel::RefreshNodeComp()
{
	auto& mul = m_ccol.GetColor().mul;
	m_mul_r->SetValue(mul.r);
	m_mul_g->SetValue(mul.g);
	m_mul_b->SetValue(mul.b);
	m_mul_a->SetValue(mul.a);

	auto& add = m_ccol.GetColor().add;
	m_add_r->SetValue(add.r);
	m_add_g->SetValue(add.g);
	m_add_b->SetValue(add.b);
	m_add_a->SetValue(add.a);
}

void WxCompColorCommonPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	static const wxSize INPUT_SIZE(65, 19);

	// mul
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		auto& mul = m_ccol.GetColor().mul;
		sizer->Add(m_mul_r = new wxSpinCtrl(win, wxID_ANY, std::to_string(mul.r)));
		sizer->Add(m_mul_g = new wxSpinCtrl(win, wxID_ANY, std::to_string(mul.g)));
		sizer->Add(m_mul_b = new wxSpinCtrl(win, wxID_ANY, std::to_string(mul.b)));
		sizer->Add(m_mul_a = new wxSpinCtrl(win, wxID_ANY, std::to_string(mul.a)));

		Connect(m_mul_r->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WxCompColorCommonPanel::UpdateColorValue));
		Connect(m_mul_g->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WxCompColorCommonPanel::UpdateColorValue));
		Connect(m_mul_b->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WxCompColorCommonPanel::UpdateColorValue));
		Connect(m_mul_a->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WxCompColorCommonPanel::UpdateColorValue));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompColorCommonPanel::UpdateColorValue(wxSpinEvent& event)
{
	// mul
	auto& mul = m_ccol.GetColor().mul;
	if (event.GetId() == m_mul_r->GetId()) {
		mul.r = m_mul_r->GetValue();
	}
}

}