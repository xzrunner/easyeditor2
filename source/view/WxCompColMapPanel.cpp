#include "ee2/WxCompColMapPanel.h"

#include <ee0/WxColorSpinCtrl.h>

#include <wx/sizer.h>

namespace ee2
{

WxCompColMapPanel::WxCompColMapPanel(wxWindow* parent, 
	                                 n2::CompColorMap& col, 
	                                 ee0::SubjectMgr& sub_mgr)
	: ee0::WxCompPanel(parent, "ColorMap")
	, m_ccol(col)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompColMapPanel::RefreshNodeComp()
{
	m_rmap_ctrl->Refresh();
	m_gmap_ctrl->Refresh();
	m_bmap_ctrl->Refresh();
}

void WxCompColMapPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);
	pane_sizer->Add(m_rmap_ctrl = new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().rmap, "rmap"));
	pane_sizer->Add(m_gmap_ctrl = new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().gmap, "gmap"));
	pane_sizer->Add(m_bmap_ctrl = new ee0::WxColorSpinCtrl(win, m_ccol.GetColor().bmap, "bmap"));

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

}