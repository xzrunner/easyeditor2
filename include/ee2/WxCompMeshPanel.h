#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompMesh.h>
#else
#include <entity2/CompMesh.h>
#endif // GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompMeshPanel : public ee0::WxCompPanel
{
public:
	WxCompMeshPanel(
		wxWindow* parent,
#ifndef GAME_OBJ_ECS
		n2::CompMesh& cmask,
#else
		ecsx::World& world,
		e2::CompMesh& cmesh,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
#ifndef GAME_OBJ_ECS
	n2::CompMesh&       m_cmesh;
#else
	ecsx::World&        m_world;
	e2::CompMesh&       m_cmesh;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;

	wxTextCtrl* m_base_path;

}; // WxCompMeshPanel

}