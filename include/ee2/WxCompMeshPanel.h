#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

class wxTextCtrl;

namespace ee2
{

class WxCompMeshPanel : public ee0::WxCompPanel
{
public:
	WxCompMeshPanel(wxWindow* parent, ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
	ECS_WORLD_SELF_DEF
	const ee0::GameObj& m_obj;

	wxTextCtrl* m_base_path;

}; // WxCompMeshPanel

}