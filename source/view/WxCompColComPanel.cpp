#include "ee2/WxCompColComPanel.h"

#include <ee0/WxColorSpinCtrl.h>

#include <wx/sizer.h>

namespace ee2
{

WxCompColComPanel::WxCompColComPanel(wxWindow* parent,
#ifndef GAME_OBJ_ECS
	                                 n2::CompColorCommon& col,
#else
	                                 e2::CompColorCommon& col,
#endif // GAME_OBJ_ECS
	                                 const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::WxCompPanel(parent, "ColorCommon")
	, m_ccol(col)
{
	InitLayout(sub_mgr);
	Expand();
}

void WxCompColComPanel::RefreshNodeComp()
{
	m_mul_ctrl->Refresh();
	m_add_ctrl->Refresh();
}

void WxCompColComPanel::InitLayout(const ee0::SubjectMgrPtr& sub_mgr)
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);
#ifndef GAME_OBJ_ECS
	pane_sizer->Add(m_mul_ctrl =
		new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().mul, "mul", sub_mgr));
	pane_sizer->Add(m_add_ctrl =
		new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().add, "add", sub_mgr));
#else
	pane_sizer->Add(m_mul_ctrl =
		new ee0::WxColorSpinCtrl(win, m_ccol.col.mul, "mul", sub_mgr));
	pane_sizer->Add(m_add_ctrl =
		new ee0::WxColorSpinCtrl(win, m_ccol.col.add, "add", sub_mgr));
#endif // GAME_OBJ_ECS

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

}