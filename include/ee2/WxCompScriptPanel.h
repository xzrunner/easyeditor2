#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

class wxTextCtrl;

namespace ee2
{

class WxCompScriptPanel : public ee0::WxCompPanel
{
public:
	WxCompScriptPanel(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr,
		ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);
	void OnReloadScript(wxCommandEvent& event);

private:
	ECS_WORLD_SELF_DEF
	ee0::SubjectMgrPtr m_sub_mgr;
	ee0::GameObj       m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompScriptPanel

}