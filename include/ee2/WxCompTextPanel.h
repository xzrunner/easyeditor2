#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>

#include <node2/CompText.h>

class wxTextCtrl;

namespace ee2
{

class WxCompTextPanel : public ee0::WxCompPanel
{
public:
	WxCompTextPanel(wxWindow* parent, n2::CompText& ctext,
		const ee0::SubjectMgrPtr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterTextValue(wxCommandEvent& event);

private:
	n2::CompText&      m_ctext;
	ee0::SubjectMgrPtr m_sub_mgr;

	wxTextCtrl* m_text;
	wxTextCtrl *m_width, *m_height;
	wxTextCtrl *m_align_h, *m_align_v;
	wxTextCtrl *m_space_h, *m_space_v;

}; // WxCompTextPanel

}