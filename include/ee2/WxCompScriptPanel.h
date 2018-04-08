#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

namespace n2 { class CompScript; }

class wxTextCtrl;

namespace ee2
{

class WxCompScriptPanel : public ee0::WxCompPanel
{
public:
	WxCompScriptPanel(wxWindow* parent, n2::CompScript& cscript,
		const ee0::SubjectMgrPtr& sub_mgr, const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);
	void OnReloadScript(wxCommandEvent& event);

private:
	n2::CompScript&    m_cscript;
	ee0::SubjectMgrPtr m_sub_mgr;
	ee0::GameObj   m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompScriptPanel

}