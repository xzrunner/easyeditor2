#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompText.h>

class wxTextCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class WxCompTextPanel : public ee0::WxCompPanel
{
public:
	WxCompTextPanel(wxWindow* parent, n2::CompText& ctext,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateTextValue(wxCommandEvent& event);

private:
	n2::CompText&    m_ctext;
	ee0::SubjectMgr& m_sub_mgr;

	wxTextCtrl* m_text;

}; // WxCompTextPanel

}