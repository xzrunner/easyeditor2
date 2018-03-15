#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>

#include <node2/CompColorCommon.h>

namespace ee0 { class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColComPanel : public ee0::WxCompPanel
{
public:
	WxCompColComPanel(wxWindow* parent, n2::CompColorCommon& col,
		const ee0::SubjectMgrPtr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout(const ee0::SubjectMgrPtr& sub_mgr);

private:
	n2::CompColorCommon& m_ccol;

	ee0::WxColorSpinCtrl *m_mul_ctrl, *m_add_ctrl;

}; // WxCompColComPanel

}