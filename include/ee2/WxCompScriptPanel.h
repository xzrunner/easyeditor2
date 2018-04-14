#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#ifndef GAME_OBJ_ECS
namespace n2 { class CompScript; }
#else
namespace ecsx { class World; }
namespace e2 { struct CompScript; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompScriptPanel : public ee0::WxCompPanel
{
public:
	WxCompScriptPanel(
		wxWindow* parent,
#ifndef GAME_OBJ_ECS
		n2::CompScript& cscript,
#else
		const ecsx::World& world,
		e2::CompScript& cscript,
#endif // GAME_OBJ_ECS
		const ee0::SubjectMgrPtr& sub_mgr, 
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);
	void OnReloadScript(wxCommandEvent& event);

private:
#ifndef GAME_OBJ_ECS
	n2::CompScript&    m_cscript;
#else
	const ecsx::World& m_world;
	e2::CompScript&    m_cscript;
#endif // GAME_OBJ_ECS
	ee0::SubjectMgrPtr m_sub_mgr;
	ee0::GameObj       m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompScriptPanel

}