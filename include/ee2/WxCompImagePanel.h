#pragma once

#include <ee0/WxCompPanel.h>

#include <node2/CompImage.h>

class wxTextCtrl;

namespace ee2
{

class WxCompImagePanel : public ee0::WxCompPanel
{
public:
	WxCompImagePanel(wxWindow* parent, n2::CompImage& cimage);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetFilepath(wxCommandEvent& event);

private:
	n2::CompImage& m_cimage;

	wxTextCtrl* m_filepath;

}; // WxCompImagePanel

}