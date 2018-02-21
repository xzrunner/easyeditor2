#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompImage.h>

class wxTextCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class WxCompImagePanel : public ee0::WxCompPanel
{
public:
	WxCompImagePanel(wxWindow* parent, n2::CompImage& cimage,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
	n2::CompImage&   m_cimage;
	ee0::SubjectMgr& m_sub_mgr;

	wxTextCtrl* m_filepath;

}; // WxCompImagePanel

}