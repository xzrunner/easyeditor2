#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#include <node2/CompMesh.h>

class wxTextCtrl;

namespace ee2
{

class WxCompMeshPanel : public ee0::WxCompPanel
{
public:
	WxCompMeshPanel(wxWindow* parent, n2::CompMesh& cmask,
		n0::SceneNode& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
	n2::CompMesh&  m_cmesh;
	n0::SceneNode& m_obj;

	wxTextCtrl* m_base_path;

}; // WxCompMeshPanel

}