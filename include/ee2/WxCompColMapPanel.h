#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompColorMap.h>

class wxSpinCtrl;

namespace ee0 { class SubjectMgr; class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColMapPanel : public ee0::WxCompPanel
{
public:
	WxCompColMapPanel(wxWindow* parent, n2::CompColorMap& col,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

private:
	n2::CompColorMap& m_ccol;
	ee0::SubjectMgr& m_sub_mgr;

	ee0::WxColorSpinCtrl *m_rmap_ctrl, *m_gmap_ctrl, *m_bmap_ctrl;

}; // WxCompColMapPanel

}