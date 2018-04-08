#pragma once

#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace ee2
{

class NodeCtrlPoint
{
public:
	static void GetNodeCtrlPoints(const n0::SceneNode& obj, sm::vec2 objs[8]);

	//// todo for perspective
	//static void GetNodeCtrlPointsExt(const n0::SceneNode& obj, sm::vec2 objs[4]);

public:
 	// 0 4 1
 	// 5   6
 	// 2 7 3
 	enum Type
 	{
 		LEFT_UP = 0,
 		RIGHT_UP,
 		LEFT_DOWN,
 		RIGHT_DOWN,
 		UP,
 		LEFT,
 		RIGHT,
 		DOWN
 	};
 
 	struct Node
 	{
 		sm::vec2 pos;
 		Type type;
		bool valid;
 
 		void SetInvalid() {
 			valid = false;
 		}
 		bool IsValid() {
 			return valid;
 		}
 	};

}; // NodeCtrlPoint

}