#include "ee2/WxCompColComPanel.h"

#include <ee0/WxColorSpinCtrl.h>

#include <wx/sizer.h>

namespace ee2
{

WxCompColComPanel::WxCompColComPanel(wxWindow* parent, 
	                                 n2::CompColorCommon& col, 
	                                 ee0::SubjectMgr& sub_mgr)
	: ee0::WxCompPanel(parent, "ColorCommon")
	, m_ccol(col)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompColComPanel::RefreshNodeComp()
{
	m_mul_ctrl->Refresh();
	m_add_ctrl->Refresh();
}

void WxCompColComPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);
	pane_sizer->Add(m_mul_ctrl = new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().mul, "mul"));
	pane_sizer->Add(m_add_ctrl = new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().add, "add"));

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

}