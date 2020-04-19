#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

class wxTextCtrl;

namespace ur2 { class Device; }

namespace ee2
{

class WxCompImagePanel : public ee0::WxCompPanel
{
public:
	WxCompImagePanel(const ur2::Device& dev, wxWindow* parent,
        ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
    const ur2::Device& m_dev;

	ECS_WORLD_SELF_DEF
	const ee0::GameObj& m_obj;

	wxTextCtrl* m_filepath;

}; // WxCompImagePanel

}