#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/Config.h>

#ifndef GAME_OBJ_ECS
namespace n2 { class CompScissor; }
#else
namespace e2 { struct CompScissor; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompScissorPanel : public ee0::WxCompPanel
{
public:
	WxCompScissorPanel(
		wxWindow* parent,
#ifndef GAME_OBJ_ECS
		n2::CompScissor& cscissor,
#else
		e2::CompScissor& cscissor,
#endif // GAME_OBJ_ECS
		const ee0::SubjectMgrPtr& sub_mgr
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterTextValue(wxCommandEvent& event);

private:
#ifndef GAME_OBJ_ECS
	n2::CompScissor&   m_cscissor;
#else
	e2::CompScissor&   m_cscissor;
#endif // GAME_OBJ_ECS
	ee0::SubjectMgrPtr m_sub_mgr;

	wxTextCtrl *m_xmin, *m_ymin, *m_xmax, *m_ymax;

}; // WxCompScissorPanel

}