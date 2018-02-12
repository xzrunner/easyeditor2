#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompColorCommon.h>

class wxSpinCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class WxCompColorCommonPanel : public ee0::WxCompPanel
{
public:
	WxCompColorCommonPanel(wxWindow* parent, n2::CompColorCommon& col,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateColorValue(wxSpinEvent& event);

private:
	n2::CompColorCommon& m_ccol;
	ee0::SubjectMgr& m_sub_mgr;

	wxSpinCtrl *m_mul_r, *m_mul_g, *m_mul_b, *m_mul_a;
	wxSpinCtrl *m_add_r, *m_add_g, *m_add_b, *m_add_a;

}; // WxCompColorCommonPanel

}