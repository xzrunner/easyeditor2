#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>

#include <node2/CompColorMap.h>

namespace ee0 { class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColMapPanel : public ee0::WxCompPanel
{
public:
	WxCompColMapPanel(wxWindow* parent, n2::CompColorMap& col,
		const ee0::SubjectMgrPtr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout(const ee0::SubjectMgrPtr& sub_mgr);

private:
	n2::CompColorMap& m_ccol;

	ee0::WxColorSpinCtrl *m_rmap_ctrl, *m_gmap_ctrl, *m_bmap_ctrl;

}; // WxCompColMapPanel

}