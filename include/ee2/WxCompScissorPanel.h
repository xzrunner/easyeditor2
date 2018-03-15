#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>

namespace n2 { class CompScissor; }

class wxTextCtrl;

namespace ee2
{

class WxCompScissorPanel : public ee0::WxCompPanel
{
public:
	WxCompScissorPanel(wxWindow* parent, n2::CompScissor& cscissor,
		const ee0::SubjectMgrPtr& sub_mgr);
	
	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterTextValue(wxCommandEvent& event);

private:
	n2::CompScissor&   m_cscissor;
	ee0::SubjectMgrPtr m_sub_mgr;

	wxTextCtrl *m_xmin, *m_ymin, *m_xmax, *m_ymax;

}; // WxCompScissorPanel

}