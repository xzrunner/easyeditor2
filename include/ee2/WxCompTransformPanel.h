#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <node2/CompTransform.h>

class wxTextCtrl;

namespace ee2
{

class WxCompTransformPanel : public ee0::WxCompPanel
{
public:
	WxCompTransformPanel(wxWindow* parent, n2::CompTransform& trans,
		const ee0::SubjectMgrPtr& sub_mgr, const ee0::GameObjWithPos& opw);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterTextValue(wxCommandEvent& event);

	void UpdateSharedValue(wxCommandEvent& event);
	void UpdateSharedPatchValue(wxCommandEvent& event);
	void UpdateUniqueValue(wxCommandEvent& event);

private:
	n2::CompTransform& m_ctrans;
	ee0::SubjectMgrPtr m_sub_mgr;
	ee0::GameObjWithPos    m_nwp;

	wxTextCtrl *m_pos_x, *m_pos_y;
	wxTextCtrl* m_angle;
	wxTextCtrl *m_scale_x, *m_scale_y;
	wxTextCtrl *m_shear_x, *m_shear_y;

}; // WxCompTransformPanel

}