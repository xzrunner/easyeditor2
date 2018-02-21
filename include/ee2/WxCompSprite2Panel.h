#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompSprite2.h>

class wxTextCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class WxCompSprite2Panel : public ee0::WxCompPanel
{
public:
	WxCompSprite2Panel(wxWindow* parent, n2::CompSprite2& cimage,
		ee0::SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
	n2::CompSprite2& m_csprite2;
	ee0::SubjectMgr& m_sub_mgr;

	wxTextCtrl* m_filepath;

}; // WxCompSprite2Panel

}