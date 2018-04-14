#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompMask.h>
#else
#include <entity2/CompMask.h>
#endif // GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompMaskPanel : public ee0::WxCompPanel
{
public:
	WxCompMaskPanel(
		wxWindow* parent, 
#ifndef GAME_OBJ_ECS
		n2::CompMask& cmask,
#else
		ecsx::World& world,
		e2::CompMask& cmask,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);
	void OnSetMaskPath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
#ifndef GAME_OBJ_ECS
	n2::CompMask&       m_cmask;
#else
	ecsx::World&        m_world;
	e2::CompMask&       m_cmask;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;

	wxTextCtrl *m_base_path, *m_mask_path;

}; // WxCompMaskPanel

}