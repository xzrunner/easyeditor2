#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompImage.h>
#else
#include <entity2/CompImage.h>
#endif // GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompImagePanel : public ee0::WxCompPanel
{
public:
	WxCompImagePanel(
		wxWindow* parent, 
#ifndef GAME_OBJ_ECS
		n2::CompImage& cimage,
#else
		ecsx::World& world,
		e2::CompImage& cimage,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
#ifndef GAME_OBJ_ECS
	n2::CompImage&      m_cimage;
#else
	ecsx::World&        m_world;
	e2::CompImage&      m_cimage;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompImagePanel

}