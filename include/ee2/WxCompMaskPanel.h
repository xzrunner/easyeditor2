#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#include <node2/CompMask.h>

class wxTextCtrl;

namespace ee2
{

class WxCompMaskPanel : public ee0::WxCompPanel
{
public:
	WxCompMaskPanel(wxWindow* parent, n2::CompMask& cmask,
		n0::SceneNode& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);
	void OnSetMaskPath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
	n2::CompMask&  m_cmask;
	n0::SceneNode& m_obj;

	wxTextCtrl *m_base_path, *m_mask_path;

}; // WxCompMaskPanel

}