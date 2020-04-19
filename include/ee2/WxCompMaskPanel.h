#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

class wxTextCtrl;

namespace ur2 { class Device; }

namespace ee2
{

class WxCompMaskPanel : public ee0::WxCompPanel
{
public:
	WxCompMaskPanel(const ur2::Device& dev,
        wxWindow* parent, ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);
	void OnSetMaskPath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
    const ur2::Device& m_dev;

	ECS_WORLD_SELF_DEF
	const ee0::GameObj& m_obj;

	wxTextCtrl *m_base_path, *m_mask_path;

}; // WxCompMaskPanel

}