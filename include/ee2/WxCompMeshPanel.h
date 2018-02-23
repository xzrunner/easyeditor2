#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompMesh.h>

class wxTextCtrl;

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class WxCompMeshPanel : public ee0::WxCompPanel
{
public:
	WxCompMeshPanel(wxWindow* parent, n2::CompMesh& cmask,
		ee0::SubjectMgr& sub_mgr, n0::SceneNode& node);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);

private:
	n0::SceneNodePtr CreateNodeFromFile();

private:
	n2::CompMesh&    m_cmesh;
	ee0::SubjectMgr& m_sub_mgr;
	n0::SceneNode&   m_node;

	wxTextCtrl* m_base_path;

}; // WxCompMeshPanel

}