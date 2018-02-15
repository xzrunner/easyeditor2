#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompColorCommon.h>

class wxSpinCtrl;

namespace ee0 { class SubjectMgr; class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColComPanel : public ee0::WxCompPanel
{
public:
	WxCompColComPanel(wxWindow* parent, n2::CompColorCommon& col,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout(ee0::SubjectMgr& sub_mgr);

private:
	n2::CompColorCommon& m_ccol;

	ee0::WxColorSpinCtrl *m_mul_ctrl, *m_add_ctrl;

}; // WxCompColComPanel

}