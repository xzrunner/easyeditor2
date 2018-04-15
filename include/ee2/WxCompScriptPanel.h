#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompScriptPanel : public ee0::WxCompPanel
{
public:
	WxCompScriptPanel(
		wxWindow* parent,
		const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
		const ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);
	void OnReloadScript(wxCommandEvent& event);

private:
#ifdef GAME_OBJ_ECS
	const ecsx::World& m_world;
#endif // GAME_OBJ_ECS
	ee0::SubjectMgrPtr m_sub_mgr;
	ee0::GameObj       m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompScriptPanel

}