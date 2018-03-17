#pragma once

#include <ee0/WxCompPanel.h>

namespace n2 { class CompScript; }

class wxTextCtrl;

namespace ee2
{

class WxCompScriptPanel : public ee0::WxCompPanel
{
public:
	WxCompScriptPanel(wxWindow* parent, n2::CompScript& cscript);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
	n2::CompScript& m_cscript;

	wxTextCtrl* m_filepath;

}; // WxCompScriptPanel

}