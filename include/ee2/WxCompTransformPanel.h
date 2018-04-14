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

class WxCompTransformPanel : public ee0::WxCompPanel
{
public:
	WxCompTransformPanel(
		wxWindow* parent, 
		const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObjWithPos& opw
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterTextValue(wxCommandEvent& event);

	void UpdateSharedValue(wxCommandEvent& event);
#ifndef GAME_OBJ_ECS
	void UpdateSharedPatchValue(wxCommandEvent& event);
	void UpdateUniqueValue(wxCommandEvent& event);
#endif // GAME_OBJ_ECS

private:
	ee0::SubjectMgrPtr  m_sub_mgr;
#ifdef GAME_OBJ_ECS
	ecsx::World&        m_world;
#endif // GAME_OBJ_ECS
	ee0::GameObjWithPos m_nwp;

	wxTextCtrl *m_pos_x, *m_pos_y;
	wxTextCtrl* m_angle;
	wxTextCtrl *m_scale_x, *m_scale_y;
	wxTextCtrl *m_shear_x, *m_shear_y;

}; // WxCompTransformPanel

}